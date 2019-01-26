#include<init.h>
#include<lib.h>
#include<context.h>
#include<memory.h>

#define l4_shift 39
#define l3_shift 30
#define l2_shift 21
#define l1_shift 12

#define l4_mask 0xFF8000000000
#define l3_mask 0x7FC0000000
#define l2_mask 0x3FE00000
#define l1_mask 0x1FF000

/*System Call handler*/
long do_syscall(int syscall, u64 param1, u64 param2, u64 param3, u64 param4)
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
                                // check if len is valid
                                if((param2<=0)||(param2>1024)){
                                    return -1;
                                }
                                char *buf = (char *)param1;
                                //printf("Buffer Address is:%x\n",buf);
                                // check if *buf is a valid address
                                u64* l4_vaddr = (u64*)osmap(current->pgd);
                                u64 buf_addr = param1;
                                u64 buf_end_addr = param1 + param2 - 1;
                                u64 l4_offset = (buf_addr & l4_mask) >> l4_shift;
                                u64 l4_entry = *(l4_vaddr + l4_offset);
                                if((l4_entry%2) == 0){
                                    return -1;
                                }
                                u64 l3_pfn = l4_entry >> 12;
                                u64* l3_vaddr = (u64*)osmap(l3_pfn);
                                u64 l3_offset = (buf_addr & l3_mask) >> l3_shift;
                                u64 l3_entry = *(l3_vaddr + l3_offset);
                                if((l3_entry%2) == 0){
                                    return -1;
                                }
                                u64 l2_pfn = l3_entry >> 12;
                                u64* l2_vaddr = (u64*)osmap(l2_pfn);
                                u64 l2_offset = (buf_addr & l2_mask) >> l2_shift;
                                u64 l2_entry = *(l2_vaddr + l2_offset);
                                if((l2_entry%2) == 0){
                                    return -1;
                                }
                                u64 l1_pfn = l2_entry >> 12;
                                u64* l1_vaddr = (u64*)osmap(l1_pfn);
                                u64 l1_offset = (buf_addr & l1_mask) >> l1_shift;
                                u64 l1_entry = *(l1_vaddr + l1_offset);
                                if((l1_entry%2) == 0){
                                    return -1;
                                }
                                //check if buf + length is valid address
                                l4_offset = (buf_end_addr & l4_mask) >> l4_shift;
                                l4_entry = *(l4_vaddr + l4_offset);
                                if((l4_entry%2) == 0){
                                    return -1;
                                }
                                l3_pfn = l4_entry >> 12;
                                l3_vaddr = (u64*)osmap(l3_pfn);
                                l3_offset = (buf_end_addr & l3_mask) >> l3_shift;
                                l3_entry = *(l3_vaddr + l3_offset);
                                if((l3_entry%2) == 0){
                                    return -1;
                                }
                                l2_pfn = l3_entry >> 12;
                                l2_vaddr = (u64*)osmap(l2_pfn);
                                l2_offset = (buf_end_addr & l2_mask) >> l2_shift;
                                l2_entry = *(l2_vaddr + l2_offset);
                                if((l2_entry%2) == 0){
                                    return -1;
                                }
                                l1_pfn = l2_entry >> 12;
                                l1_vaddr = (u64*)osmap(l1_pfn);
                                l1_offset = (buf_end_addr & l1_mask) >> l1_shift;
                                l1_entry = *(l1_vaddr + l1_offset);
                                if((l1_entry%2) == 0){
                                    return -1;
                                }
                                //print the string if buf and len are valid
                                for(int i=0;i<param2;i++){
                                    printf("%c",*(buf+i));
                                }
                                return param2;
                             }
          case SYSCALL_EXPAND:
                             {  
                                     /*Your code goes here*/
                                    //check if size is valid
                                    if((param1<0)||(param1>512)){
                                        //printf("Returning NULL for size\n");
                                        return NULL;
                                    }
                                    u32 size = (u32)param1;
                                    int flag = (int)param2;
                                    if(flag == MM_RD){
                                        if((current->mms[MM_SEG_RODATA].next_free + 4096*size)>current->mms[MM_SEG_RODATA].end){
                                            //printf("Returning NULL for MM_RD\n");
                                            return NULL;
                                        }else{
                                            u64 return_val = current->mms[MM_SEG_RODATA].next_free;
                                            current->mms[MM_SEG_RODATA].next_free = current->mms[MM_SEG_RODATA].next_free + 4096*size;
                                            //printf("Expand is returning %x\n",return_val);
                                            return return_val;
                                        }
                                    }
                                    if(flag == MM_WR){
                                        if((current->mms[MM_SEG_DATA].next_free + 4096*size)>current->mms[MM_SEG_DATA].end){
                                            //printf("Returning NULL for MM_WR\n");
                                            return NULL;
                                        }else{
                                            u64 return_val = current->mms[MM_SEG_DATA].next_free;
                                            current->mms[MM_SEG_DATA].next_free = current->mms[MM_SEG_DATA].next_free + 4096*size;
                                            //printf("Expand is returning %x\n",return_val);
                                            return return_val;
                                        }
                                    }
                             }
          case SYSCALL_SHRINK:
                             {  
                                    /*Your code goes here*/
                                    u32 size = (u32)param1;
                                    int flag = (int)param2;
                                    unsigned long old_next_free,new_next_free;
                                    if(flag == MM_RD){
                                        if((current->mms[MM_SEG_RODATA].next_free - 4096*size)<current->mms[MM_SEG_RODATA].start){
                                            //printf("Returning NULL for MM_RD\n");
                                            return NULL;
                                        }else{
                                            old_next_free = current->mms[MM_SEG_RODATA].next_free;
                                            current->mms[MM_SEG_RODATA].next_free = current->mms[MM_SEG_RODATA].next_free - 4096*size;
                                            new_next_free = current->mms[MM_SEG_RODATA].next_free;
                                        }
                                    }
                                    if(flag == MM_WR){
                                        if((current->mms[MM_SEG_DATA].next_free - 4096*size)<current->mms[MM_SEG_DATA].start){
                                            //printf("Returning NULL for MM_WR\n");
                                            return NULL;
                                        }else{
                                            old_next_free = current->mms[MM_SEG_DATA].next_free;
                                            current->mms[MM_SEG_DATA].next_free = current->mms[MM_SEG_DATA].next_free - 4096*size;
                                            new_next_free = current->mms[MM_SEG_DATA].next_free;
                                        }
                                    }
                                    for(int i=0;i<size;i++){
                                        unsigned long addr = old_next_free - 4096*i - 4096;
                                        //printf("Shrink is removing data table with starting address : %x\n",addr);
                                        u64* l4_vaddr = (u64*)osmap(current->pgd);
                                        u64 l4_offset = (addr & l4_mask) >> l4_shift;
                                        u64 l4_entry = *(l4_vaddr + l4_offset);
                                        if((l4_entry%2)==0){
                                            continue;
                                        }
                                        u64 l3_pfn = l4_entry >> 12;
                                        u64* l3_vaddr = (u64*)osmap(l3_pfn);
                                        u64 l3_offset = (addr & l3_mask) >> l3_shift;
                                        u64 l3_entry = *(l3_vaddr + l3_offset);
                                        if((l3_entry%2)==0){
                                            continue;
                                        }
                                        u64 l2_pfn = l3_entry >> 12;
                                        u64* l2_vaddr = (u64*)osmap(l2_pfn);
                                        u64 l2_offset = (addr & l2_mask) >> l2_shift;
                                        u64 l2_entry = *(l2_vaddr + l2_offset);
                                        if((l2_entry%2)==0){
                                            continue;
                                        }
                                        u64 l1_pfn = l2_entry >> 12;
                                        u64* l1_vaddr = (u64*)osmap(l1_pfn);
                                        u64 l1_offset = (addr & l1_mask) >> l1_shift;
                                        u64 l1_entry = *(l1_vaddr + l1_offset);
                                        if((l1_entry%2)==0){
                                            continue;
                                        }else{
                                            u64 data_pfn = l1_entry >> 12;
                                            os_pfn_free(USER_REG,data_pfn);
                                            *(l1_vaddr + l1_offset) = 0;
                                        }
                                    }
                                    //printf("Shrink is returning %x\n",new_next_free);
                                    return new_next_free;
                             }
                             
          default:
                              return -1;
                                
    }
    return 0;   /*GCC shut up!*/
}

