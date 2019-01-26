#include<context.h>
#include<memory.h>
#include<lib.h>

void cleanup(u32 fpn){
	for(u32 i=0;i<512;i++){
		*((u64*)osmap(fpn)+i) = 0;
	}
}

void prepare_context_mm(struct exec_context *ctx)
{

	u32 page_14 = os_pfn_alloc(OS_PT_REG);cleanup(page_14);
	ctx->pgd = page_14;

    u64* pointer_14 = (u64*)osmap(ctx->pgd);
    u64 * pointer_13 =0;
    u64 * pointer_12 =0;
    u64 * pointer_11 =0;


	u64 codea = ctx->mms[MM_SEG_CODE].start;
	u64 dataa = ctx->mms[MM_SEG_DATA].start;
	u64 stacka = ctx->mms[MM_SEG_STACK].end- (1 << 12) ;
	
	u64 oset_14 = codea >> 39;
	//printf("14: %x\n",oset_14);
	//printf("val 14: %x\n",pointer_14);
	if((u64)(*(pointer_14 + oset_14)&1)==0)
	{
		u32 page_13 = os_pfn_alloc(OS_PT_REG);cleanup(page_13);
		u64 page_13a = page_13;
		page_13a = (page_13a << 12) + 7;
		*(pointer_14 + oset_14) = page_13a;
		pointer_13 = (u64*)osmap(page_13);
		//printf("Adrress : %x\n",pointer_13);
		//printf("PTE 14 %x",*(pointer_14 + oset_14));
		
	}
	else
	{
		u64 x = *(pointer_14 + oset_14);
		x = x >> 12;
		pointer_13 = (u64*)osmap(x);
		//printf("Adrress : %x\n",pointer_13);
		//printf("PTE 14 : %x\n",*(pointer_14 + oset_14));
	}
	

	u64 oset_13 = (codea & 0x7FC0000000) >> 30;
	//printf("l3 : %x\n",oset_13);
	u32 page_12;
	if((u64)(*(pointer_13 + oset_13)&1)==0)
	{
		page_12 = os_pfn_alloc(OS_PT_REG);cleanup(page_12);
		u64 page_12a = page_12;
		page_12a = (page_12a << 12) + 7;
		*(pointer_13 + oset_13) = page_12a;
		pointer_12 = (u64*)osmap(page_12);
		//rintf("Adrress : %x\n",pointer_12);
		//printf("PTE 13 %x\n",*(pointer_13 + oset_13));
		//printf("here\n");
	}
	else
	{
		u64 y = *(pointer_13 + oset_13); 
		y = y >> 12;
		pointer_12 = (u64*)osmap(y);
		//printf("Adrress : %x\n",pointer_12);
		//printf("PTE 13 : %x\n",*(pointer_13 + oset_13));
	}
	

	u64 oset_12 = (codea & 0x3FE00000)>> 21;
	//printf("12 :%x %x\n",page_12,oset_12);
	//printf("bool %x\n",*((u64*)osmap(page_12) + oset_12));
	if((u64)(*((u64*)osmap(page_12) + oset_12)&1)==0)
	{
		u32 page_11 = os_pfn_alloc(OS_PT_REG);cleanup(page_11);
		u64 page_11a = page_11;
		page_11a = (page_11a << 12) + 7;
		*((u64*)osmap(page_12) + oset_12) = page_11a;
		pointer_11 = (u64*)osmap(page_11);
		//printf("Adrress : %x\n",pointer_11);
		//printf("PTE 12 %x",*((u64*)osmap(page_12) + oset_12));
	}
	else
	{
		u64 z = *((u64*)osmap(page_12) + oset_12);
		z = z >> 12;
		pointer_11 = (u64*)osmap(z);
		//printf("Adrress : %x\n",pointer_11);
		//printf("PTE 12 : %x\n",*(pointer_12 + oset_12));
	}
	

	u64 oset_11 = (codea & 0x1FF000) >> 12;
	u32 page = os_pfn_alloc(USER_REG);cleanup(page);
	u64 pagea = page;
	pagea = (pagea << 12) + 5;
	*(pointer_11 + oset_11) = pagea;
	//printf("PTE 11: %x\n",*(pointer_11 + oset_11));




	//printf("data\n");
	
	oset_14 = dataa >> 39;
	//printf("14 : %x\n",oset_14);
	if((u64)(*(pointer_14 + oset_14)&1)==0)
	{
		u32 page_13b = os_pfn_alloc(OS_PT_REG);cleanup(page_13b);
		u64 page_13ba = page_13b;
		page_13ba = (page_13ba << 12) + 7;
		*(pointer_14 + oset_14) = page_13ba;
		pointer_13 = (u64*)osmap(page_13b);
		//printf("Adrress : %x\n",pointer_13);
		//printf("PTE 14 %x",*(pointer_14 + oset_14));
		
	}
	else
	{
		u64 x = *(pointer_14 + oset_14);
		x = x >> 12;
		pointer_13 = (u64*)osmap(x);
		//printf("Adrress : %x\n",pointer_13);
		//printf("PTE 14 : %x\n",*(pointer_14 + oset_14));
	}
	

	oset_13 = (dataa & 0x7FC0000000) >> 30;
	//printf("l3 : %x\n",oset_13);
	u32 page_12b;
	if((u64)(*(pointer_13 + oset_13)&1)==0)
	{
		page_12b = os_pfn_alloc(OS_PT_REG);cleanup(page_12b);
		u64 page_12ba = page_12b;
		page_12ba = (page_12ba << 12) + 7;
		*(pointer_13 + oset_13) = page_12ba;
		pointer_12 = (u64*)osmap(page_12b);
		//rintf("Adrress : %x\n",pointer_12);
		//printf("PTE 13 %x\n",*(pointer_13 + oset_13));
		//printf("here\n");
	}
	else
	{
		u64 y = *(pointer_13 + oset_13); 
		y = y >> 12;
		pointer_12 = (u64*)osmap(y);
		//printf("Adrress : %x\n",pointer_12);
		//printf("PTE 13 : %x\n",*(pointer_13 + oset_13));
	}
	

	oset_12 = (dataa & 0x3FE00000)>> 21;
	//printf("12 :%x %x\n",page_12b,oset_12);
	//printf("bool %x\n",*((u64*)osmap(page_12b) + oset_12));
	if((u64)(*((u64*)osmap(page_12b) + oset_12)&1)==0)
	{
		u32 page_11b = os_pfn_alloc(OS_PT_REG);cleanup(page_11b);
		u64 page_11ba = page_11b;
		page_11ba = (page_11ba << 12) + 7;
		*((u64*)osmap(page_12b) + oset_12) = page_11ba;
		pointer_11 = (u64*)osmap(page_11b);
		//printf("Adrress : %x\n",pointer_11);
		//printf("PTE 12 %x",*((u64*)osmap(page_12b) + oset_12));
	}
	else
	{
		u64 z = *((u64*)osmap(page_12b) + oset_12);
		z = z >> 12;
		pointer_11 = (u64*)osmap(z);
		//printf("Adrress : %x\n",pointer_11);
		//printf("PTE 12 : %x\n",*(pointer_12 + oset_12));
	}
	

	oset_11 = (dataa & 0x1FF000) >> 12;
	
	*(pointer_11 + oset_11) = (ctx->arg_pfn << 12) + 7;
	//printf("11 : %x\n",oset_11);
	//printf("PTE 11 %x\n",*(pointer_11 + oset_11));
    


	//printf("Stack\n");
	
	oset_14 = stacka >> 39;

	//printf("14 : %x\n",oset_14);
	if((*(pointer_14 + oset_14)&1)==0)
	{
		u32 page_13c = os_pfn_alloc(OS_PT_REG);cleanup(page_13c);
		u64 page_13ca = page_13c;
		page_13ca = (page_13ca << 12) + 7;
		*(pointer_14 + oset_14) = page_13ca;
		pointer_13 = (u64*)osmap(page_13c);
		//printf("PTE 14 %x",*(pointer_14 + oset_14));
		//printf("Adrress : %x\n",pointer_13);
		
	}
	else
	{
		u64 x1 = *(pointer_14 + oset_14);
		x1 = x1 >> 12;
		pointer_13 = (u64*)osmap(x1);
		//printf("Adrress : %x\n",pointer_13);
		//printf("Offset : %x\n",oset_14);
		//printf("14 : %x\n",*(pointer_14 + oset_14));
	}
	

	oset_13 = (stacka & 0x7FC0000000)>> 30;
	//printf("13 : %x\n",oset_13);
	if((*(pointer_13 + oset_13)&1)==0)
	{
		u32 page_12c = os_pfn_alloc(OS_PT_REG);cleanup(page_12c);
		u64 page_12ca = page_12c;
		page_12ca = (page_12ca << 12) + 7;
		*(pointer_13 + oset_13) = page_12ca;
		pointer_12 = (u64*)osmap(page_12c);
		//printf("Adrress : %x\n",pointer_12);
		//printf("PTE 13 %x\n",*(pointer_13 + oset_13));
	}
	else
	{
		u64 y1 = *(pointer_13 + oset_13);
		y1 = y1 >> 12;
		u64* pointer_12 = (u64*)osmap(y1);
		//printf("Adrress : %x\n",pointer_12);
		//printf("PTE 13 : %x\n",*(pointer_13 + oset_13));	
	}
	

	oset_12 = (stacka & 0x3FE00000) >> 21;
	//printf("12 : %x\n",oset_12);
	if((*(pointer_12 + oset_12)&1)==0)
	{
		u32 page_11c = os_pfn_alloc(OS_PT_REG);cleanup(page_11c);
		u64 page_11ca = page_11c;
		page_11ca = (page_11ca << 12) + 7;
		*(pointer_12 + oset_12) = page_11ca;
		pointer_11 = (u64*)osmap(page_11c);
		//printf("PTE 12 %x\n",*(pointer_12 + oset_12));
		//printf("Adrress : %x",pointer_11);
	}
	else
	{
		u64 z1 = *(pointer_12 + oset_12);
		z1 = z1 >> 12;
		u64* pointer_11 = (u64*)osmap(*(pointer_12 + oset_12));
		//printf("Adrress : %x\n",pointer_11);
		//printf("PTE 12 : %x\n",*(pointer_12 + oset_12));
	}
	

	oset_11 = (stacka & 0x1FF000) >> 12;
	u32 pagec = os_pfn_alloc(USER_REG);cleanup(pagec);
	pagec = (pagec << 12) + 7;
	*(pointer_11 + oset_11) = pagec;
	//printf("11 : %x\n",oset_11);
	//printf("PTE 11 : %x\n",*(pointer_11 + oset_11));


   return;
}
void cleanup_context_mm(struct exec_context *ctx)
{


	u64 codeva = ctx->mms[MM_SEG_CODE].start;
	u64 datava = ctx->mms[MM_SEG_DATA].start;
	u64 stackva = ctx->mms[MM_SEG_STACK].end- (1 << 12) ;


	//printf("Code\n");
	u64* pointer_14 = (u64*)osmap(ctx->pgd);
	u64 oset_14 = codeva >> 39;
	u64 PTE_14 = *(pointer_14 + oset_14);
	u64 page_13_c = PTE_14 >> 12;
	

	u64* pointer_13 = (u64*)osmap(page_13_c);
	u64 oset_13 = (codeva & 0x7FC0000000) >> 30;
	u64 PTE_13 = *(pointer_13 + oset_13);
	u64 page_12_c = PTE_13 >> 12;
	

	u64* pointer_12 = (u64*)osmap(page_12_c);
	u64 oset_12 = (codeva & 0x3FE00000) >> 21;
	u64 PTE_12 = *(pointer_12 + oset_12);
	u64 page_11_c = PTE_12 >> 12;
	

	u64* pointer_11 = (u64*)osmap(page_11_c);
	u64 oset_11 = (codeva & 0x1FF000) >> 12;
	u64 PTE_11 = *(pointer_11 + oset_11);
	u64 page_c = PTE_11 >> 12;
	


	//printf("Data\n");
	oset_14 = datava >> 39;
	PTE_14 = *(pointer_14 + oset_14);
	u64 page_13_d = PTE_14 >> 12;
	

	pointer_13 = (u64*)osmap(page_13_d);
	oset_13 = (datava & 0x7FC0000000) >> 30;
	PTE_13 = *(pointer_13 + oset_13);
	u64 page_12_d = PTE_13 >> 12;
	

	pointer_12 = (u64*)osmap(page_12_d);
	oset_12 = (datava & 0x3FE00000) >> 21;
	PTE_12 = *(pointer_12 + oset_12);
	u64 page_11_d = PTE_12 >> 12;
	


	//printf("Stack\n");
	oset_14 = stackva >> 39;
	PTE_14 = *(pointer_14 + oset_14);
	u64 page_13_s = PTE_14 >> 12;

	pointer_13 = (u64*)osmap(page_13_s);
	oset_13 = (stackva & 0x7FC0000000) >> 30;
	PTE_13 = *(pointer_13 + oset_13);
	u64 page_12_s = PTE_13 >> 12;
	

	pointer_12 = (u64*)osmap(page_12_s);
	oset_12 = (stackva & 0x3FE00000) >> 21;
	PTE_12 = *(pointer_12 + oset_12);
	u64 page_11_s = PTE_12 >> 12;
	

	pointer_11 = (u64*)osmap(page_11_s);
	oset_11 = (stackva & 0x1FF000) >> 12;
	PTE_11 = *(pointer_11 + oset_11);
	u64 page_s = PTE_11 >> 12;
	


	os_pfn_free(OS_PT_REG,page_13_c);
	if(page_13_c != page_13_d)
	{
		os_pfn_free(OS_PT_REG,page_13_d);
	}
	if((page_13_c != page_13_s) && (page_13_d != page_13_s))
	{
		os_pfn_free(OS_PT_REG,page_13_s);
	}


	os_pfn_free(OS_PT_REG,page_12_c);
	if(page_12_c != page_12_d)
	{
		os_pfn_free(OS_PT_REG,page_12_d);
	}
	if((page_12_c != page_12_s) && (page_12_d != page_12_s))
	{
		os_pfn_free(OS_PT_REG,page_12_s);
	}


	os_pfn_free(OS_PT_REG,page_11_c);
	if(page_11_c != page_11_d)
	{
		os_pfn_free(OS_PT_REG,page_11_d);
	}
	if((page_11_c != page_11_s) && (page_11_d != page_11_s))
	{
		os_pfn_free(OS_PT_REG,page_11_s);
	}

	os_pfn_free(OS_PT_REG,ctx->pgd);


	os_pfn_free(USER_REG,page_c);
	os_pfn_free(USER_REG,ctx->arg_pfn);
	os_pfn_free(USER_REG,page_s);

   return;
}
