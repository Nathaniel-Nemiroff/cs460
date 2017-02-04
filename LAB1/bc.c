typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define GREEN  10
#define CYAN   11
#define RED    12
#define MAG    13
#define YELLOW 14

struct partition{
	u8  drive;
	u8  head;
	u8  sector;
	u8  cylinder;
	u8  type;
	u8  end_head;
	u8  end_sector;
	u8  end_cylinder;
	u32 start_sector;
	u32 nr_sectors;
};

struct dap{
	u8  len;
	u8  zero;
	u16 nsector;
	u16 addr;
	u16 segment;
	u16 s1;
	u16 s2;
};

struct dap dap, *dp;
u16 color = RED;

#define BOOTSEG 0x9000

// #include "bio.c"

char mbr[512];
char ans[64];

// load a disk sector to (DS, addr) where addr is the offset
int getSector(u32 sector, char *addr)
{
	dp->addr = addr;
	dp->s1   = (u32)sector;
	diskr();  // call int13-43 in assembly
	// NEED ANOTHER BS.S TO SUPPORT THIS
}

int main()
{
	int i;
	struct partition *p;
	printf("booter start in main()\n");

	// initialize the dap struct
	dp = &dap;
	dp->len     = 0x10;
	dp->zero    = 0;
	dp->nsector = 1;
	dp->addr    = 0;
	dp->segment = BOOTSEG;
	dp->s1      = 0;
	dp->s2      = 0;

	getSector((u32)0, (u16)mbr);
}


