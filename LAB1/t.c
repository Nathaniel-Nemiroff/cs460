/** t.c **/

#include "ext2.h"

struct partition {
	u8 drive;
	u8 head;
	u8 sector;
	u8 cylinder;
	u8 type;
	u8 end_head;
	u8 end_sector;
	u8 end_cylinder;
	u32 start_sector;
	u32 nr_sectors;
};
struct dap{
	u8 len;
	u8 zero;
	u16 nsector;
	u16 addr;
	u16 segment;
	u32 s1;
	u32 s2;
};

int prints(char *s)
{
	//putc
	char *c = s;
	while(*c)
	{
		putc(*(c++));
	}
}
int putd(char c)
{putc(c+48);}
int gets(char *s)
{
	//getc
	char *c = s;
	char g;
	while((g = getc())!='\r'){
		putc(g);
		*(c++) = g;
	}
	putc('\n');
	putc('\r');
	*c = '\0';
}


struct dap dap, *dp;
char mbr[512];
char ans[64];

int getSector(u32 sector, char *addr)
{
	dp->addr = addr;
	dp->s1 = (u32)sector;
	diskr();
}

main() {
	char name[64];
	char buf[2048];
	int i = 1;
	struct partition *p;

	dp = &dap;
	dp->len = 0x10;
	dp->zero = 0;
	dp->nsector = 1;
	dp->addr = 0;
	dp->segment = 0x9000;
	dp->s1 = 0;
	dp->s2 = 0;

	getSector((u32)100, (u16)mbr);

	while(1){
		prints("What's your name? ");
		gets(name);
		if(name[0]==0)
			break;
		prints("Welcome "); prints(name); prints("\n\r");
	}/*
	for(i=0;i<1000;i++){
		get_block(i, buf);
		getc();
		prints(buf);
	}*/
	prints("return to assembly and hang\n\r");

	while(i < 4)
	{
		p = mbr + 0x1BE + (i++ * 16);

		putd(i);
		putc(' ');
		if((u8)p->type == 144)
			prints("MTX  ");
		else
			prints("UNKN ");
		putd((u16)p->start_sector);
		putc(' ');
		putd((u16)p->nr_sectors);
		putc('\n');
	}
}

get_block(int blk, char *buf)
{
	// convert blk to (cyl, head, sector)

	readfd(blk/18, ((2*blk)%36)/18, ((2*blk)%36)%18, buf);
}
