/* vm.c: Generic interface for virtual memory objects. */

#include "threads/malloc.h"
#include "vm/vm.h"
#include "vm/inspect.h"

// * Project 3 추가
#include "threads/mmu.h"

/* Initializes the virtual memory subsystem by invoking each subsystem's
 * intialize codes. */
void
vm_init (void) {
	vm_anon_init ();
	vm_file_init ();
#ifdef EFILESYS  /* For project 4 */
	pagecache_init ();
#endif
	register_inspect_intr ();
	/* DO NOT MODIFY UPPER LINES. */
	/* TODO: Your code goes here. */
  list_init(&frame_list);
}

/* Get the type of the page. This function is useful if you want to know the
 * type of the page after it will be initialized.
 * This function is fully implemented now. */
enum vm_type
page_get_type (struct page *page) {
	int ty = VM_TYPE (page->operations->type);
	switch (ty) {
		case VM_UNINIT:
			return VM_TYPE (page->uninit.type);
		default:
			return ty;
	}
}

/* Helpers */
static struct frame *vm_get_victim (void);
static bool vm_do_claim_page (struct page *page);
static struct frame *vm_evict_frame (void);

/* Create the pending page object with initializer. If you want to create a
 * page, do not create it directly and make it through this function or
 * `vm_alloc_page`. */
