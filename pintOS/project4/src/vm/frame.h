#ifndef FRAME_H
#define FRAME_H
//#include line
//

//#define frame_offset 1 << 11
#include <list.h>
#include "threads/synch.h"
#include "threads/thread.h"

struct list frame_table;
struct semaphore frame_sema;


struct frame_elem {
   char *thread_name;
   uint32_t key;
   uint32_t *page_ptr;
   struct list_elem elem;
   bool mapped;
};

void init_frame_table();
bool frame_insert(char *name, uint32_t *kpage, uint32_t *vaddr);
//insert's vaddr will be pt, not pd
void delete_entry(char *name, uint32_t *kpage);
bool check_exist(uint32_t key);
uint32_t *get_page_addr(char *name, uint32_t *kpage);

void frame_sema_get();
void frame_sema_release();

uint32_t *evict(struct frame_elem *elem_);
#endif
