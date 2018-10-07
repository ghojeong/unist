#include "vm/page.h"
#include "userprog/pagedir.h"
#include "vm/frame.h"

void page_list_init(void) {
  list_init(&page_list);
  sema_init(&g_sema, 1);
}

void sema_get() {
  sema_down(&g_sema);
}

void sema_release() {
  sema_up(&g_sema);
}

void add_page_entry(uint32_t *vaddr, uint32_t *kpage, struct file *file_, struct thread *thread_,
                    bool mapped) {
sema_get();
  struct page_list_elem *elem_ = malloc(sizeof(struct page_list_elem));
  elem_->kpage = kpage;
  elem_->vpage = vaddr;
  elem_->file_ = file_;
  elem_->valid = true;
  elem_->mapped = mapped;
  elem_->thread_ = thread_;
  list_push_back(&page_list, &elem_->elem);
sema_release();
}

void remove_page_elem_all(struct thread *thread_) {
sema_get();
  struct list_elem *e;
  struct page_list_elem *pe;
  for(e = list_begin(&page_list); e != list_end(&page_list);) {
     pe = list_entry(e, struct page_list_elem, elem);
     if(pe->thread_ == thread_) {
       e = list_next(e);
       list_remove(&pe->elem);
       free(pe);
     }
     else {
       e = list_next(e);
     }
  }
sema_release();
}

void remove_page_elem(uint32_t *vaddr, struct thread *thread_) {
  struct list_elem *e;
  struct page_list_elem *pe;
sema_get();
  for(e = list_begin(&page_list); e != list_end(&page_list); e = list_next(e)) {
     pe = list_entry(e, struct page_list_elem, elem);
     if(pe->thread_ == thread_ && pe->vpage == vaddr) {
       list_remove(&pe->elem);
       free(pe);
sema_release();
       return;
     }
  }
sema_release();
}

uint32_t* search_exist(uint32_t *key) {
sema_get();
  struct list_elem *e;
  struct page_list_elem *pe;
  for(e = list_begin(&page_list); e != list_end(&page_list); e = list_next(e)) {
    pe = list_entry(e, struct page_list_elem, elem);
    if(pe->kpage == key) {
sema_release();
      return pe->kpage;
    }
    pe = NULL;
  }
sema_release();
  return NULL;
}

uint32_t get_estimated_vaddr(uint32_t *vaddr) { 
   return (uint32_t)vaddr & PTE_ADDR;
}

//we will estimate the paddr to get the data from file
uint32_t get_estimated_paddr(uint32_t *pd, uint32_t *vaddr) {
  uint32_t *pt, *pde, *pte;
  pde = pd + pd_no(vaddr);
  if(*pde == 0) return 0; //means no matching pde entry, thus can't estimate
  pt = pde_get_pt(*pde);
  pte = &pt[pt_no (vaddr)];
  if(pte == NULL) return 0; //means no matching pte entry, can't estimate
  return (uint32_t)(pte_get_page(*pte) + pg_ofs (vaddr));
}

struct page_list_elem* check_allocated_page(uint32_t *vaddr) {
   struct list_elem *e;
   struct page_list_elem *elem_;
sema_get();
   for(e = list_begin(&page_list); e != list_end(&page_list); e = list_next(e)) {
      elem_ = list_entry(e, struct page_list_elem, elem);
      if(elem_->vpage == vaddr) {
sema_release();
         return elem_;
      }
      elem_ = NULL;
   }
sema_release();
  return NULL; 
}