extern int handle_div_by_zero(void)
{
    /*Your code goes in here*/
    u64* rbp;
    asm volatile("mov %%rbp, %0;"
                :"=r" (rbp)
                :
                :"memory"
    );

    u64 RIP = *(rbp + 1);
    printf("Div-by-zero detected at %x\n",RIP);
    do_exit();
    return 0;
}

extern int handle_page_fault(void)
{
    /*Your code goes in here*/
    //printf("\nPage Fault Handler Starts\n");
    u64* rbp;
    asm volatile("mov %%rbp, %0;"
                :"=r" (rbp)
                :
                :"memory"
    );
    
    
    u64 error_code = *(rbp + 1);
    u64 RIP = *(rbp + 2);

    u64 wrong_addr;
    asm volatile("mov %%cr2, %0;"
                :"=r" (wrong_addr)
                :
                :"memory"
    );

    asm volatile("push %%rsi;"
                 "push %%rdi;"
                 "push %%rax;"
                 "push %%rbx;"
                 "push %%rcx;"
                 "push %%rdx;"
                 "push %%r8;"
                 "push %%r9;"
                 "push %%r10;"
                 "push %%r11;"
                 "push %%r12;"
                 "push %%r13;"
                 "push %%r14;"
                 "push %%r15;"
                 :
                 :
                 :"memory"
    );

    u64* new_rsp;
    asm volatile("mov %%rsp, %0;"
                 :"=r" (new_rsp)
                 :
                 :"memory"
    );

    //printf("At beginning of PFH, RIP:%x, Wrong Address:%x, Error Code:%x\n",RIP,wrong_addr,error_code);
    
    struct exec_context *current = get_current_ctx();

    //printf("Data Start : %x, Data Next_free : %x, Data End : %x\n",current->mms[MM_SEG_DATA].start,current->mms[MM_SEG_DATA].next_free,current->mms[MM_SEG_DATA].end);
    //printf("ROData Start : %x, ROData Next_free : %x, ROData End : %x\n",current->mms[MM_SEG_RODATA].start,current->mms[MM_SEG_RODATA].next_free,current->mms[MM_SEG_RODATA].end);
    //printf("Stack Start : %x, Stack Next_free : %x, Stack End : %x\n",current->mms[MM_SEG_STACK].start,current->mms[MM_SEG_STACK].next_free,current->mms[MM_SEG_STACK].end);


    if((wrong_addr >= current->mms[MM_SEG_DATA].start)&&(wrong_addr < current->mms[MM_SEG_DATA].next_free)){
        //allocate page table for address
        u64* l4_vaddr = (u64*)osmap(current->pgd);
        u64* l3_vaddr;         
        u64* l2_vaddr;         
        u64* l1_vaddr;         
        u64 l4_offset = (wrong_addr & l4_mask) >> l4_shift;
        if(*(l4_vaddr + l4_offset)%2 == 0){
            u32 l3_pfn = os_pfn_alloc(OS_PT_REG);
            u64 l3_pfnval = (u64)l3_pfn;
            l3_pfnval = l3_pfnval << 12;
            l3_pfnval = l3_pfnval + 7;
            *(l4_vaddr + l4_offset) = l3_pfnval;
            l3_vaddr = (u64*)osmap(l3_pfn);
            for(int i=0;i<512;i++){
                *(l3_vaddr + i) = 0;
            }
        }else{
            u64 pte4 = *(l4_vaddr + l4_offset);
            pte4 = pte4 >> 12;
            l3_vaddr = (u64*)osmap(pte4);
        }
        u64 l3_offset = (wrong_addr & l3_mask) >> l3_shift;
        if(*(l3_vaddr + l3_offset)%2 == 0){
            u32 l2_pfn = os_pfn_alloc(OS_PT_REG);
            u64 l2_pfnval = (u64)l2_pfn;
            l2_pfnval = l2_pfnval << 12;
            l2_pfnval = l2_pfnval + 7;
            *(l3_vaddr + l3_offset) = l2_pfnval;
            l2_vaddr = (u64*)osmap(l2_pfn);
            for(int i=0;i<512;i++){
                *(l2_vaddr + i) = 0;
            }
        }else{
            u64 pte3 = *(l3_vaddr + l3_offset);
            pte3 = pte3 >> 12;
            l2_vaddr = (u64*)osmap(pte3);
        }
        u64 l2_offset = (wrong_addr & l2_mask) >> l2_shift;
        if(*(l2_vaddr + l2_offset)%2 == 0){
            u32 l1_pfn = os_pfn_alloc(OS_PT_REG);
            u64 l1_pfnval = (u64)l1_pfn;
            l1_pfnval = l1_pfnval << 12;
            l1_pfnval = l1_pfnval + 7;
            *(l2_vaddr + l2_offset) = l1_pfnval;
            l1_vaddr = (u64*)osmap(l1_pfn);
            for(int i=0;i<512;i++){
                *(l1_vaddr + i) = 0;
            }
        }else{
            u64 pte2 = *(l2_vaddr + l2_offset);
            pte2 = pte2 >> 12;
            l1_vaddr = (u64*)osmap(pte2);
        }
        u64 l1_offset = (wrong_addr & l1_mask) >> l1_shift;
        u32 data_table_pfn = os_pfn_alloc(USER_REG);   
        u64 data_table_pfnval = (u64)data_table_pfn;
        data_table_pfnval = data_table_pfnval << 12;
        data_table_pfnval = data_table_pfnval + 7;
        *(l1_vaddr + l1_offset) = data_table_pfnval;
        //page table allocated and mapping done
        //return to user code
        //printf("Page fault handled successfully.\n\n");
        asm volatile("mov %0, %%rsp;"
                     "pop %%r15;"
                     "pop %%r14;"
                     "pop %%r13;"
                     "pop %%r12;"
                     "pop %%r11;"
                     "pop %%r10;"
                     "pop %%r9;"
                     "pop %%r8;"
                     "pop %%rdx;"
                     "pop %%rcx;"
                     "pop %%rbx;"
                     "pop %%rax;"
                     "pop %%rdi;"
                     "pop %%rsi;"
                     "mov %%rbp, %%rsp;"
                     "pop %%rbp;"
                     "add $8, %%rsp;"
                     "iretq;"
                    :
                    :"r" (new_rsp)
                    :"memory"
        );

        return 0;
    
    }
    if((wrong_addr >= current->mms[MM_SEG_RODATA].start)&&(wrong_addr < current->mms[MM_SEG_RODATA].next_free)){
        //allocate page table for address
        if((error_code%4)/2 == 1){
            printf("Illegal write operation at RIP:%x trying to access address %x with error code:%x\n\n",RIP,wrong_addr,error_code);
            do_exit();
            return 0;
        }
        u64* l4_vaddr = (u64*)osmap(current->pgd);
        u64* l3_vaddr;
        u64* l2_vaddr;
        u64* l1_vaddr;
        u64 l4_offset = (wrong_addr & l4_mask) >> l4_shift;
        if(*(l4_vaddr + l4_offset)%2 == 0){
            u32 l3_pfn = os_pfn_alloc(OS_PT_REG);
            u64 l3_pfnval = (u64)l3_pfn;
            l3_pfnval = l3_pfnval << 12;
            l3_pfnval = l3_pfnval + 7;
            *(l4_vaddr + l4_offset) = l3_pfnval;
            l3_vaddr = (u64*)osmap(l3_pfn);
            for(int i=0;i<512;i++){
                *(l3_vaddr + i) = 0;
            }
        }else{
            u64 pte4 = *(l4_vaddr + l4_offset);
            pte4 = pte4 >> 12;
            l3_vaddr = (u64*)osmap(pte4);
        }
        u64 l3_offset = (wrong_addr & l3_mask) >> l3_shift;
        if(*(l3_vaddr + l3_offset)%2 == 0){
            u32 l2_pfn = os_pfn_alloc(OS_PT_REG);
            u64 l2_pfnval = (u64)l2_pfn;
            l2_pfnval = l2_pfnval << 12;
            l2_pfnval = l2_pfnval + 7;
            *(l3_vaddr + l3_offset) = l2_pfnval;
            l2_vaddr = (u64*)osmap(l2_pfn);
            for(int i=0;i<512;i++){
                *(l2_vaddr + i) = 0;
            }
        }else{
            u64 pte3 = *(l3_vaddr + l3_offset);
            pte3 = pte3 >> 12;
            l2_vaddr = (u64*)osmap(pte3);
        }
        u64 l2_offset = (wrong_addr & l2_mask) >> l2_shift;
        if(*(l2_vaddr + l2_offset)%2 == 0){
            u32 l1_pfn = os_pfn_alloc(OS_PT_REG);
            u64 l1_pfnval = (u64)l1_pfn;
            l1_pfnval = l1_pfnval << 12;
            l1_pfnval = l1_pfnval + 7;
            *(l2_vaddr + l2_offset) = l1_pfnval;
            l1_vaddr = (u64*)osmap(l1_pfn);
            for(int i=0;i<512;i++){
                *(l1_vaddr + i) = 0;
            }
        }else{
            u64 pte2 = *(l2_vaddr + l2_offset);
            pte2 = pte2 >> 12;
            l1_vaddr = (u64*)osmap(pte2);
        }
        u64 l1_offset = (wrong_addr & l1_mask) >> l1_shift;
        u32 data_table_pfn = os_pfn_alloc(USER_REG);   
        u64 data_table_pfnval = (u64)data_table_pfn;
        data_table_pfnval = data_table_pfnval << 12;
        data_table_pfnval = data_table_pfnval + 5;
        *(l1_vaddr + l1_offset) = data_table_pfnval;
        //page table allocated and mapping done
        //return to user code
        //printf("Page fault handled successfully.\n\n");
        
        asm volatile("mov %0, %%rsp;"
                     "pop %%r15;"
                     "pop %%r14;"
                     "pop %%r13;"
                     "pop %%r12;"
                     "pop %%r11;"
                     "pop %%r10;"
                     "pop %%r9;"
                     "pop %%r8;"
                     "pop %%rdx;"
                     "pop %%rcx;"
                     "pop %%rbx;"
                     "pop %%rax;"
                     "pop %%rdi;"
                     "pop %%rsi;"
                     "mov %%rbp, %%rsp;"
                     "pop %%rbp;"
                     "add $8, %%rsp;"
                     "iretq;"
                    :
                    :"r" (new_rsp)
                    :"memory"
        );
        return 0;
    }
    if((wrong_addr >= current->mms[MM_SEG_STACK].start)&&(wrong_addr < current->mms[MM_SEG_STACK].end)){
        //allocate page table for address
        u64* l4_vaddr = (u64*)osmap(current->pgd);
        u64* l3_vaddr;         
        u64* l2_vaddr;         
        u64* l1_vaddr;         
        u64 l4_offset = (wrong_addr & l4_mask) >> l4_shift;
        if(*(l4_vaddr + l4_offset)%2 == 0){
            u32 l3_pfn = os_pfn_alloc(OS_PT_REG);
            u64 l3_pfnval = (u64)l3_pfn;
            l3_pfnval = l3_pfnval << 12;
            l3_pfnval = l3_pfnval + 7;
            *(l4_vaddr + l4_offset) = l3_pfnval;
            l3_vaddr = (u64*)osmap(l3_pfn);
            for(int i=0;i<512;i++){
                *(l3_vaddr + i) = 0;
            }
        }else{
            u64 pte4 = *(l4_vaddr + l4_offset);
            pte4 = pte4 >> 12;
            l3_vaddr = (u64*)osmap(pte4);
        }
        u64 l3_offset = (wrong_addr & l3_mask) >> l3_shift;
        if(*(l3_vaddr + l3_offset)%2 == 0){
            u32 l2_pfn = os_pfn_alloc(OS_PT_REG);
            u64 l2_pfnval = (u64)l2_pfn;
            l2_pfnval = l2_pfnval << 12;
            l2_pfnval = l2_pfnval + 7;
            *(l3_vaddr + l3_offset) = l2_pfnval;
            l2_vaddr = (u64*)osmap(l2_pfn);
            for(int i=0;i<512;i++){
                *(l2_vaddr + i) = 0;
            }
        }else{
            u64 pte3 = *(l3_vaddr + l3_offset);
            pte3 = pte3 >> 12;
            l2_vaddr = (u64*)osmap(pte3);
        }
        u64 l2_offset = (wrong_addr & l2_mask) >> l2_shift;
        if(*(l2_vaddr + l2_offset)%2 == 0){
            u32 l1_pfn = os_pfn_alloc(OS_PT_REG);
            u64 l1_pfnval = (u64)l1_pfn;
            l1_pfnval = l1_pfnval << 12;
            l1_pfnval = l1_pfnval + 7;
            *(l2_vaddr + l2_offset) = l1_pfnval;
            l1_vaddr = (u64*)osmap(l1_pfn);
            for(int i=0;i<512;i++){
                *(l1_vaddr + i) = 0;
            }
        }else{
            u64 pte2 = *(l2_vaddr + l2_offset);
            pte2 = pte2 >> 12;
            l1_vaddr = (u64*)osmap(pte2);
        }
        u64 l1_offset = (wrong_addr & l1_mask) >> l1_shift;
        u32 data_table_pfn = os_pfn_alloc(USER_REG);   
        u64 data_table_pfnval = (u64)data_table_pfn;
        data_table_pfnval = data_table_pfnval << 12;
        data_table_pfnval = data_table_pfnval + 7;
        *(l1_vaddr + l1_offset) = data_table_pfnval;
        //page table allocated and mapping done
        //return to user code
        //printf("Page fault handled successfully.\n\n");
        
        asm volatile("mov %0, %%rsp;"
                     "pop %%r15;"
                     "pop %%r14;"
                     "pop %%r13;"
                     "pop %%r12;"
                     "pop %%r11;"
                     "pop %%r10;"
                     "pop %%r9;"
                     "pop %%r8;"
                     "pop %%rdx;"
                     "pop %%rcx;"
                     "pop %%rbx;"
                     "pop %%rax;"
                     "pop %%rdi;"
                     "pop %%rsi;"
                     "mov %%rbp, %%rsp;"
                     "pop %%rbp;"
                     "add $8, %%rsp;"
                     "iretq;"
                    :
                    :"r" (new_rsp)
                    :"memory"
        );
        return 0;
    }
    printf("Page Fault at RIP:%x trying to access address %x with error code:%x\n\n",RIP,wrong_addr,error_code);
    do_exit();
    return 0;
}
