#include "type.h"

PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList;
int procSize = sizeof(PROC);
int nproc = 0;

int body();
char *pname[]={"Sun", "Mercury", "Venus", "Earth",  "Mars", "Jupiter", 
               "Saturn", "Uranus", "Neptune" };

/**************************************************
  bio.o, queue.o loader.o are in mtxlib
**************************************************/
/* #include "bio.c" */
#include "queue.c"
/* #include "loader.c" */

/*
u8 get_byte(u16 segment, u16 offset)
{
	u8 byte;
	u16 ds = getds();
	setds(segment);
	byte = *(u8 *)offset;
	setds(ds);
	return byte;
}
void put_byte(u8 byte, 16 segment, u16 offset)
{
	u16 ds = getds();
	setds(segment);
	*(u8 *)offset = byte;
	setds(ds);
}
*/

u16 get_word(u16 segment, u16 offset)
{
	u16 retval = 0;
	retval = (get_byte(segment, offset+1)&0xFF) * 0x100;
	retval += (get_byte(segment, offset)&0xFF);
	return retval;
}
int put_word(u16 WORD, u16 segment, u16 offset)
{
	u8 HIGH = 0;
	u8 LOW  = 0;
	HIGH    = WORD / 0x100;
	LOW     = WORD & 0xFF;
	put_byte(HIGH, segment, offset+1);
	put_byte(LOW , segment, offset);
}

#include "wait.c"             // YOUR wait.c   file
#include "kernel.c"           // YOUR kernel.c file
#include "int.c"              // YOUR int.c    file
int color;

int init()
{
    PROC *p; int i;
    color = 0x0C;
    printf("init ....");
    for (i=0; i<NPROC; i++){   // initialize all procs
        p = &proc[i];
        p->pid = i;
        p->status = FREE;
        p->priority = 0;  
        strcpy(proc[i].name, pname[i]);
        p->next = &proc[i+1];
    }
    freeList = &proc[0];      // all procs are in freeList
    proc[NPROC-1].next = 0;
    readyQueue = sleepList = 0;

    /**** create P0 as running ******/
    p = get_proc(&freeList);
    p->status = RUNNING;
    p->ppid   = 0;
    p->parent = p;
    running = p;
    nproc = 1;
    printf("done\n");
} 

int scheduler()
{
    if (running->status == READY)
        enqueue(&readyQueue, running);
     running = dequeue(&readyQueue);
     color = running->pid + 0x0A;
}

int int80h();
int set_vector(u16 vector, u16 handler)
{
     // put_word(word, segment, offset)
     put_word(handler, 0, vector<<2);
     put_word(0x1000,  0,(vector<<2) + 2);
}
            
main()
{
    printf("MTX starts in main()\n");
    init();      // initialize and create P0 as running
    set_vector(80, int80h);

    kfork("/bin/u1");     // P0 kfork() P1

    while(1){
      printf("P0 running\n");
      while(!readyQueue);
      printf("P0 switch process\n");
      tswitch();         // P0 switch to run P1
   }
}

