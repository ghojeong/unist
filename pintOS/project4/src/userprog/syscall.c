#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "threads/palloc.h"i

#include "vm/frame.h"
#include "vm/page.h"
#include "userprog/pagedir.h"
#include "filesys/file.h"
//#include <syscall.h>

#define WORDSIZE 4


typedef int mapid_t;
#define MAP_FAILED ((mapid_t) -1)


static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

void exit(int status);
tid_t exec(const char (*cmd_line));
int wait_(tid_t pid);
int open(const char *file_name);
int filesize(int fd);
int read(int fd, void *buffer, unsigned size);
int write(int fd, const void *buffer, unsigned size);
void seek(int fd, unsigned position);
void close_sys(int fd);
unsigned tell_(int fd);

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
 if(!pagedir_get_page((uint32_t *)thread_current()->pagedir, f->esp))
   exit(-1);
  int call_num = *((int *)f->esp);
//  printf("system call is called with %d\n", call_num);
//for number of argument,
// 1 : esp+1
// 2 : esp+4, esp+5
// 3 : esp+5, esp+6, esp+7
//
  if(call_num == SYS_HALT || call_num == 0) //halt
     shutdown_power_off();
  else if(call_num == SYS_EXIT || call_num == 1) {
     if(!is_user_vaddr(f->esp+WORDSIZE))
      exit(-1);
     int status = *((int *)((f->esp)+WORDSIZE));
     if(thread_current()->status_ptr != NULL &&
        thread_current()->status_ptr->status != -1)
       exit(-1);
     f->eax = (uint32_t)status; 
     exit(status);
  }
  else if(call_num == SYS_EXEC || call_num == 2) {
    if(!is_user_vaddr(*((void **)((f->esp)+WORDSIZE))) || 
       (void *)((f->esp)+WORDSIZE) < (void *)0x08084000 )
        exit(-1);
    if(!pagedir_get_page((uint32_t *)thread_current()->pagedir,
       *((void **)((f->esp)+WORDSIZE))))
        exit(-1);
    char *cmd = malloc(strlen(*((const char **)((f->esp)+WORDSIZE)))+1);
     strlcpy(cmd, *((const char **)((f->esp)+WORDSIZE)), 
             strlen(*((const char **)((f->esp)+WORDSIZE)))+1);
     open(cmd);
     f->eax = (tid_t)exec(cmd);
     free(cmd);
  }
  else if(call_num == SYS_WAIT || call_num == 3) {
     
     tid_t pid = *((tid_t *)((f->esp)+WORDSIZE));
     int ret = wait_(pid);
     f->eax =  (uint32_t)ret;
  }
  else if(call_num == SYS_CREATE || call_num == 4) {
     if(!pagedir_get_page((uint32_t *)thread_current()->pagedir,
        *((void **) ((f->esp)+4*WORDSIZE))))
    if(!is_user_vaddr(*((void **)((f->esp)+4*WORDSIZE))))


     if(!is_user_vaddr(*((void **)((f->esp)+4*WORDSIZE))) ||  
       (void *)((f->esp)+4*WORDSIZE) < (void *)0x08084000 )
        exit(-1);
    if(!pagedir_get_page((uint32_t *)thread_current()->pagedir,
       *((void **)((f->esp)+4*WORDSIZE))))
        exit(-1);
    const char *name = *((const char **)((f->esp)+4*WORDSIZE));
    int32_t size = *((int32_t *)((f->esp)+5*WORDSIZE));
    if(name != NULL)
    f->eax = (bool)filesys_create(name, size);
    else
     exit(-1);
  }
  else if(call_num == SYS_REMOVE || call_num == 5) {
    if(!is_user_vaddr(*((void **)((f->esp)+WORDSIZE))) ||  
       (void *)((f->esp)+WORDSIZE) < (void *)0x08084000 )
        exit(-1);
    if(!pagedir_get_page((uint32_t *)thread_current()->pagedir,
       *((void **)((f->esp)+WORDSIZE))))
        exit(-1);

    const char *name = *((const char **)((f->esp)+WORDSIZE));
    if(name != NULL)
      f->eax = filesys_remove(name);
    else f->eax = false;

  }
  else if(call_num == SYS_OPEN || call_num == 6) {
      if(!is_user_vaddr(*((void **)((f->esp)+WORDSIZE))) ||  
       (void *)((f->esp)+WORDSIZE) < (void *)0x08084000 )
       exit(-1);
    if(!pagedir_get_page((uint32_t *)thread_current()->pagedir,
       *((void **)((f->esp)+WORDSIZE))))
        exit(-1);

   
    const char *name = *((const char **)((f->esp)+WORDSIZE));
    if(name != NULL) {
    f->eax = (int)open(name);
 
    }
    else f->eax = -1;
  }
  else if(call_num == SYS_FILESIZE || call_num == 7) {
    int fd = *((int *)((f->esp)+WORDSIZE));
    f->eax = filesize(fd);
  }
  else if(call_num == SYS_READ || call_num == 8) {
    if(!is_user_vaddr(*((void **)((f->esp)+6*WORDSIZE))) ||
       (void *)((f->esp)+6*WORDSIZE) < (void *)0x08084000)
       exit(-1);     
//    if(f->esp == NULL || (f->esp)+6*WORDSIZE == NULL) 
//       exit(-1);
    int fd = *((int *)((f->esp)+5*WORDSIZE));
    void *buffer = *((void **)((f->esp)+6*WORDSIZE));
   
//    if(buffer < f->esp) exit(-1);

    unsigned size = *((unsigned *)((f->esp)+7*WORDSIZE));
    if(buffer == NULL) exit(-1);
    f->eax = read(fd, buffer, size);
  }
  else if(call_num == SYS_WRITE || call_num == 9) {
      if(!is_user_vaddr(*((void **)((f->esp)+6*WORDSIZE))) ||  
       (void *)((f->esp)+6*WORDSIZE) < (void *)0x08084000 )
       exit(-1);
    if(!pagedir_get_page((uint32_t *)thread_current()->pagedir,
       *((void **)((f->esp)+6*WORDSIZE))))
        exit(-1);

    int fd = *((int *)((f->esp)+5*WORDSIZE));
    const void *buffer = *((const void **)((f->esp)+6*WORDSIZE));
    unsigned size = *((unsigned *)((f->esp)+7*WORDSIZE));
    f->eax = write(fd, buffer, size);
  }
  else if(call_num == SYS_SEEK || call_num == 10) {
    int fd = *((int *)((f->esp)+4*WORDSIZE));
    unsigned pos = *((unsigned *)((f->esp)+5*WORDSIZE));
    seek(fd, pos);
  }
  else if(call_num == SYS_TELL || call_num == 11) {
    int fd = *((int *)((f->esp)+WORDSIZE)); 
    tell_(fd);
  } 
  else if(call_num == SYS_CLOSE || call_num == 12) {
    int fd = *((int *)((f->esp)+WORDSIZE));
    close_sys(fd);
  }
  else if(call_num == SYS_MMAP || call_num == 13) {
    int fd = *((int *)((f->esp)+4*WORDSIZE));
    void *addr = *((void **)((f->esp)+5*WORDSIZE));
    f->eax = mmap(fd, addr);
  }
  else if(call_num == SYS_MUNMAP || call_num == 14) {
    int map_id = *((int *)((f->esp)+WORDSIZE));
    munmap(map_id);
  }
}

