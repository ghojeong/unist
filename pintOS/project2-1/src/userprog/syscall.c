#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

#define WORDSIZE 4

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
 if(!pagedir_get_page((uint32_t *)thread_current()->pagedir, f->esp))
   exit(-1);
/*
 else if(!(pagedir_get_page((uint32_t *)thread_current()->pagedir, f->esp +1*WORDSIZE) ||
           pagedir_get_page((uint32_t *)thread_current()->pagedir, f->esp +4*WORDSIZE) ||
           pagedir_get_page((uint32_t *)thread_current()->pagedir, f->esp +5*WORDSIZE)))
   exit(-1);
*/
//first, check the number of syscall
//  printf ("system call!\n");
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
     if(thread_current()->status_ptr->status != -1)
       exit(-1);
     f->eax = (uint32_t)status; 
     exit(status);
     //thread_exit();
  }
  else if(call_num == SYS_EXEC || call_num == 2) {
    
    const char *cmd = *((const char **)((f->esp)+WORDSIZE));
   //need to do synch
   // tid_t ret = (tid_t)exec(cmd);
   // if(ret == TID_ERROR)
   //  f->eax = -1;
   // else
     f->eax = (tid_t)exec(cmd);
  }
  else if(call_num == SYS_WAIT || call_num == 3) {
     
     tid_t pid = *((tid_t *)((f->esp)+WORDSIZE));
     int ret = wait_(pid);
 //    printf("return from wait with value : %d\n", ret);
     f->eax =  (uint32_t)ret;//wait_(pid);
//     printf("f->eax set to : %d\n", f->eax);
  }
  else if(call_num == SYS_CREATE || call_num == 4) {
     if(!pagedir_get_page((uint32_t *)thread_current()->pagedir,
        *((void **) ((f->esp)+4*WORDSIZE))))
        exit(-1);
    const char *name = *((const char **)((f->esp)+4*WORDSIZE));
    int32_t size = *((int32_t *)((f->esp)+5*WORDSIZE));
    if(name != NULL)
    f->eax = (bool)filesys_create(name, size);
    else
     exit(-1);
//      thread_exit();
  }
  else if(call_num == SYS_REMOVE || call_num == 5) {
    thread_exit();
  }
  else if(call_num == SYS_OPEN || call_num == 6) {
     if(!pagedir_get_page((uint32_t *)thread_current()->pagedir, 
        *((void **)((f->esp)+WORDSIZE))))
       exit(-1);
   
    const char *name = *((const char **)((f->esp)+WORDSIZE));
    if(name != NULL)
    f->eax = (int)open(name);
    else f->eax = -1;
//    thread_exit();
  }
  else if(call_num == SYS_FILESIZE || call_num == 7) {
    thread_exit();
  }
  else if(call_num == SYS_READ || call_num == 8) {
    thread_exit();
  }
  else if(call_num == SYS_WRITE || call_num == 9) {
    int fd = *((int *)((f->esp)+5*WORDSIZE));
    const void *buffer = *((const void **)((f->esp)+6*WORDSIZE));
    unsigned size = *((unsigned *)((f->esp)+7*WORDSIZE));
    write(fd, buffer, size);
  }
  else if(call_num == SYS_SEEK || call_num == 10) {
    thread_exit();
  }
  else if(call_num == SYS_TELL || call_num == 11) {
    thread_exit();
  } 
  else if(call_num == SYS_CLOSE || call_num == 12) {
    int fd = *((int *)((f->esp)+WORDSIZE));
    close_sys(fd);
//    thread_exit();
  }
  else  {
//    printf("call number : %d\n", call_num);
   // thread_exit ();
  }
}

//syscalls that need to implement and their call number

void exit(int status) { //call number 1

  struct thread *current_ = thread_current ();
  struct thread *parent_ = thread_current ()->parent_thread;
  lock_acquire(&current_->status_ptr->lock_);
//  if(parent_ != NULL)
   current_->status_ptr->status = status;    
  printf("%s: exit(%d)\n", current_->name, status);
  thread_exit();
//  printf("after thread_exit function\n");
//  if(parent != NULL) {
//    current_->status_ptr->exit =true;
//    currnet_->status_ptr->chilid_thread = NULL;
//    current->parent_thread = NULL;
//  }
}

tid_t exec(const char (*cmd_line)) {//call number 2
//  char *arg;
//  memcpy(arg, cmd_line, sizeof(cmd_line)+1);
  return process_execute (cmd_line);
  //need synchronization??
}

int wait_(tid_t pid) { //call number 3
//    printf("wait_ callend\n");
//    while(1);
//    int ret = process_wait(pid);
//    int i;
//    printf("examining return value..\n");
    //give some loop
//    for(i = 0; i < 1000; i++);
    return process_wait(pid); //simply call process_wait, it will determine
}
/*
bool create(const char *file, unsigned initial_size) { //call number 4
   return filesys_create (file, initial_size);
}

bool remove(const char *file) //call number 5
*/

int open(const char *file_name) { //call number 6
  struct thread *current_ = thread_current();
  struct file_descriptor *fd;
  fd = malloc(sizeof(struct file_descriptor));
  fd->file_ = filesys_open(file_name);
  if(fd->file_ != NULL) {
   fd->fn = current_->file_num++;
   list_push_back(&current_->file_list, &fd->elem);
   return fd->fn;
  }
  else return -1;
}
/*
int filesize(int fd) //call number 7

int read(int fd, void *buffer, unsigned size) //call number 8

int write(int fd, const void *buffer, unsigned size) //call number 9
*/
int write(int fd, const void *buffer, unsigned size) {
   int total_size;
//   printf("fd == %d, size : %d, buffer : %s\n", fd, size, buffer);
   if(fd == 1) {
     char buf[size];
     memcpy(buf, (void *)buffer, size);
//     printf("buf : %s\n", buf);
//     memcpy(buf, buffer, size);
     putbuf(buf, size);
     total_size += size;
   }
   else {
     struct list_elem *e;
     struct thread* current_ = thread_current();
     struct file_descriptor* fd;
     for(e = list_begin(&current_->file_list); e != list_end(&current_->file_list);
         e = list_next(e)) {
        fd = list_entry(e, struct file_descriptor, elem);
        if(fd->fn == fd) break;
         fd = NULL;
     }
     if(fd == NULL) return 0;
     return file_write(fd->file_, buffer, size);
     
     
   }
   return total_size; //just for now
}
/*
void seek(int fd, unsigned position) //call number 10

unsigned tell(int fd) //call number 11
*/
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


