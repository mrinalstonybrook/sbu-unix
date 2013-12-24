#include <defs.h>
#include <my_lib.h>
#include <stdio.h>
#include <sys/pci.h>
#include <sys/e1000.h>


#define KERNBASE 0xffffffff80000000
#define VIRT_TO_PHY(x) ((char *)x-KERNBASE)


struct e1000_rx_desc recv_desc[256];
struct recv_data_buffer recv_buf[256];

char testbuf[500];
char *ffff=testbuf;

struct e1000_tx_desc trans_desc[256];
struct trans_data_buffer trans_buf[256];

void interrupt_handler_e1000(void){
	printk("\n Getting e1000 interrupt");
	//check for recv or transmit interrup
	// behave accordingly
}

void e1000_initialise(struct pci_func *e1000){

//printk("\n testbuf virtual [%x]",testbuf);
//printk("\n testbuf physical [%x]",ffff);

//printk("\nGoing for init :: Device status = [%x]",(*(((uint32_t *)(pci_e1000.start_virtual_address+ E1000_STATUS )))));  

uint64_t start=(uint64_t)(e1000->start_virtual_address);

/******************GENERAL CONFIGURATION********************/

(*((uint32_t *)(start + E1000_CTRL))) |= E1000_CTRL_ASDE;
(*((uint32_t *)(start + E1000_CTRL))) |= E1000_CTRL_SLU;
(*((uint32_t *)(start + E1000_CTRL))) &= (~E1000_CTRL_LRST);
(*((uint32_t *)(start + E1000_CTRL))) &= (~E1000_CTRL_PHY_RST);
(*((uint32_t *)(start + E1000_CTRL))) &= (~E1000_CTRL_ILOS);


(*((uint32_t *)(start + E1000_FCAH))) = 0x0;
(*((uint32_t *)(start + E1000_FCAL))) = 0x0;
(*((uint32_t *)(start + E1000_FCT))) = 0x0;
(*((uint32_t *)(start + E1000_FCTTV))) = 0x0;
(*((uint32_t *)(start + E1000_CTRL))) &= (~E1000_CTRL_RFCE);
(*((uint32_t *)(start + E1000_CTRL))) &= (~E1000_CTRL_TFCE);

/************************Receive Initialization ************************/
/*
Program the Receive Address Register(s) (RAL/RAH) with the desired Ethernet addresses. 
RAL[0]/RAH[0] should always be used to store the Individual Ethernet MAC address of the 
Ethernet controller. Thisa
16 registers contain the lower bits of the 48-bit Ethernet address.
All 32 bits are valid.a
see page 330 and 329
*/

/* not defiend in e1000_hw.h */
#define E1000_RAL       0x05400  /* Receive Address LOW - RW Array */
#define E1000_RAH       0x05404  /* Receive Address High - RW Array */


(*((uint32_t *)(start + E1000_RAL))) = 0x52540012;
(*((uint32_t *)(start + E1000_RAH))) = 0x3456;

(*((uint32_t *)(start + E1000_IMS))) |= E1000_IMS_RXT0;
(*((uint32_t *)(start + E1000_IMS))) |= E1000_IMS_RXO;
(*((uint32_t *)(start + E1000_IMS))) |= E1000_IMS_RXDMT0;
(*((uint32_t *)(start + E1000_IMS))) |= E1000_IMS_RXSEQ;
(*((uint32_t *)(start + E1000_IMS))) |= E1000_IMS_LSC;


/*
#define E1000_RDBAH0   E1000_RDBAH  RX Desc Base Address High (0) - RW 
#define E1000_RDBAL0   E1000_RDBAL  RX Desc Base Address Low (0) - RW 
#define E1000_RDBAH1   0x02904   RX Descriptor Base Address High (1) - RW 
#define E1000_RDBAL1   0x02900   RX Descriptor Base Address Low (1) - RW 
#define E1000_RDBAH    0x02804   RX Descriptor Base Address High - RW 
#define E1000_RDBAL    0x02800   RX Descriptor Base Address Low - RW 
make sure your receive buffers are large enough to store the largest possible standard Ethernet packet (1518 bytes).
buffer 2048 B

little endian 
top-bottom
left - right
--- goes in sequence in  structure
*/


int i=0;
for(i=0;i<256;i++)
recv_desc[i].buffer_addr=(uint64_t)(((char *)(recv_buf[i].buf))-0xffffffff80000000);

//printk("\n RECV_BUF [%x]",recv_buf);
//printk("\n RECV_BUF [%x]",(((char *)recv_buf)-0xffffffff80000000));

recv_buf[0].buf[0]='s';
recv_buf[0].buf[1]='s';
recv_buf[0].buf[2]='s';
recv_buf[0].buf[3]='s';
recv_buf[0].buf[4]='s';
recv_buf[0].buf[5]='\0';
//printk("\n VALUE RECV_BUF [%s]",recv_buf[0].buf);

uint64_t recv_buf_phy=(uint64_t)(((char *)recv_desc)-0xffffffff80000000); 
//printk("\n RECV_BUF_PHY =[%x]",recv_buf_phy);

(*((uint32_t *)(start + E1000_RDBAL))) = (uint32_t)(recv_buf_phy & 0xffffffff);
//printk("\n E1000_RDBAL =[%x]",(*((uint32_t *)(start + E1000_RDBAL))));
(*((uint32_t *)(start + E1000_RDBAH))) = (uint32_t)((((uint64_t)recv_buf_phy)>>32) & 0xffffffff);
//printk("\n E1000_RDBAH =[%x]",(*((uint32_t *)(start + E1000_RDBAH))));
/* (Note that TDT is an index into the transmit descriptor array, not a byte offset; the documentation isn't very clear about this.)*/
(*((uint32_t *)(start + E1000_RDLEN))) = 256;

(*((uint32_t *)(start + E1000_RDH))) = 0;
(*((uint32_t *)(start + E1000_RDT))) = 0;


(*((uint32_t *)(start + E1000_RCTL))) |= E1000_RCTL_EN;
(*((uint32_t *)(start + E1000_RCTL))) &= (~E1000_RCTL_LPE);

/*Loopback Mode (RCTL.LBM) should be set to 00b for normal operation. */

(*((uint32_t *)(start + E1000_RCTL))) &= (~(0x3<<6));  //(E1000_RCTL_LBM_NO);
(*((uint32_t *)(start + E1000_RCTL))) |= (E1000_RCTL_RDMTS_EIGTH);
/*Configure the Multicast Offset (RCTL.MO) bits to the desired value.*/
(*((uint32_t *)(start + E1000_RCTL))) |= (E1000_RCTL_MO_3);
(*((uint32_t *)(start + E1000_RCTL))) |= (E1000_RCTL_BAM);


(*((uint32_t *)(start + E1000_RCTL))) &= (~(0x3<<16));
(*((uint32_t *)(start + E1000_RCTL))) &= (~(0x1<<25));

/* Set the Strip Ethernet CRC (RCTL.SECRC) bit if the desire is for hardware to strip the CRC 
prior to DMA-ing the receive packet to host memory.*/

(*((uint32_t *)(start + E1000_RCTL))) |= (E1000_RCTL_SECRC);


/************************ Transmit Initialization *******************************/

i=0;
for(i=0;i<256;i++){
trans_desc[i].buffer_addr=(uint64_t)(((char *)(trans_buf[i].buf))-0xffffffff80000000);
trans_desc[i].lower.flags.length=100;
trans_desc[i].lower.flags.cmd=25;
}

//printk("trans_desc[0].buffer_addr=[%x] :: trans_buf[0].buf=[%x] :: trans_desc[0]=[%x] :: &trans_buf[0]=[%x] :: &trans_desc[0]=[%x]",trans_desc[0].buffer_addr,trans_buf[0].buf,trans_desc[0], &trans_buf[0],&trans_desc[0]);

uint64_t trans_buf_phy=(uint64_t)(((char *)&trans_desc[0])-0xffffffff80000000); 
//printk("\n TRANS_BUF_PHY =[%x]",trans_buf_phy);
//printk("\n TRANS_DESC_PHY =[%x]",trans_buf_phy);

(*((uint32_t *)(start + E1000_TDBAL))) = (uint32_t)(trans_buf_phy & 0xffffffff);

//printk("\n E1000_TDBAL =[%x]",(*((uint32_t *)(start + E1000_TDBAL))));

(*((uint32_t *)(start + E1000_TDBAH))) = (uint32_t)((((uint64_t)trans_buf_phy)>>32) & 0xffffffff);

//printk("\n E1000_TDBAH =[%x]",(*((uint32_t *)(start + E1000_TDBAH))));

/* (Note that TDT is an index into the transmit descriptor array, not a byte offset; the documentation isn't very clear about this.)*/
(*((uint32_t *)(start + E1000_TDLEN))) = 256;

(*((uint32_t *)(start + E1000_TDH))) = 0;
(*((uint32_t *)(start + E1000_TDT))) = 0;

(*((uint32_t *)(start + E1000_TCTL))) |= (E1000_TCTL_EN);
(*((uint32_t *)(start + E1000_TCTL))) |= (0x100); //(E1000_TCTL_CT)
(*((uint32_t *)(start + E1000_TCTL))) |= (0x40<<12); //(E1000_TCTL_COLD)

(*((uint32_t *)(start + E1000_TIPG))) |= (0xA); 

/********************************** Double enable ********************************/

(*((uint32_t *)(start + E1000_TCTL))) |= (E1000_TCTL_EN);
(*((uint32_t *)(start + E1000_RCTL))) |= E1000_RCTL_EN;

/********************************************************************************/

}


