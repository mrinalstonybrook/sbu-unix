#include <defs.h>
#include <stdio.h>
#include <sys/pci.h>
#include <sys/sata.h>
#include <pmap.h>
#include<sys/ahci.h>
#include<sys/tarfs.h>
#include <sys/e1000.h>

extern char kernmem,physbase ;
struct pci_func pci_e1000,pci_ahci;
uint64_t *pages_for_ahci_start;

static inline void sysOutLong( unsigned short port, uint32_t val ){
	asm volatile( "outl %0, %1"
			: : "a"(val), "Nd"(port) );
}

static inline uint32_t sysInLong( unsigned short port ){
	uint32_t ret;
	asm volatile( "inl %1, %0"
			: "=a"(ret) : "Nd"(port) );
	return ret;
}

// byte -- 8 bits
// word -- 16 bits
// long -- 64 bits

unsigned short pciConfigReadWord (unsigned short bus, unsigned short slot,unsigned short func, unsigned short offset){
	uint32_t address;
	uint32_t lbus = (uint32_t)bus;
	uint32_t lslot = (uint32_t)slot;
	uint32_t lfunc = (uint32_t)func;
	unsigned short tmp = 0;

	address = (uint32_t)((lbus << 16) | (lslot << 11) |
			(lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));


	sysOutLong (0xCF8, address);

	tmp = (unsigned short)((sysInLong (0xCFC) >> ((offset & 2) * 8)) & 0xffff);
	return (tmp);
}


uint32_t ReadWord(unsigned short bus, unsigned short slot,unsigned short func, unsigned short offset){
	uint32_t address;
	uint32_t lbus = (uint32_t)bus;
	uint32_t lslot = (uint32_t)slot;
	uint32_t lfunc = (uint32_t)func;
	uint32_t tmp = 0;


	address = (uint32_t)((lbus << 16) | (lslot << 11) |
			(lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
	sysOutLong (0xCF8, address);
	tmp = (uint32_t)(sysInLong (0xCFC) /* & 0xffff*/);
	return (tmp);
}


void get_mmio_space_size(struct pci_func *pci_device){
	uint32_t address;
	uint32_t lbus = pci_device->bus_num;
	uint32_t lslot = pci_device->slot_num;
	uint32_t lfunc = 0;
	uint32_t tmp = 0;
        unsigned short offset=(unsigned short)(pci_device->mmio_reg|0x0);

	address = (uint32_t)((lbus << 16) | (lslot << 11) |(lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

	sysOutLong (0xCF8, address);
	sysOutLong(0xCFC,0xffffffff);
        tmp=(ReadWord(pci_device->bus_num,pci_device->slot_num,0,(pci_device->mmio_reg|0x0)));   
	pci_device->mmio_reg_size = (~tmp)+1;

	sysOutLong (0xCF8, address);
	sysOutLong (0xCFC,pci_device->mmio_reg_addr);
}

int pci_scan_bus(struct pci_func *pci_device) {
	int bus;
	int slot;
	unsigned short vendor,device;
	for(bus = 0; bus < 256; bus++) {
		for(slot = 0; slot < 32; slot++) {
			vendor = pciConfigReadWord(bus,slot,0,(0x00|0x0)); 
			device = pciConfigReadWord(bus,slot,0,(0x00|0x02));
			if(vendor==pci_device->vendor_id && device==pci_device->device_id){
				pci_device->bus_num=bus;
				pci_device->slot_num=slot;
				pci_device->mmio_reg_addr=ReadWord(bus,slot,0,(pci_device->mmio_reg|0x0));   
				pci_device->irq_line=(ReadWord(bus,slot,0,(pci_device->interrupt_reg|0x0)) & 0xff);   
                                return 1;

			}
		}
	}
return 0;
}

void change_irq(struct pci_func *pci_device,int irq){
	uint32_t address;
	uint32_t lbus = pci_device->bus_num;
	uint32_t lslot = pci_device->slot_num;
	uint32_t lfunc = 0;
	unsigned short offset=(unsigned short)(pci_device->interrupt_reg|0x0);

	address = (uint32_t)((lbus << 16) | (lslot << 11) |(lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

	uint32_t val=ReadWord(lbus,lslot,0,(pci_device->interrupt_reg|0x0));


	sysOutLong (0xCF8, address);
	sysOutLong(0xCFC,((val & 0xffffff00) | irq));
	pci_device->irq_line=(ReadWord(lbus,lslot,0,(pci_device->interrupt_reg|0x0)) & 0xff);   
}


void map_mmio_to_virtual_space(struct pci_func *pci_device){

pci_device->start_virtual_address = (uint64_t)kmalloc_phy((uint64_t)pci_device->mmio_reg_addr , pci_device->mmio_reg_size);

};




void pci_init(void){
	int i;
	uint32_t *tmp;
	/***************************   E1000  *********************************/
	pci_e1000.vendor_id=0x8086;
	pci_e1000.device_id=0x100e;
	pci_e1000.mmio_reg=0x10;
	pci_e1000.interrupt_reg=0x3c;
	pci_scan_bus(&pci_e1000); 
	get_mmio_space_size(&pci_e1000);
	change_irq(&pci_e1000,10);
	map_mmio_to_virtual_space(&pci_e1000);
	e1000_initialise(&pci_e1000);
	printk("\n Device status = [%x]",((((uint32_t *)(pci_e1000.start_virtual_address))+ E1000_STATUS)));  
	printk("\n Device status = [%x]",(*(((uint32_t *)(pci_e1000.start_virtual_address+ E1000_STATUS )))));  
	printk("\n Congrats :: successfull completion of E1000 initialization");
	printk("\n");
	tmp=(uint32_t *)&pci_e1000;
	for(i=0;i<9;i++){
		printk("[%x] ",*tmp);
		tmp++;
	}
	transmit_packet(&pci_e1000);
	print_status_of_transmission(&pci_e1000);
	printk("\n Done Transmission");
	/********************************** AHCI + SATA ***************************/
	pci_ahci.vendor_id=0x8086;
	pci_ahci.device_id=0x2922;
	pci_ahci.mmio_reg=0x24;
	pci_ahci.interrupt_reg=0x3c;
	pci_scan_bus(&pci_ahci); 
	get_mmio_space_size(&pci_ahci);
	map_mmio_to_virtual_space(&pci_ahci);
	pages_for_ahci_start= kmalloc_iden(30*4*1024);
	printk("\n");
	tmp=(uint32_t *)&pci_ahci;
	for(i=0;i<9;i++){
		printk("[%x] ",*tmp);
		tmp++;
	}

	probe_port((HBA_MEM *)pci_ahci.start_virtual_address); 
	//       mem_map_ahci(pci_ahci.start_virtual_address);

	inialize_sata_table();
print_sata_table();

	/************************* TARFS *****************************************/
tarfs_init();
print_tarfs_file_table();
}