bool
vm_alloc_page_with_initializer (enum vm_type type, void *upage, bool writable,
		vm_initializer *init, void *aux) {

	ASSERT (VM_TYPE(type) != VM_UNINIT)

 	bool success = false;
	struct supplemental_page_table *spt = &thread_current()->spt;

	/* Check wheter the upage is already occupied or not. */
	if (spt_find_page (spt, upage) == NULL) {
		/* TODO: Create the page, fetch the initializer according to the VM type,
		 * TODO: and then create "uninit" page struct by calling uninit_new. You
		 * TODO: should modify the field after calling the uninit_new. */
    // 연어: 필드 바꾸라는데 뭐라는지 모르겠음
    struct page *p = (struct page*)malloc(sizeof(struct page));

    if (type == VM_ANON)
      uninit_new(p, pg_round_down(upage), init, type, aux, anon_initializer);
    else if(type == VM_FILE)
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
spt_find_page (struct supplemental_page_table *spt UNUSED, void *va UNUSED) {
	// struct page *page = NULL;
	/* TODO: Fill this function. */
	struct page p;
	struct hash_elem *e;

	p.va = pg_round_down(va);
	e = hash_find(&(spt->pages), &p.hash_elem);
	
	return e != NULL ? hash_entry (e, struct page, hash_elem) : NULL;
}

/* Insert PAGE into spt with validation. */
bool
spt_insert_page (struct supplemental_page_table *spt UNUSED, struct page *page UNUSED) {
	int succ = false;
	/* TODO: Fillthis function. */
	if(hash_insert (&(spt->pages), &(page->hash_elem)) == NULL){
		succ = true;
	}
	return succ;
}

void
spt_remove_page (struct supplemental_page_table *spt, struct page *page) {
	vm_dealloc_page (page);
	return true;
}

/* Get the struct frame, that will be evicted. */
static struct frame *
vm_get_victim (void) {
	struct frame *victim = NULL;
	 /* TODO: The policy for eviction is up to you. */

	return victim;
}

/* Evict one page and return the corresponding frame.
 * Return NULL on error.*/
static struct frame *
vm_evict_frame (void) {
	struct frame *victim UNUSED = vm_get_victim ();
	/* TODO: swap out the victim and return the evicted frame. */

	return NULL;
}

/* palloc() and get frame. If there is no available page, evict the page
 * and return it. This always return valid address. That is, if the user pool
 * memory is full, this function evicts the frame to get the available memory
 * space.*/
static struct frame *
vm_get_frame (void) {
	struct frame *frame = (struct frame*)malloc(sizeof(struct frame));
	/* TODO: Fill this function. */
	//연어
	frame->kva = palloc_get_page(PAL_USER);
	frame->page = NULL;
  if (frame->kva == NULL){
    frame = NULL;
		PANIC("todo");
	}
	ASSERT (frame != NULL);
	ASSERT (frame->page == NULL);

  // Son0-0
  // list_push_back(&frame_list, &frame->frame_elem);

	return frame;
}

/* Growing the stack. */
static void
vm_stack_growth (void *addr UNUSED) {
}

/* Handle the fault on write_protected page */
static bool
vm_handle_wp (struct page *page UNUSED) {
}

/* Return true on success */
bool
vm_try_handle_fault (struct intr_frame *f UNUSED, void *addr UNUSED,
		bool user UNUSED, bool write UNUSED, bool not_present UNUSED) {
	struct supplemental_page_table *spt UNUSED = &thread_current ()->spt;
	struct page *page = spt_find_page(spt, addr); // * ref: 혜진
  // printf("address: %u / user: %d / write: %d not_present: %d\n", addr, user, write, not_present);
	/* TODO: Validate the fault */
	/* TODO: Your code goes here */
	struct thread *cur = thread_current();
	bool success = false;

	// check_address(addr);
	if(is_kernel_vaddr(addr)){
		return false;
	}

	if (not_present){
     	if (f->rsp == addr+8){
         	while (cur->stack_bottom > addr){
				vm_alloc_page_with_initializer (VM_ANON||VM_MARKER_0, cur->stack_bottom-PGSIZE, true, NULL, NULL);
				if (success = vm_claim_page(cur->stack_bottom-PGSIZE)){
					cur->stack_bottom -= PGSIZE;
				}
			}
			return success;
		}
		if (page) {
			return vm_do_claim_page(page);
		}
	}
	return false;
}


/* Free the page.
 * DO NOT MODIFY THIS FUNCTION. */
void
vm_dealloc_page (struct page *page) {
	destroy (page);
	free (page);
}

/* Claim the page that allocate on VA. */
bool
vm_claim_page (void *va UNUSED) {
	struct page *page = spt_find_page(&thread_current()->spt,va); 
	/* TODO: Fill this function */

	if (page == NULL)
		return false;
  // printf("current tid: %d and claim page va: %p\n", thread_current()->tid, page->va);
	return vm_do_claim_page (page);
}

/* Claim the PAGE and set up the mmu. */
static bool
vm_do_claim_page (struct page *page) {
  struct frame *frame = vm_get_frame();

  /* Set links */
  frame->page = page;
  page->frame = frame;

  /* TODO: Insert page table entry to map page's VA to frame's PA. */
  pml4_set_page(thread_current()->pml4, page->va, frame->kva, page->writable); // * ref: 혜진
  return swap_in (page, frame->kva);
}

/* Initialize new supplemental page table */
void
supplemental_page_table_init (struct supplemental_page_table *spt UNUSED) {
	hash_init (&(spt->pages), page_hash, page_less, NULL);
}

/* Copy supplemental page table from src to dst */
bool
supplemental_page_table_copy (struct supplemental_page_table *dst UNUSED,
		struct supplemental_page_table *src UNUSED) {
      hash_apply(&src->pages, &copy_page);
      return true;
}

/* Free the resource hold by the supplemental page table */
void
supplemental_page_table_kill (struct supplemental_page_table *spt UNUSED) {
	/* TODO: Destroy all the supplemental_page_table hold by thread and
	 * TODO: writeback all the modified contents to the storage. */
  hash_clear(&spt->pages, &delete_page);
}


/* Returns true if page a precedes page b. */
bool
page_less (const struct hash_elem *a_,
           const struct hash_elem *b_, void *aux UNUSED) {
  const struct page *a = hash_entry (a_, struct page, hash_elem);
  const struct page *b = hash_entry (b_, struct page, hash_elem);

  return a->va < b->va;
}

/* Returns a hash value for page p. */
unsigned
page_hash (const struct hash_elem *p_, void *aux UNUSED) {
  const struct page *p = hash_entry (p_, struct page, hash_elem);
  return hash_bytes (&p->va, sizeof p->va);
}

void
delete_page (const struct hash_elem *a_, void *aux UNUSED) {
  struct page *page = hash_entry(a_, struct page, hash_elem);
  vm_dealloc_page(page);
}

void copy_page (const struct hash_elem *a_, void *aux UNUSED) {
  // * hash_elem으로 부모 page를 찾고 
  struct page *page = hash_entry(a_, struct page, hash_elem);

  // * 부모 page의 정보를 활용하여 initializer 호출
  if (vm_alloc_page_with_initializer(page_get_type(page), page->va, page->writable, NULL, NULL)) {
    // * 자식 테이블에 방금 생성된 페이지를 찾아서 frame 할당과 부모 frame memcopy
    struct page *newpage = spt_find_page(&thread_current()->spt, page->va);
    if (page->frame) {
      newpage->frame = vm_get_frame();
      memcpy(newpage->frame->kva, page->frame->kva, PGSIZE);
      newpage->frame->page = newpage;
      pml4_set_page(thread_current()->pml4, newpage->va, newpage->frame->kva, newpage->writable);
    }
  }
}

void check_valid_buffer (void *buffer, unsigned size, void *esp, bool to_write) {
	uint64_t start_check = buffer;
	uint64_t size_check = size;
	struct thread *cur = thread_current();
	// printf("========pass???========");
	while (size_check >= 0){
		check_address(start_check);
		struct page *page = spt_find_page(&cur->spt,start_check);

		// printf("writable?=%d\n",page->writable);
		if (page==NULL || page->writable != to_write){
			exit(-1);
		}
		size_check -= PGSIZE;
		start_check += PGSIZE;
	} 
	return;
}

void check_valid_string(const void *str, void *esp){
	struct thread *cur = thread_current();

	check_address(str);

	if (!spt_find_page(&cur->spt,str)){
		exit(-1);
	}
	

}
