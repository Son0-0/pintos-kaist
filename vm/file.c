/* file.c: Implementation of memory backed file object (mmaped object). */

#include "vm/vm.h"

// * VM 추가
#include "threads/vaddr.h"
#include "userprog/process.h"

static bool file_backed_swap_in (struct page *page, void *kva);
static bool file_backed_swap_out (struct page *page);
static void file_backed_destroy (struct page *page);
static bool lazy_load_segment (struct page *page, struct dummy *aux);

/* DO NOT MODIFY this struct */
static const struct page_operations file_ops = {
	.swap_in = file_backed_swap_in,
	.swap_out = file_backed_swap_out,
	.destroy = file_backed_destroy,
	.type = VM_FILE,
};

/* The initializer of file vm */
void
vm_file_init (void) {
}

/* Initialize the file backed page */
bool
file_backed_initializer (struct page *page, enum vm_type type, void *kva) {
	/* Set up the handler */
	page->operations = &file_ops;

	struct file_page *file_page = &page->file;
}

/* Swap in the page by read contents from the file. */
static bool
file_backed_swap_in (struct page *page, void *kva) {
	struct file_page *file_page UNUSED = &page->file;
}

/* Swap out the page by writeback contents to the file. */
static bool
file_backed_swap_out (struct page *page) {
	struct file_page *file_page UNUSED = &page->file;
}

/* Destory the file backed page. PAGE will be freed by the caller. */
static void
file_backed_destroy (struct page *page) {
	struct file_page *file_page UNUSED = &page->file;
}

/* Do the mmap */
void *
do_mmap (void *addr, size_t length, int writable, struct file *file, off_t offset) {
  
  uint64_t va = addr;
  file = file_reopen(file);

  uint64_t file_size= file_length(file);

  while (0 < file_size) {
    struct dummy *aux = (struct dummy*)malloc(sizeof(struct dummy));
    aux->file = file;
    aux->read_bytes = file_size < PGSIZE ? file_size : PGSIZE;
    aux->zero_bytes = PGSIZE - aux->read_bytes;
    aux->ofs = offset;

    if (!vm_alloc_page_with_initializer(VM_FILE, va, writable, lazy_load_segment, aux)) {
      return NULL;
    }

    offset += aux->read_bytes;
    file_size -= aux->read_bytes;
    va += PGSIZE;
  }
  return addr;
}

/* Do the munmap */
void
do_munmap (void *addr) {
  struct page *page = spt_find_page(&thread_current()->spt, addr);
  struct file *file = thread_current()->fdt[page->fd];
  size_t size = page->file_size;

  file = file_reopen(file);
  file_seek(file, page->file_ofs);
  uint64_t write_bytes;

  while (0 < size) {
    struct page *cur_page = spt_find_page(&thread_current()->spt, addr);
    if (page && page->frame) {
      write_bytes = size < PGSIZE ? size : PGSIZE;
      if (size != file_write(file, page->frame->kva, write_bytes)) {
        exit(-1);
      }
    }
    size -= write_bytes;
    addr += PGSIZE;
    destroy(page);
  }

}

static bool
lazy_load_segment (struct page *page, struct dummy *aux) {
	
	/* TODO: Load the segment from the file */
	/* TODO: This called when the first page fault occurs on address VA. */
	/* TODO: VA is available when calling this function. */
	if (file_read_at(aux->file, page->frame->kva, aux->read_bytes, aux->ofs) != (int) aux->read_bytes) {
    palloc_free_page (page->frame->kva);
    free(aux);
		return false;
	}
	memset (page->frame->kva + aux->read_bytes, 0, aux->zero_bytes);
  free(aux);
  return true;
}