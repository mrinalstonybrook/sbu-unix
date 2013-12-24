#include <sys/idt.h>
#include <defs.h>

extern void printk(const char *fmt,... );

struct idt{
	uint16_t offset_1; // offset bits 0..15
	uint16_t selector; // a code segment selector in GDT or LDT
	unsigned res :3;
	unsigned zero :5;
	unsigned type:4;
	unsigned s:1;
	unsigned dpl:2;
	unsigned p:1;

	// type and attributes, see below
	uint16_t offset_2; // offset bits 16..31
	uint32_t offset_3; // offset bits 31..63
	uint32_t zero1;      // unused, set to 0
}__attribute__((packed));

struct idt idt_entries[256];

void fill_idt_entries(int isr_num,uint64_t  isr_address, int dpl){
	idt_entries[isr_num].offset_1=(isr_address) & 0xffff;
	idt_entries[isr_num].selector=0x08;
	idt_entries[isr_num].res=0;
	idt_entries[isr_num].zero=0;
	idt_entries[isr_num].p=1;
	idt_entries[isr_num].dpl= dpl & 0x3;
	idt_entries[isr_num].s=0;
	idt_entries[isr_num].type=0xE;
	idt_entries[isr_num].offset_2=((isr_address) >> 16) & 0xffff;
	idt_entries[isr_num].offset_3=((isr_address) >> 32) & 0xffffffff;
	idt_entries[isr_num].zero1=0;
}

struct idtr_t {
	uint16_t size;
	uint64_t addr;
}__attribute__((packed));

static struct idtr_t idtr = {
	256*sizeof(struct idt),
	(uint64_t)idt_entries,
};

void _x86_64_asm_lidt(struct idtr_t* idtr);

void reload_idt(){
	_x86_64_asm_lidt(&idtr);
}
