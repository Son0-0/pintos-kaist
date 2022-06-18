/* vm.c: Generic interface for virtual memory objects. */

#include "threads/malloc.h"
#include "vm/vm.h"
#include "vm/inspect.h"

// * Project 3 추가
#include "threads/mmu.h"
#include "userprog/syscall.h"

/* Initializes the virtual memory subsystem by invoking each subsystem's
 * intialize codes. */
void vm_init(void)
{
  vm_anon_init();
  vm_file_init();
#ifdef EFILESYS /* For project 4 */
  pagecache_init();
#endif
  register_inspect_intr();
  /* DO NOT MODIFY UPPER LINES. */
  /* TODO: Your code goes here. */
  list_init(&frame_list);
}

/* Get the type of the page. This function is useful if you want to know the
 * type of the page after it will be initialized.
 * This function is fully implemented now. */
enum vm_type
page_get_type(struct page *page)
{
  int ty = VM_TYPE(page->operations->type);
  switch (ty)
  {
  case VM_UNINIT:
    return VM_TYPE(page->uninit.type);
  default:
    return ty;
  }
}

/* Helpers */
static struct frame *vm_get_victim(void);
static bool vm_do_claim_page(struct page *page);
static struct frame *vm_evict_frame(void);

/* Create the pending page object with initializer. If you want to create a
 * page, do not create it directly and make it through this function or
 * `vm_alloc_page`. */
bool vm_alloc_page_with_initializer(enum vm_type type, void *upage, bool writable,
                                    vm_initializer *init, void *aux)
{

  ASSERT(VM_TYPE(type) != VM_UNINIT)

  bool success = false;
  struct supplemental_page_table *spt = &thread_current()->spt;

  /* Check wheter the upage is already occupied or not. */
  if (spt_find_page(spt, upage) == NULL)
  {
    /* TODO: Create the page, fetch the initializer according to the VM type,
     * TODO: and then create "uninit" page struct by calling uninit_new. You
     * TODO: should modify the field after calling the uninit_new. */
    struct page *p = (struct page *)malloc(sizeof(struct page));

    // printf("va: %p | type: %d | writable: %d\n", upage, type, writable);

    if (type == VM_ANON)
      uninit_new(p, pg_round_down(upage), init, type, aux, anon_initializer);
    else if (type == VM_FILE)
      uninit_new(p, pg_round_down(upage), init, type, aux, file_backed_initializer);

    p->writable = writable;
    /* TODO: Insert the page into the spt. */
    success = spt_insert_page(spt, p);
  }
  return success;
err:
  return success;
  // return false;
}

/* Find VA from spt and return page. On error, return NULL. */
struct page *
spt_find_page(struct supplemental_page_table *spt UNUSED, void *va UNUSED)
{
  // struct page *page = NULL;
  /* TODO: Fill this function. */
  struct page p;
  struct hash_elem *e;

  p.va = pg_round_down(va);
  e = hash_find(&(spt->pages), &p.hash_elem);

  return e != NULL ? hash_entry(e, struct page, hash_elem) : NULL;
}

/* Insert PAGE into spt with validation. */
bool spt_insert_page(struct supplemental_page_table *spt UNUSED, struct page *page UNUSED)
{
  int succ = false;
  /* TODO: Fillthis function. */
  if (hash_insert(&(spt->pages), &(page->hash_elem)) == NULL)
  {
    succ = true;
  }
  return succ;
}

void spt_remove_page(struct supplemental_page_table *spt, struct page *page)
{
  vm_dealloc_page(page);
  return true;
}

