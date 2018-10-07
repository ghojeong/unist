#ifndef PAGE_H
#define PAGE_H

#include "filesys/file.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "threads/palloc.h"
#include "vm/frame.h"
#include "userprog/pagedir.h"
#include "threads/pte.h"
#include "threads/vaddr.h"
//#include statements
//
//
//

struct list page_list;

struct semaphore g_sema;

struct page_list_elem {
  uint32_t *vpage;
  uint32_t *kpage;
  struct file *file_;
  struct thread *thread_;
  struct list_elem elem;
//  struct semaphore sema_;
  bool valid;
  bool mapped;
  //any other thing??
};

void page_list_init(void);
void sema_get();
void sema_release();
bool alloc_page_entry(uint32_t *pd, uint32_t *vaddr, uint32_t *kpage, struct file *file_, 
                      enum palloc_flags, bool writable); 
//will be used to allocate new entry
void add_page_entry(uint32_t *vaddr, uint32_t *kpage, struct file *file_, struct thread *thread_,
                    bool mapped); //will be used to add
//void set_page(uint32_t *vaddr, uint32_t *kpage)
//void page_list_init();
uint32_t get_estimated_vaddr(uint32_t *vaddr); //return vaddr without offset
uint32_t get_estimated_paddr(uint32_t *pd, uint32_t *vaddr); //return paddr without offset

uint32_t get_page_key(uint32_t *pd, uint32_t *vaddr);
uint32_t *search_exist(uint32_t *key);
struct file *get_file(uint32_t *key);

void remove_page_elem_all(struct thread *thread_);
void remove_page_elem(uint32_t *vaddr, struct thread *thread_);

struct page_list_elem* check_allocated_page(uint32_t *vaddr);
#endif