//syscalls that need to implement and their call number

void exit(int status) { //call number 1

  struct thread *current_ = thread_current ();
  if(current_->status_ptr != NULL)
   current_->status_ptr->status = status;

  if(!list_empty(&current_->file_map_list)) {
     while(!list_empty(&current_->file_map_list)) {
       struct file_map *mapped_file = list_entry(list_front(&current_->file_map_list), 
                                                  struct file_map, elem);
       munmap(mapped_file->map_id);
     }
  }
//  sema_get();
  remove_page_elem_all(current_);
//  sema_release();

  printf("%s: exit(%d)\n", current_->name, status);
   thread_exit();
}

tid_t exec(const char (*cmd_line)) {//call number 2
  return process_execute (cmd_line);
}

int wait_(tid_t pid) { //call number 3
    return process_wait(pid); //simply call process_wait, it will determine
}

int open(const char *file_name) { //call number 6
  struct thread *current_ = thread_current();

  struct file_descriptor *fd;
  fd = malloc(sizeof(struct file_descriptor));
  fd->file_ = filesys_open(file_name);
  if(fd->file_ != NULL) {
   fd->fn = current_->file_num++;
   fd->file_name = file_name;
   list_push_back(&current_->file_list, &fd->elem);
   
   return fd->fn;
  }
  else {
   free(fd);
   return -1;
  }

}

