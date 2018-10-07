#include "vm/swap.h"
#include <round.h>

//block sector size = 512, page size = 4096, so 8 sector is needed to save one page

void get_swap_sema() {
   sema_down(&swap_sema_g);
}

void release_swap_sema() {
   sema_up(&swap_sema_g);
}

void init_swap_table() {
   swap_table = malloc(sizeof(struct swap_slot));
   sema_init(&swap_sema_g, 1);
   list_init(&swap_table->swap_list);
   swap_table->block_loc = block_get_role(BLOCK_SWAP);
   if(swap_table->block_loc != NULL)
    swap_table->sector_size = block_size(swap_table->block_loc)*512;
   else
    swap_table->sector_size = 0;
   swap_table->size_count = swap_table->sector_size/PGSIZE;
   swap_table->swap_map = bitmap_create(swap_table->size_count);
   ASSERT(swap_table->size_count == bitmap_size(swap_table->swap_map));
   ASSERT(bitmap_count(swap_table->swap_map, 0, swap_table->size_count, false)
           == swap_table->size_count);
}


void swap_out() {
get_swap_sema();
   ASSERT(bitmap_count(swap_table->swap_map, 0, swap_table->size_count, true) 
          != swap_table->size_count) //there is a free slot at the table
   struct list_elem *e;
   struct page_list_elem *p_elem = NULL;
sema_get();
   for(e = list_begin(&page_list); e != list_end(&page_list); e = list_next(e)) {
      p_elem = list_entry(e, struct page_list_elem, elem);
      if(p_elem->valid == true && p_elem->mapped == false) 
         break;
      p_elem = NULL;
   }
   ASSERT(p_elem != NULL);
   ASSERT(get_page_addr(p_elem->thread_->name, p_elem->kpage) == p_elem->vpage);
   ASSERT(check_swap_table(p_elem->thread_, p_elem->vpage, p_elem->kpage) != true);
  int offset = bitmap_scan(swap_table->swap_map, 0, 1, false);
  int block_offset = 8 * offset;
  void *buffer = malloc(PGSIZE); //buffer to save the page information
  memcpy(buffer, (const void *)p_elem->kpage, PGSIZE);
  ASSERT(memcmp((const void *)buffer, (const void *)p_elem->kpage, PGSIZE) == 0);

  int i;
  for(i = 0; i < 8; i++) 
    block_write(swap_table->block_loc, block_offset + i, (const void *)(buffer + i*512));

  bitmap_flip(swap_table->swap_map, offset);
  ASSERT(bitmap_test(swap_table->swap_map, offset) == true); 
  p_elem->valid = false;
  ASSERT(pagedir_get_page(p_elem->thread_->pagedir, p_elem->vpage) != NULL);
  pagedir_clear_page(p_elem->thread_->pagedir, p_elem->vpage);
  palloc_free_page(p_elem->kpage);
  ASSERT(pagedir_get_page(p_elem->thread_->pagedir, p_elem->vpage) == NULL);

  struct swap_list_elem *s_elem = malloc(sizeof(struct swap_list_elem));
  s_elem->thread_ = p_elem->thread_;
  s_elem->vaddr = p_elem->vpage;
  s_elem->paddr = p_elem->kpage;
  s_elem->offset = offset;

  list_push_back(&swap_table->swap_list, &s_elem->elem);
sema_release();
release_swap_sema();
//  return p_elem->kpage;
}