/* Get the struct frame, that will be evicted. */
static struct frame *
vm_get_victim(void)
{
  struct frame *victim = NULL;
  /* TODO: The policy for eviction is up to you. */

  struct list_elem *cur_elem = list_begin(&frame_list);
  while (cur_elem != list_end(&frame_list)) {
    struct frame *cur_f = list_entry(cur_elem, struct frame, frame_elem);
    if(!pml4_is_accessed(thread_current()->pml4, cur_f->page->va)) {
      if (thread_current()->stack_btm <= cur_f->page->va && cur_f->page->va <= USER_STACK) {
        cur_elem = list_next(cur_elem);
        continue;
      }
      list_remove(cur_elem);
      puts(">>>>>>>>>>>>>>>>>> return >>>>>>>>>>>>>>>>");
      printf("%p %p VM_TYPE: %d\n", cur_f->page->va, cur_f->kva, page_get_type(cur_f->page));
      return cur_f;
    } else {
      pml4_set_accessed(thread_current()->pml4, cur_f->page->va, false);
      cur_elem = list_next(cur_elem);
    }
  }

  // cur_elem = list_begin(&frame_list);
  // return list_entry(cur_elem, struct frame, frame_elem);

  cur_elem = list_begin(&frame_list);
  while (cur_elem != list_end(&frame_list)) {
    struct frame *cur_f = list_entry(cur_elem, struct frame, frame_elem);
    // printf("%p %p VM_TYPE: %d\n", cur_f->page->va, cur_f->kva, page_get_type(cur_f->page));
    if (thread_current()->stack_btm <= cur_f->page->va && cur_f->page->va <= USER_STACK) {
      cur_elem = list_next(cur_elem);
      continue;
    }
    if(!pml4_is_accessed(thread_current()->pml4, cur_f->page->va)) {
      if (thread_current()->stack_btm <= cur_f->page->va && cur_f->page->va <= USER_STACK) {
        cur_elem = list_next(cur_elem);
        continue;
      }
      list_remove(cur_elem);
      puts(">>>>>>>>>>>>>>>>>> return >>>>>>>>>>>>>>>>");
      printf("%p %p VM_TYPE: %d\n", cur_f->page->va, cur_f->kva, page_get_type(cur_f->page));
      return cur_f;
    } else {
      pml4_set_accessed(thread_current()->pml4, cur_f->page->va, false);
      cur_elem = list_next(cur_elem);
    }
  }
}

/* Evict one page and return the corresponding frame.
 * Return NULL on error.*/
static struct frame *
vm_evict_frame(void)
{
  struct frame *victim UNUSED = vm_get_victim();
  /* TODO: swap out the victim and return the evicted frame. */
  if(swap_out(victim->page))
    return victim;
  // if (page_get_type(victim->page) == 2) {
  //   if(swap_out(victim->page))
  //     return victim;
  // }
  // else {
  //   victim->page->frame = NULL;
  //   pml4_clear_page(thread_current()->pml4, victim->page->va);
  //   return victim;
  // }

  return NULL;
}

/* palloc() and get frame. If there is no available page, evict the page
 * and return it. This always return valid address. That is, if the user pool
 * memory is full, this function evicts the frame to get the available memory
 * space.*/
static struct frame *
vm_get_frame(void)
{
  struct frame *frame = (struct frame *)malloc(sizeof(struct frame));
  /* TODO: Fill this function. */
  frame->kva = palloc_get_page(PAL_USER);
  frame->page = NULL;
  
  if (frame->kva == NULL) {
    frame = vm_evict_frame();
    frame->page = NULL;
  }
  ASSERT(frame != NULL);
  ASSERT(frame->page == NULL);

  list_push_back(&frame_list, &frame->frame_elem);

  return frame;
}

/* Growing the stack. */
static void
vm_stack_growth(void *addr UNUSED)
{
  vm_alloc_page(VM_ANON, addr, true);
  thread_current()->stack_btm = addr;
}

/* Handle the fault on write_protected page */
static bool
vm_handle_wp(struct page *page UNUSED)
{
}

/* Return true on success */
bool vm_try_handle_fault(struct intr_frame *f UNUSED, void *addr UNUSED,
                         bool user UNUSED, bool write UNUSED, bool not_present UNUSED)
{
  struct supplemental_page_table *spt UNUSED = &thread_current()->spt;
  struct page *page = spt_find_page(spt, addr); // * ref: 혜진
  /* TODO: Validate the fault */
  /* TODO: Your code goes here */
  if (is_kernel_vaddr(addr))
    return false;
  if (page && not_present) {
    return vm_do_claim_page(page);
  }
  if (f->rsp - 8 <= addr) {
    uint64_t size = thread_current()->stack_btm;
    while (addr < size) {
      size -= PGSIZE;
      vm_stack_growth(size);
      if (spt_find_page(spt, size)) {
        if(!vm_claim_page(size))
          return false;
      }
    }
    return true;
  }
  return false;
}