int filesize(int fd) { //call number 7
  struct thread *current_ = thread_current();
  struct list_elem *e;
  struct file_descriptor *file_desc;
  for(e = list_begin(&current_->file_list); e != list_end(&current_->file_list);
      e = list_next(e)) {
      file_desc = list_entry(e, struct file_descriptor, elem);
      if(file_desc->fn == fd) break;
      file_desc = NULL;
  }
  if(file_desc == NULL) exit(-1);
  return file_length(file_desc->file_);
}

int read(int fd, void *buffer, unsigned size) {//call number 8
  struct thread *current_ = thread_current();
  struct list_elem *e;
  struct file_descriptor *file_desc;
//  int32_t size;
  if(buffer == NULL) exit(-1);
  if(fd == 0) 
    return input_getc();

  for(e = list_begin(&current_->file_list); e != list_end(&current_->file_list);
      e = list_next(e)) {
     file_desc = list_entry(e, struct file_descriptor, elem);
     if(file_desc->fn == fd) break;
     file_desc = NULL;
  }
  if(file_desc == NULL) exit(-1);
  return file_read(file_desc->file_, buffer, size);
}

int write(int fd, const void *buffer, unsigned size) {
   int total_size;
   if(fd == 1) {
     char buf[size];
     memcpy(buf, (void *)buffer, size);
     putbuf(buf, size);
     total_size += size;
   }
   else {
     if(buffer == NULL) exit(-1);
     struct list_elem *e;
     struct thread* current_ = thread_current();
     struct file_descriptor* file_desc;

     for(e = list_begin(&current_->file_list); e != list_end(&current_->file_list);
         e = list_next(e)) {
        file_desc = list_entry(e, struct file_descriptor, elem);
        if(file_desc->fn == fd) break;
         file_desc = NULL;
     }
     if(file_desc == NULL) return 0;//exit(-1); //or return 0??
     
     struct exec_file *ex_;
     for(e = list_begin(&exec_file_list); e != list_end(&exec_file_list); e = list_next(e)) {
         ex_ = list_entry(e, struct exec_file, elem);
         if(strcmp(ex_->name, file_desc->file_name) == 0) { 
           break;
         }
         ex_ = NULL;
     }
     if(ex_ != NULL) return 0;

     total_size =  file_write(file_desc->file_, buffer, size);
//     printf("total size : %d\n", total_size);
   }
   return total_size; //just for now
}

void seek(int fd, unsigned position) { //call number 10
   struct list_elem *e;
   struct thread* current_ = thread_current();
   struct file_descriptor* file_desc;
   for(e = list_begin(&current_->file_list); e != list_end(&current_->file_list);
         e = list_next(e)) {
        file_desc = list_entry(e, struct file_descriptor, elem);
        if(file_desc->fn == fd) break;
         file_desc = NULL;
     }
     if(file_desc == NULL) exit(-1);
     file_seek(file_desc->file_, position);

}

unsigned tell_(int fd) { //call number 11
   unsigned ret;
   struct list_elem *e;
   struct thread* current_ = thread_current();
   struct file_descriptor* file_desc;
   for(e = list_begin(&current_->file_list); e != list_end(&current_->file_list);
         e = list_next(e)) {
        fd = list_entry(e, struct file_descriptor, elem);
        if(file_desc->fn == fd) break;
         file_desc = NULL;
     }
     if(file_desc == NULL) exit(-1);
     ret = file_tell(file_desc->file_) + 1;
     return ret;
}

void close_sys(int fd) { //call number 12
  struct list_elem *e;
  struct file_descriptor* desc_ = NULL;
  struct thread* current_ = thread_current();

  for(e = list_begin(&current_->file_list); e != list_end(&current_->file_list);
      e = list_next(e)) {
     desc_ = list_entry(e, struct file_descriptor, elem);
     if(desc_->fn == fd) break;
  }
  if(desc_ == NULL) return;
  file_close(desc_->file_);
  list_remove(&desc_->elem);
  free(desc_);
}

