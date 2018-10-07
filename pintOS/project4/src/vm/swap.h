//header files
//
#include "devices/block.h"
#include "threads/thread.h"
#include <list.h>
#include "threads/synch.h"
#include "vm/frame.h"
#include "vm/page.h"
#include "userprog/pagedir.h"
#include <bitmap.h>

struct swap_slot {
  struct bitmap *swap_map;
  struct block *block_loc;
  struct list swap_list;
  uint32_t sector_size;
  int size_count;
};

struct swap_slot *swap_table;

struct swap_list_elem {
  struct thread *thread_;
  uint32_t *vaddr;
  uint32_t *paddr;
  uint32_t offset;

  struct list_elem elem;
};

struct semaphore swap_sema_g;

void init_swap_table();
void swap_out();
bool swap_in(struct thread *thread_, uint32_t *vaddr);

bool check_swap_table(struct thread *thread_, uint32_t *vaddr, uint32_t *paddr);

uint32_t search_inside(struct thread *thread_, uint32_t *vaddr);

uint32_t *invalid_check(struct thread *thread_);

void get_swap_sema();
void release_swap_sema();
