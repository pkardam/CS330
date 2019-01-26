#include<init.h>
#include<lib.h>
#include<context.h>
#include<memory.h>

/*char* VtP(u64 vadd,struct exec_context *current)
{
  u64* pointer_14 = (u64*)osmap(current->pgd);
  u64 oset_14 = (va & 0xFF8000000000) >> 39;

  if((u64)(*(pointer_14 + oset_14)&1)==0)
    return -1;
  u64 page_13 = *(pointer_14 + oset_14);
  page_13 = page_13 >> 12;
  u64* pointer_13 = (u64*)osmap(page_13);
  u64 oset_13 = (vadd & 0x7FC0000000) >> 30;

  if((u64)(*(pointer_13 + oset_13)&1)==0)
    return -1;
  u64 page_12 = *(pointer_13 + oset_13);
  page_12 = page_12 >> 12;
  u64* pointer_12 = (u64*)osmap(page_12);
  u64 oset_12 = (vadd & 0x3FE00000) >> 21;

  if((u64)(*(pointer_12 + oset_12)&1)==0)
    return -1;
  u64 page_11 = *(pointer_12 + oset_12);
  page_11 = page_11 >> 12;
  u64* pointer_11 = (u64*)osmap(page_11);
  u64 oset_11 = (vadd & 0x1FF000) >> 12;

  if((u64)(*(pointer_11 + oset_11)&1)==0)
    return -1;
  u64 page = *(pointer_11 + oset_11);
  page = page >> 12;
  u64* pointer = (u64*)osmap(page);
  u64 oset = vadd ;

  return (pointer + oset);

  
}*/


