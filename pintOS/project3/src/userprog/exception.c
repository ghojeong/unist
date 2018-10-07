#include "userprog/exception.h"
#include <inttypes.h>
#include <stdio.h>
#include "userprog/gdt.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

#include "vm/page.h"
#include "vm/swap.h"

/* Number of page faults processed. */
static long long page_fault_cnt;

static void kill (struct intr_frame *);
static void page_fault (struct intr_frame *);

/* Registers handlers for interrupts that can be caused by user
   programs.

   In a real Unix-like OS, most of these interrupts would be
   passed along to the user process in the form of signals, as
   described in [SV-386] 3-24 and 3-25, but we don't implement
   signals.  Instead, we'll make them simply kill the user
   process.

   Page faults are an exception.  Here they are treated the same
   way as other exceptions, but this will need to change to
   implement virtual memory.

   Refer to [IA32-v3a] section 5.15 "Exception and Interrupt
   Reference" for a description of each of these exceptions. */
void
exception_init (void) 
{
  /* These exceptions can be raised explicitly by a user program,
     e.g. via the INT, INT3, INTO, and BOUND instructions.  Thus,
     we set DPL==3, meaning that user programs are allowed to
     invoke them via these instructions. */
  intr_register_int (3, 3, INTR_ON, kill, "#BP Breakpoint Exception");
  intr_register_int (4, 3, INTR_ON, kill, "#OF Overflow Exception");
  intr_register_int (5, 3, INTR_ON, kill,
                     "#BR BOUND Range Exceeded Exception");

  /* These exceptions have DPL==0, preventing user processes from
     invoking them via the INT instruction.  They can still be
     caused indirectly, e.g. #DE can be caused by dividing by
     0.  */
  intr_register_int (0, 0, INTR_ON, kill, "#DE Divide Error");
  intr_register_int (1, 0, INTR_ON, kill, "#DB Debug Exception");
  intr_register_int (6, 0, INTR_ON, kill, "#UD Invalid Opcode Exception");
  intr_register_int (7, 0, INTR_ON, kill,
                     "#NM Device Not Available Exception");
  intr_register_int (11, 0, INTR_ON, kill, "#NP Segment Not Present");
  intr_register_int (12, 0, INTR_ON, kill, "#SS Stack Fault Exception");
  intr_register_int (13, 0, INTR_ON, kill, "#GP General Protection Exception");
  intr_register_int (16, 0, INTR_ON, kill, "#MF x87 FPU Floating-Point Error");
  intr_register_int (19, 0, INTR_ON, kill,
                     "#XF SIMD Floating-Point Exception");

  /* Most exceptions can be handled with interrupts turned on.
     We need to disable interrupts for page faults because the
     fault address is stored in CR2 and needs to be preserved. */
  intr_register_int (14, 0, INTR_OFF, page_fault, "#PF Page-Fault Exception");
}

/* Prints exception statistics. */
void
exception_print_stats (void) 
{
  printf ("Exception: %lld page faults\n", page_fault_cnt);
}

/* Handler for an exception (probably) caused by a user process. */
static void
kill (struct intr_frame *f) 
{
  /* This interrupt is one (probably) caused by a user process.
     For example, the process might have tried to access unmapped
     virtual memory (a page fault).  For now, we simply kill the
     user process.  Later, we'll want to handle page faults in
     the kernel.  Real Unix-like operating systems pass most
     exceptions back to the process via signals, but we don't
     implement them. */
     
  /* The interrupt frame's code segment value tells us where the
     exception originated. */
  switch (f->cs)
    {
    case SEL_UCSEG:
      /* User's code segment, so it's a user exception, as we
         expected.  Kill the user process.  */
      printf ("%s: dying due to interrupt %#04x (%s).\n",
              thread_name (), f->vec_no, intr_name (f->vec_no));
      intr_dump_frame (f);
//        exit(-1);
      thread_exit (); 

    case SEL_KCSEG:
      /* Kernel's code segment, which indicates a kernel bug.
         Kernel code shouldn't throw exceptions.  (Page faults

      intr_dump_frame (f);
      PANIC ("Kernel bug - unexpected interrupt in kernel"); 

    default:
      /* Some other code segment?  Shouldn't happen.  Panic the
         kernel. */
      printf ("Interrupt %#04x (%s) in unknown segment %04x\n",
             f->vec_no, intr_name (f->vec_no), f->cs);
//	exit(-1);
      thread_exit ();
    }
}