void transmit_packet(struct pci_func *e1000){

	uint64_t start=(uint64_t)(e1000->start_virtual_address);
	int index=(*((uint32_t *)(start + E1000_TDT)));
	//printk("\n E1000_TDT [%d]",index);
	char buf[1512]="shashank shahsank shashank shahsank";
	strcpy(trans_buf[index].buf,buf);
	//printk("\n from tans pack=[%s]",trans_buf[index].buf);
	//printk("\n addr tans pack=[%x]",trans_buf[index].buf);
	//printk("\n addr tans pack=[%x]",trans_buf[index]);
        (*(uint32_t *)(&trans_desc[index]))&=(~E1000_TXD_DTYP_D);
	trans_desc[index].lower.flags.length=100;
	trans_desc[index].lower.flags.cmd=25;
        //printk("\n structure [%x] [%x]",trans_desc[index].lower.flags.length,trans_desc[index].lower.flags.cmd);
	(*((uint32_t *)(start + E1000_TDT)))=index+1;
	//printk("\n E1000_TDT [%d]",(*((uint32_t *)(start + E1000_TDT))));

	//printk("\n STARTING TRANS WHILE LOOP");
	while(!((trans_desc[index].upper.fields.status) & 0x1));
	//printk("\ndesc done");

}

void print_status_of_transmission(struct pci_func *e1000){
	uint64_t start=(uint64_t)(e1000->start_virtual_address);
	int index=(*((uint32_t *)(start + E1000_TDT)));
	printk("\n TDT=[%d]",index);
	//printk("\n TDT=[%d]",(*((uint32_t *)(start + E1000_TDH))));
}
