/* anon.c: Implementation of page for non-disk image (a.k.a. anonymous page). */

#include "vm/vm.h"
#include "devices/disk.h"
#include "bitmap.h"

/* DO NOT MODIFY BELOW LINE */
//흥민이형 받아!!!
static struct disk *swap_disk;
static struct bitmap *swap_slot;
static bool anon_swap_in (struct page *page, void *kva);
static bool anon_swap_out (struct page *page);
static void anon_destroy (struct page *page);

/* DO NOT MODIFY this struct */
static const struct page_operations anon_ops = {
	.swap_in = anon_swap_in,
	.swap_out = anon_swap_out,
	.destroy = anon_destroy,
	.type = VM_ANON,
};

/* Initialize the data for anonymous pages */
void
vm_anon_init (void) {
	/* TODO: Set up the swap_disk. */
	//흥민이형 받아!!!
	swap_disk = disk_get(1,1);
	swap_slot = bitmap_create(512);
}

/* Initialize the file mapping */
bool
anon_initializer (struct page *page, enum vm_type type, void *kva) {
	/* Set up the handler */
	page->operations = &anon_ops;

	struct anon_page *anon_page = &page->anon;
  // return true;
}

/* Swap in the page by read contents from the swap disk. */
//흥민이형!!!
static bool
anon_swap_in (struct page *page, void *kva) {
	struct anon_page *anon_page = &page->anon;
	disk_read(swap_disk, page->slot_idx, kva);
	bitmap_reset(swap_slot, page->slot_idx);
	return true
}

/* Swap out the page by writing contents to the swap disk. */
static bool
anon_swap_out (struct page *page) {
	struct anon_page *anon_page = &page->anon;
	disk_sector_t empty_slot = bitmap_scan(swap_slot,1,512,false);
	if (!empty_slot){
		return false;
	}
	disk_write (swap_disk, empty_slot, page->frame->kva);
	bitmap_mark(swap_slot, empty_slot);
	page->slot_idx = empty_slot;
	return true;
}

/* Destroy the anonymous page. PAGE will be freed by the caller. */
static void
anon_destroy (struct page *page) {
	struct anon_page *anon_page = &page->anon;
}

// disk_sector_t
// get_slot (struct disk *d) {
// 	return bitmap_scan(swap_slot,1,512,false);
// }