mapid_t mmap(int fd, void *addr) {
  if(addr == NULL) return MAP_FAILED;
  if(pg_ofs(addr) != 0) return MAP_FAILED;
  if(fd == 0 || fd == 1) return MAP_FAILED;
   struct thread *current = thread_current();
   struct list_elem *e;
   struct file_descriptor *desc;
   struct file_map *map_elem;

//   sema_get();

   for(e = list_begin(&current->file_list); e != list_end(&current->file_list);
       e = list_next(e)) {
      desc = list_entry(e, struct file_descriptor, elem);
      if(desc->fn == fd) break;
      desc = NULL;
   }
   if(desc == NULL) { /*sema_release();*/  return MAP_FAILED; }

   for(e = list_begin(&current->file_map_list); e != list_end(&current->file_map_list);
       e = list_next(e)) {
      map_elem = list_entry(e, struct file_map, elem);

      if((map_elem->vaddr < addr) && ((map_elem->vaddr[map_elem->offset-1]/* + map_elem->offset*PGSIZE*/) > addr)) 
        {/* sema_release();*/ return MAP_FAILED; }
   }

   int size = filesize(fd);
   int num_page = size / PGSIZE + (size % PGSIZE == 0? 0 : 1);

   int i;
   for(i = 0; i < num_page; i++) {
     uint32_t *pde = thread_current()->pagedir + pd_no(addr + i*PGSIZE);
    

     if(*pde != 0) {
//       sema_release();
       return MAP_FAILED;
     }
   }

   if(size == 0) {/* sema_release(); */return MAP_FAILED; }
   map_elem = malloc(sizeof(struct file_map));
   map_elem->map_id = ++current->map_count;
   map_elem->fn = fd;
   map_elem->offset = num_page;
   map_elem->file_size = size;
   map_elem->file_ = file_reopen(desc->file_);
   map_elem->vaddr = malloc(num_page * sizeof(uint32_t *));
   map_elem->page = malloc(num_page * sizeof(uint32_t *));
   for(i = 0; i < num_page; i++)
      map_elem->vaddr[i] = addr + i*PGSIZE;
   for(i = 0; i < num_page; i++)
      map_elem->page[i] = palloc_get_page(PAL_USER | PAL_ZERO);
   for(i = 0; i < num_page; i++) {
       file_read_at(map_elem->file_, map_elem->page[i]/*map_elem->page + i*PGSIZE*/, PGSIZE, i*PGSIZE);
       pagedir_set_accessed(current->pagedir, map_elem->vaddr[i]/*addr + i*PGSIZE*/, true);
       pagedir_set_page(current->pagedir, map_elem->vaddr[i]/*addr + i*PGSIZE*/, map_elem->page[i]/*kpage + i*PGSIZE*/, true, map_elem->file_);

       add_page_entry(map_elem->vaddr[i]/*addr + i*PGSIZE*/, map_elem->page[i]/*kpage + i*PGSIZE*/, map_elem->file_, 
                      thread_current(), true);
       frame_insert(thread_current()->name, map_elem->page[i]/*kpage + i*PGSIZE*/, map_elem->vaddr[i]/*addr + i*PGSIZE*/);
        
   }
   list_push_back(&current->file_map_list, &map_elem->elem) ;
   return map_elem->map_id;

}

void munmap(int map_id) {
   struct list_elem *e;
   struct thread *current = thread_current();
   struct file_map * mapped_file;
   for(e = list_begin(&current->file_map_list); e != list_end(&current->file_map_list);
       e = list_next(e)) {
       mapped_file = list_entry(e, struct file_map, elem);
       if(mapped_file->map_id == map_id) {
         break;
      }
      mapped_file = NULL;
   }
   if(mapped_file == NULL) {exit(-1);}

   //check exist first
   int i;

   for(i = 0; i < mapped_file->offset; i++) {
 ASSERT(pagedir_get_page(current->pagedir, mapped_file->vaddr[i]) == mapped_file->page[i]);
      if(pagedir_is_dirty(current->pagedir, mapped_file->vaddr[i])) {
         file_write_at(mapped_file->file_, (const void *)(mapped_file->page[i]), PGSIZE,
                       i*PGSIZE);
      }
   }
  for(i = 0; i < mapped_file->offset; i++) {
       pagedir_clear_page(current->pagedir, mapped_file->vaddr[i]);
       palloc_free_page(mapped_file->page[i]);
  }

   list_remove(&mapped_file->elem);
   file_close(mapped_file->file_);
   free(mapped_file);
}
