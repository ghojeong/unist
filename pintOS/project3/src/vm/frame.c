#include "vm/frame.h"i
#include <stdio.h>
//#include <list.h>

void init_frame_table() {
   list_init(&frame_table);
   sema_init(&frame_sema, 1);
}

void frame_sema_get() {
   sema_down(&frame_sema);
}

void frame_sema_release() {
   sema_up(&frame_sema);
}

bool frame_insert(char * name, uint32_t *kpage, uint32_t *vaddr) {
   uint32_t key = (uint32_t)kpage; //*kpage holds actual frame addr
   struct list_elem *e;
   struct frame_elem* f_elem;
frame_sema_get();
   for(e = list_begin(&frame_table); e != list_end(&frame_table); e = list_next(e)) {
     f_elem = list_entry(e, struct frame_elem, elem);
     if(f_elem->key == (uint32_t)kpage) {
        f_elem->page_ptr = vaddr;
frame_sema_release();
        return true;
     }
   }

   struct frame_elem *entry = malloc(sizeof(struct frame_elem));
   entry->key = key;
   entry->page_ptr = vaddr;
   entry->thread_name = name;
   list_push_back(&frame_table, &entry->elem);
frame_sema_release();
   return true; 
}

void delete_entry(char *name, uint32_t *kpage) {
   struct list_elem *e;
   struct frame_elem* entry;
   uint32_t key = (uint32_t)kpage;
   if(list_empty(&frame_table)) {
     return;
   }
frame_sema_get();
   for(e = list_begin(&frame_table); e != list_end(&frame_table); e = list_next(e)) {
      entry = list_entry(e, struct frame_elem, elem);
      if(entry->key == key && (strcmp(name, entry->thread_name) == 0)) {
        list_remove(&entry->elem); //delete should do anything else?
frame_sema_release();
        return;
      }
   }
frame_sema_release();
}

bool check_exist(uint32_t key) {
    struct list_elem *e;
    struct frame_elem* entry;
    if(list_empty(&frame_table)) {
      return false;
    }
frame_sema_get();
    for(e = list_begin(&frame_table); e != list_end(&frame_table); e = list_next(e)) {
       entry = list_entry(e, struct frame_elem, elem);
       if(entry->key == key) {
frame_sema_release();
         return true;
       }
    }
frame_sema_release();
   return false;
}

uint32_t *get_page_addr(char *name, uint32_t *kpage) {
   struct list_elem *e;
   struct frame_elem *entry;
   uint32_t key = (uint32_t)kpage;
   if(list_empty(&frame_table)) {
     return NULL;
   }
frame_sema_get();
   for(e = list_begin(&frame_table); e != list_end(&frame_table); e = list_next(e)) {
      entry = list_entry(e, struct frame_elem, elem);
      if(entry->key == key && (strcmp(name, entry->thread_name)==0)) {
        frame_sema_release();
        return entry->page_ptr;
      }
   }
frame_sema_release();
   return NULL;
}