bool swap_in(struct thread *thread_, uint32_t *vaddr) {
  struct list_elem *e;
  struct page_list_elem *p_elem;
  struct swap_list_elem *s_elem;
get_swap_sema();

  uint32_t *kpage = palloc_get_page(PAL_USER);
//  uint32_t *kpage = pagedir_get_page(thread_->pagedir, vaddr);
  if(kpage == NULL) { 
    release_swap_sema();
//    printf("kpage is NULL\n");
    return false;
  }

sema_get();
  for(e = list_begin(&page_list); e != list_end(&page_list); e = list_next(e)) {
     p_elem = list_entry(e, struct page_list_elem, elem);
     if(p_elem->thread_ == thread_ && p_elem->vpage == vaddr && p_elem->valid == false)
      break;
     p_elem = NULL;
  }
  ASSERT(p_elem != NULL);

  for(e = list_begin(&swap_table->swap_list); e != list_end(&swap_table->swap_list);
      e = list_next(e)) {
     s_elem = list_entry(e, struct swap_list_elem, elem);
     if(s_elem->thread_ == thread_ && s_elem->vaddr == vaddr) break;
     s_elem = NULL;
  }
  ASSERT(s_elem != NULL);
  ASSERT(p_elem->kpage == s_elem->paddr);
  void *buffer = malloc(PGSIZE);
  int i;
  for(i = 0; i < 8; i++)
   block_read(swap_table->block_loc, (8*s_elem->offset) + i, buffer + i*512);
  ASSERT(kpage != NULL);
  memcpy(kpage, buffer, PGSIZE); //copy the data into page
  bool check = pagedir_set_page(thread_->pagedir, vaddr, kpage, true, NULL);
  ASSERT(check == true);
  p_elem->valid = true;
  p_elem->kpage = kpage;
  list_remove(&p_elem->elem);
  list_push_back(&page_list, &p_elem->elem);
printf("push back element\n");
sema_release();

frame_sema_get();
  frame_insert(thread_->name, kpage, vaddr);
frame_sema_release();
printf("puishing finished\n");
  list_remove(&s_elem->elem);
  ASSERT(bitmap_test(swap_table->swap_map, s_elem->offset) == true);
  bitmap_flip(swap_table->swap_map, s_elem->offset);
  free(s_elem);
release_swap_sema();
  printf("returning\n");
  return true;
}



bool check_swap_table(struct thread *thread_, uint32_t *vaddr, uint32_t *paddr) {
   struct list_elem *e;
   struct swap_list_elem *elem_;
get_swap_sema();
   for(e = list_begin(&swap_table->swap_list); e != list_end(&swap_table->swap_list); 
       e = list_next(e)) {
      elem_ = list_entry(e, struct swap_list_elem, elem);
      if(elem_->thread_ == thread_ && elem_->vaddr == vaddr && elem_->paddr == paddr) {
         release_swap_sema();
         return true;
      }
   }
release_swap_sema();
   return false;
}

uint32_t search_inside(struct thread *thread_, uint32_t *vaddr) {
   struct list_elem *e;
   struct page_list_elem *p_elem;
   struct swap_list_elem *s_elem;
get_swap_sema();
sema_get();
   for(e = list_begin(&page_list); e != list_end(&page_list); e = list_next(e)) {
       p_elem = list_entry(e, struct page_list_elem, elem);
       if(p_elem->thread_ == thread_ && p_elem->vpage == vaddr && p_elem->valid == false)
         break;
      p_elem = NULL;
   }
   if(p_elem == NULL) {
     sema_release();
     release_swap_sema();
     return -1;
   }
   for(e = list_begin(&swap_table->swap_list); e != list_end(&swap_table->swap_list);
       e = list_next(e)) {
       s_elem = list_entry(e, struct swap_list_elem, elem);
       if(s_elem->thread_ == p_elem->thread_ && s_elem->vaddr == p_elem->vpage &&
           s_elem->paddr == p_elem->kpage) {
           sema_release();
           release_swap_sema();
           return s_elem->offset;
       }
       s_elem = NULL;
   }
sema_release();
release_swap_sema();
   return -1;
}

uint32_t *invalid_check(struct thread *thread_) {
  struct list_elem *e;
  struct page_list_elem *p_elem;
sema_get();
  for(e = list_begin(&page_list); e != list_end(&page_list); e = list_next(e)) {
      p_elem = list_entry(e, struct page_list_elem, elem);
      if(p_elem->thread_ == thread_ && p_elem->valid == false) {
         sema_release();
         return p_elem->vpage;
      }
      p_elem = NULL;
  }
sema_release();
  return NULL;
}