/* Free the page.
 * DO NOT MODIFY THIS FUNCTION. */
void vm_dealloc_page(struct page *page)
{
  destroy(page);
  free(page);
}

/* Claim the page that allocate on VA. */
bool vm_claim_page(void *va UNUSED)
{
  struct page *page = spt_find_page(&thread_current()->spt, va);
  /* TODO: Fill this function */
  if (page == NULL)
    return false;
  return vm_do_claim_page(page);
}

/* Claim the PAGE and set up the mmu. */
static bool
vm_do_claim_page(struct page *page)
{
  struct frame *frame = vm_get_frame();

  /* Set links */
  frame->page = page;
  page->frame = frame;
  
  /* TODO: Insert page table entry to map page's VA to frame's PA. */
  pml4_set_page(thread_current()->pml4, page->va, frame->kva, page->writable); // * ref: 혜진
  return swap_in(page, frame->kva);
}

/* Initialize new supplemental page table */
void supplemental_page_table_init(struct supplemental_page_table *spt UNUSED)
{
  hash_init(&(spt->pages), page_hash, page_less, NULL);
}

/* Copy supplemental page table from src to dst */
bool supplemental_page_table_copy(struct supplemental_page_table *dst UNUSED,
                                  struct supplemental_page_table *src UNUSED)
{
  
  return hash_apply(&src->pages, &copy_page);
}

/* Free the resource hold by the supplemental page table */
void supplemental_page_table_kill(struct supplemental_page_table *spt UNUSED)
{
  /* TODO: Destroy all the supplemental_page_table hold by thread and
   * TODO: writeback all the modified contents to the storage. */
  hash_clear(&spt->pages, &delete_page);
}

/* Returns true if page a precedes page b. */
bool page_less(const struct hash_elem *a_,
               const struct hash_elem *b_, void *aux UNUSED)
{
  const struct page *a = hash_entry(a_, struct page, hash_elem);
  const struct page *b = hash_entry(b_, struct page, hash_elem);

  return a->va < b->va;
}

/* Returns a hash value for page p. */
unsigned
page_hash(const struct hash_elem *p_, void *aux UNUSED)
{
  const struct page *p = hash_entry(p_, struct page, hash_elem);
  return hash_bytes(&p->va, sizeof p->va);
}

void delete_page(const struct hash_elem *a_, void *aux UNUSED)
{
  struct page *page = hash_entry(a_, struct page, hash_elem);
  vm_dealloc_page(page);
}

bool copy_page(const struct hash_elem *a_, void *aux UNUSED)
{
  // * hash_elem으로 부모 page를 찾고
  struct page *page = hash_entry(a_, struct page, hash_elem);
  bool success = vm_alloc_page_with_initializer(page_get_type(page), page->va, page->writable, NULL, NULL);
  
  // * 부모 page의 정보를 활용하여 initializer 호출
  if (success) {
    // * 자식 테이블에 방금 생성된 페이지를 찾아서 frame 할당과 부모 frame memcopy
    if (page->frame) {
      struct page *newpage = spt_find_page(&thread_current()->spt, page->va);
      newpage->frame = vm_get_frame();
      memcpy(newpage->frame->kva, page->frame->kva, PGSIZE);
      newpage->frame->page = newpage;
      success = pml4_set_page(thread_current()->pml4, newpage->va, newpage->frame->kva, newpage->writable);
      if (!success)
        return success;
    }
  }

  return success;
}

bool munmap_page(const struct hash_elem *a, void *aux UNUSED) {
  struct page *cur_page = hash_entry(a, struct page, hash_elem);
  if (cur_page) {
    struct file *file = cur_page->mfile;
    if (file) {
      munmap(cur_page->va);
    }
  }
  return true;
}