/*System Call handler*/
int do_syscall(int syscall, u64 param1, u64 param2, u64 param3, u64 param4)
{
    struct exec_context *current = get_current_ctx();
    printf("[GemOS] System call invoked. syscall no  = %d\n", syscall);
    switch(syscall)
    {
          case SYSCALL_EXIT:
                              printf("[GemOS] exit code = %d\n", (int) param1);
                              do_exit();
                              break;
          case SYSCALL_GETPID:
                              printf("[GemOS] getpid called for process %s, with pid = %d\n", current->name, current->id);
                              return current->id;      
          case SYSCALL_WRITE:
                             {
                                char *buf = (char *)param1;
                                //u64 badd = param1;


                                if(((param2>1024) || param2<0))
                                {
                                  //invalid length
                                  return -1
                                }
                                
                                u64* pointer_14 = (u64*)osmap(current->pgd);
                                u64 oset_14 = (param1 & 0xFF8000000000) >> 39;
                                if((u64)(*(pointer_14 + oset_14)&1)==0)
                                  return -1;
                                u64 page_13 = *(pointer_14 + oset_14);
                                page_13 = page_13 >> 12;
                                u64* pointer_13 = (u64*)osmap(page_13);
                                u64 oset_13 = (param1 & 0x7FC0000000) >> 30;

                                if((u64)(*(pointer_13 + oset_13)&1)==0)
                                  return -1;
                                u64 page_12 = *(pointer_13 + oset_13);
                                page_12 = page_12 >> 12;
                                u64* pointer_12 = (u64*)osmap(page_12);
                                u64 oset_12 = (param1 & 0x3FE00000) >> 21;

                                if((u64)(*(pointer_12 + oset_12)&1)==0)
                                  return -1;
                                u64 page_11 = *(pointer_12 + oset_12);
                                page_11 = page_11 >> 12;
                                u64* pointer_11 = (u64*)osmap(page_11);
                                u64 oset_11 = (param1 & 0x1FF000) >> 12;

                                if((u64)(*(pointer_11 + oset_11)&1)==0)
                                  return -1;



                                u64 oset_14 = ((param1+param2-1) & 0xFF8000000000) >> 39;
                                if((u64)(*(pointer_14 + oset_14)&1)==0)
                                  return -1;
                                u64 page_13 = *(pointer_14 + oset_14);
                                page_13 = page_13 >> 12;
                                u64* pointer_13 = (u64*)osmap(page_13);
                                u64 oset_13 = ((param1+param2-1) & 0x7FC0000000) >> 30;

                                if((u64)(*(pointer_13 + oset_13)&1)==0)
                                  return -1;
                                u64 page_12 = *(pointer_13 + oset_13);
                                page_12 = page_12 >> 12;
                                u64* pointer_12 = (u64*)osmap(page_12);
                                u64 oset_12 = ((param1+param2-1) & 0x3FE00000) >> 21;

                                if((u64)(*(pointer_12 + oset_12)&1)==0)
                                  return -1;
                                u64 page_11 = *(pointer_12 + oset_12);
                                page_11 = page_11 >> 12;
                                u64* pointer_11 = (u64*)osmap(page_11);
                                u64 oset_11 = ((param1+param2-1) & 0x1FF000) >> 12;

                                if((u64)(*(pointer_11 + oset_11)&1)==0)
                                  return -1;

                                for(int i=0;i<param2;i++){
                                  printf("%c", *(buf+i));
                                }
                             }
          case SYSCALL_EXPAND:
                             {

                                if(param1>1024 || param1<0)
                                  return -1;
                                struct mm_segment curr_val;
                                if(param1==MAP_RD)
                                {
                                  curr_val = (current->mms[MM_SEG_RODATA]);
                                  if (4096*param2 > curr_val.end - curr_val.next_free)
                                    return 0;
                                }
                                else if(param1==MAP_WR)
                                {
                                  curr_val = (current->mms[MM_SEG_DATA]);
                                  if (4096*param2 > curr_val.end - curr_val.next_free)
                                    return 0;
                                }
                                curr_val.next_free = curr_val.next_free + 4096*param2;
                                u64 final = curr_val.next_free - 4096*param2;
                                return final;
                             }
          case SYSCALL_SHRINK:
                             {  
                                struct mm_segment curr_val;
                                if(param1==MAP_RD)
                                {
                                  curr_val = (current->mms[MM_SEG_RODATA]);
                                  if (curr_val.next_free - 4096*param2 < curr_val.start)
                                    return 0;
                                }
                                else if(param1==MAP_WR)
                                {
                                  curr_val = (current->mms[MM_SEG_DATA]);
                                  if (curr_val.next_free - 4096*param2 < curr_val.start)
                                    return 0;
                                }

                                curr_val.next_free = curr_val.next_free - 4096*param2;
                                
                                for(u64 i=0;i<param1;i++)
                                {
                                  u64 bad = curr_val.next_free - 1;
                                  u64* pointer_14 = (u64*)osmap(current->pgd);
                                  u64 oset_14 = (bad & 0xFF8000000000) >> 39;
                                  if((u64)(*(pointer_14 + oset_14)&1)==0)
                                    return -1;
                                  u64 page_13 = *(pointer_14 + oset_14);
                                  page_13 = page_13 >> 12;
                                  u64* pointer_13 = (u64*)osmap(page_13);
                                  u64 oset_13 = (bad & 0x7FC0000000) >> 30;

                                  if((u64)(*(pointer_13 + oset_13)&1)==0)
                                    return -1;
                                  u64 page_12 = *(pointer_13 + oset_13);
                                  page_12 = page_12 >> 12;
                                  u64* pointer_12 = (u64*)osmap(page_12);
                                  u64 oset_12 = (bad & 0x3FE00000) >> 21;

                                  if((u64)(*(pointer_12 + oset_12)&1)==0)
                                    return -1;
                                  u64 page_11 = *(pointer_12 + oset_12);
                                  page_11 = page_11 >> 12;
                                  u64* pointer_11 = (u64*)osmap(page_11);
                                  u64 oset_11 = (bad & 0x1FF000) >> 12;

                                  if((u64)(*(pointer_11 + oset_11)&1)==0)
                                    return -1;

                                  u64 page = *(pointer_11 + oset_11);
                                  page = page >> 12;
                                  os_pfn_free(USER_REG,page);
                                  for(int i=0;i<512;i++){
                                    *(pointer_11 + i) = 0;
                                  }

                                }
                                return curr_val.next_free;

                             }
                             
          default:
                              return -1;
                                
    }
    return 0;   /*GCC shut up!*/
}

extern int handle_div_by_zero(void)
{
    u64 rbp;
    asm volatile("mov %%rbp, %0;"
                :"=r" (rbp)
                :
                :"memory"
    );

    printf("[interrupt] Divide by zero\n", rbp + 8);
    do_exit();
    return 0;
}

extern int handle_page_fault(void)
{
    /*Your code goes in here*/
    /*asm volatile("push %%rax;"
                 "push %%rbx;"
                 "push %%rcx;"
                 "push %%rdx;"
                 "push %%rsi;"
                 "push %%rdi;"
                 "push %%r8;"
                 "push %%r9;"
                 "push %%r10;"
                 "push %%r11;"
                 "push %%r12;"
                 "push %%r13;"
                 "push %%r14;"
                 "push %%r15;"
                 
    );

    u64* rbp;
    asm volatile("mov %%rbp, %0;"
                :"=r" (rbp)
                :
                :"memory"
    );

    u64 cr2;
    asm volatile("mov %%cr2, %0;"
                :"=r" (cr2)
                :
                :"memory"
    );

    u64* rsp;
    asm volatile("mov %%rsp, %0;"
                 :"=r" (rsp)
                 :
                 :"memory"
    );


    struct exec_context *current = get_current_ctx();





    printf("page fault handler: unimplemented!\n");
    return 0;*/
}