/* Page fault handler.  This is a skeleton that must be filled in
   to implement virtual memory.  Some solutions to project 2 may
   also require modifying this code.

   At entry, the address that faulted is in CR2 (Control Register
   2) and information about the fault, formatted as described in
   the PF_* macros in exception.h, is in F's error_code member.  The
   example code here shows how to parse that information.  You
   can find more information about both of these in the
   description of "Interrupt 14--Page Fault Exception (#PF)" in
   [IA32-v3a] section 5.15 "Exception and Interrupt Reference". */
static void
page_fault (struct intr_frame *f) 
{
  bool not_present;  /* True: not-present page, false: writing r/o page. */
  bool write;        /* True: access was write, false: access was read. */
  bool user;         /* True: access by user, false: access by kernel. */
  void *fault_addr;  /* Fault address. */

  /* Obtain faulting address, the virtual address that was
     accessed to cause the fault.  It may point to code or to
     data.  It is not necessarily the address of the instruction
     that caused the fault (that's f->eip).
     See [IA32-v2a] "MOV--Move to/from Control Registers" and
     [IA32-v3a] 5.15 "Interrupt 14--Page Fault Exception
     (#PF)". */
  asm ("movl %%cr2, %0" : "=r" (fault_addr));

  /* Turn interrupts back on (they were only off so that we could
     be assured of reading CR2 before it changed). */
  intr_enable ();

  /* Count page faults. */
  page_fault_cnt++;

  /* Determine cause. */
  not_present = (f->error_code & PF_P) == 0;
  write = (f->error_code & PF_W) != 0;
  user = (f->error_code & PF_U) != 0;


//  printf("fault addr : %p, esp : %p stack base : %p\n", fault_addr, f->esp, thread_current()->stack_base);
//  if(!write) printf("read fault\n");
//  if(f->esp > fault_addr) printf("f->esp is larger that faultaddr\n");

//  printf("not_present : %d, write : %d, user : %d\n", not_present, write, user);


//  sema_get();
//   thread_current();
//  get_swap_sema();
//  printf("fault addr : %p\n", fault_addr);
/*
  struct thread *current = thread_current();
  uint32_t *swap_check = invalid_check(current);
  if(swap_check != NULL) {
      while(swap_check != NULL) {
         bool checker = swap_in(current, swap_check);
         if(checker == false) {
            swap_out();
            swap_in(current, swap_check);
         }
         swap_check = invalid_check(current);
      }

//     swap_in(current, swap_check);
     return;
  }
*/
  if(user && fault_addr > PHYS_BASE) { /*sema_release();*/ exit(-1); }
//  if(!user && fault_addr > PHYS_BASE - PGSIZE && fault_addr < PHYS_BASE) exit(-1);
//  printf("exit 1\n");
  if(fault_addr < 0x08084000) { /*sema_release(); */exit(-1); }
//  printf("exit 2\n");
  if(!write && fault_addr < f->esp) { /*sema_release();*/ exit(-1); }
//  printf("exit 3\n");
  if(!write && fault_addr < f->esp - PGSIZE) { /*sema_release();*/ exit(-1); }
//  printf("exit 4\n");
  if(user && f->esp > PHYS_BASE) { /*sema_release();*/ exit(-1); }
//  printf("exit 5\n");
  if(!not_present) { /*sema_release();*/ exit(-1); }
//  printf("exit 6\n");

//  printf("fault addr : %p\n", fault_addr);
  uint32_t *pde, *pg, *pt, *pte;
  pde = thread_current()->pagedir + pd_no(fault_addr);
  //first, try to find at the page list,
    uint32_t vaddr = (uint32_t)fault_addr & PTE_ADDR;
    struct page_list_elem *p_elem = check_allocated_page((uint32_t *)vaddr);
  
    if(p_elem != NULL && p_elem->thread_ != thread_current()) {
//    printf("exit at here\n");
//    sema_release();
      exit(-1); 
    }
/*
    else if(p_elem != NULL && p_elem->thread_ == thread_current()) {
       //handlin swap in
       if(p_elem->valid == false) {
          reclaim_frame(thread_current(), (uint32_t *)vaddr);
          return;
        }
       else printf("what should we do...?\n");
    }
*/
    if(*pde == 0) {
//    printf("*pde == 0, make new pde\n");
      pg = palloc_get_page(PAL_ZERO);
     *pde = pde_create(pg);
    }
//    else if(check_allocated_page(*pde, thread_current()) == false)
//      exit(-1);
    pt = pde_get_pt(*pde);
    pte = &pt[pt_no((void *)vaddr)];
    pg = palloc_get_page(PAL_USER | PAL_ZERO);
    *pte = pte_create_user(pg, true);

//    printf("kpage : %p\n", pg);

//    sema_get();
    add_page_entry(pde, pg, NULL, thread_current()->name, false);
//    sema_release();

//    frame_sema_get();
    frame_insert(thread_current()->name, pg, pde);
//    frame_sema_release();
    thread_current()->stack_base -= PGSIZE;
//sema_release();
    return;

//   }
//  }
/*
#ifdef USERPROG
if(!not_present) exit(-1);

 if(user && not_present) {
  if(!is_user_vaddr(fault_addr) || fault_addr < (void *)0x08084000)
    exit(-1); 

  if(fault_addr <= f->esp - PGSIZE) exit(-1);
  void *tmp = search_exist(fault_addr);
  uint32_t *pt, *pde;
  pde = thread_current()->pagedir + pd_no(fault_addr);

  if(*pde == 0) {
    pt = palloc_get_page(PAL_USER | PAL_ZERO);

   *pde = pde_create(pt);
   uint32_t *pte = pte_create_user(pt, true);
   uint32_t *paddr = pte_get_page(*pte) + pg_ofs (fault_addr);
   uint32_t vaddr = (uint32_t)fault_addr & PTE_ADDR;
   paddr = pte_get_page(*pte);
   return;
   
  }
  pt = pde_get_pt(*pde);
  uint32_t *pte = &pt[pt_no (fault_addr)];
  uint32_t *paddr = pte_get_page(*pte) + pg_ofs (fault_addr);
  uint32_t vaddr = (uint32_t)fault_addr & PTE_ADDR;
  paddr = pte_get_page(*pte);
  pt = palloc_get_page(PAL_USER | PAL_ZERO);
  *pte = pte_create_user(pt, true);
   return;
 }

 if(is_user_vaddr(fault_addr)) {
  if(!not_present) 
exit(-1);

  if(fault_addr < (void *)0x08084000)
 exit(-1);

  uint32_t *pt, *pde;
  pde = thread_current()->pagedir + pd_no(fault_addr);
  if(*pde != 0) {
   pt = pde_get_pt(*pde);
   uint32_t *pte = &pt[pt_no (fault_addr)];
   uint32_t *paddr = pte_get_page(*pte) + pg_ofs (fault_addr);
   uint32_t vaddr = (uint32_t)fault_addr & PTE_ADDR;
   paddr = pte_get_page(*pte);
   pt = palloc_get_page(PAL_USER | PAL_ZERO);

//   if(pt + PGSIZE > PHYS_BASE) exit(-1);
   *pte = pte_create_user(pt, true);
   return;
  }
 }

 exit(-1);
#endif

*/
 exit(-1);

  /* To implement virtual memory, delete the rest of the function
     body, and replace it with code that brings in the page to
     which fault_addr refers. */
  printf ("Page fault at %p: %s error %s page in %s context.\n",
          fault_addr,
          not_present ? "not present" : "rights violation",
          write ? "writing" : "reading",
          user ? "user" : "kernel");
  kill (f);
}

