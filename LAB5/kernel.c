/****************** kernel.c file *************************/
int body();

//ADD kfork(char *filename)

PROC *kfork(char *filename)
{
      PROC *p = get_proc(&freeList);
      int i;
      if (p==0){
         printf("no more PROC, kfork() failed\n");
         return -1;
      }
      p->status = READY;
      p->inkmode = 1;
      //p->priority = 1;          // priority = 1 for all proc except P0
      p->ppid = running->pid;   // parent = running
      p->parent = running;

      for (i=1; i<10; i++)
	  p->kstack[SSIZE-i]= 0 ;
      p->kstack[SSIZE-1] = (int)body;
      p->ksp = &(p->kstack[SSIZE-9]);

      p->uss = (p->pid+1)*0x1000;
      p->usp = -24;

	printf("loading file %s\n", filename);
	if(filename){
		if(load(filename, p->uss)==-1)
			return -1;

	printf("loaded successfully into %d\n", p->pid);
	}
      enqueue(&readyQueue, p);  // enter p into readyQueue by priority
      nproc++;
      put_word(p->uss, p->uss, -12*2);
      put_word(p->uss, p->uss, -11*2);
      put_word(0, p->uss, -10*2);
      put_word(0, p->uss, -9*2);
      put_word(0, p->uss, -8*2);
      put_word(0, p->uss, -7*2);
      put_word(0, p->uss, -6*2);
      put_word(0, p->uss, -5*2);
      put_word(0, p->uss, -4*2);

      put_word(0, p->uss, -3*2);
      put_word(p->uss, p->uss, -2*2);
      put_word(0x0200, p->uss, -1*2);

      return p;                 // return child PROC pointer
/*
1) do the same as kfork to create a child proc in such a way that when
the child starts to run (in kmode) it resumes to goUmode

2) determine the child's segment as u16 child_segment
	set child's PROC.uss = child_segment, and
		    PROC.usp = running->usp (see below for why)

	but do NOt load any umode image (file) to child's segment
	because it will have the same umode image of its parent
	(the running proc)

3) use get_word/put_word to implement a "copy_image(child_segment)
	function, which copies the umode image of running to
	child_segment

	copy_image implies that the umode image of parent and child
	are IDEnTICAL, so their saved usp must also be the same value
	(each is an offset relative to their own segment)

4) copy image
	
*/
}
int copyImage(u16 pseg, u16 cseg, u16 size)
{
   u16 i;
   for(i=0;i<size;i++)
	put_word(get_word(pseg, 2*i), cseg, 2*i);
}
int fork()
{
	int pid; u16 segment;
	PROC *p = kfork(0);
	if(p<1) return -1;
	segment = (p->pid+1)*0x1000;
	copyImage(running->uss, segment, 32*1024);
	p->uss = segment;
	p->usp = running->usp;
	
	put_word(segment, segment, p->usp);//uDS=seg
	put_word(segment, segment, p->usp+2);//uES=seg
	put_word(      0, segment, p->usp+2*8);//uax=0
	put_word(segment, segment, p->usp+2*10);//uCS=seg
	printf("FORK: %d %d\n", get_word(running->uss, p->usp+2*12), get_word(p->uss, p->usp+2*12));
	return p->pid;
}

int kexec(char *y)
{
	int i, length = 0, argc;
	char filename[64], *cp = filename;
	char *pch;
	char *argv[10];
	
	u16 segment = running->uss;
	while((*cp++ = get_byte(running->uss, y++))&&length++ < 64);
	//while((*cp++ = *y++)!=' ' && length++ < 64);
	//*(cp-1) = 0;
	pch = strtok(filename, " ");
	argc = 0;
	while(pch != 0)
	{
		argv[argc++]=pch;
		//strcpy(argv[i++], pch);
		pch = strtok(0, " ");
	}
	printf("cmd     :%s\n", argv[0]);
	for(i=1;i<argc;i++){
		printf("argv[%d]:%s\n", i, argv[i]);
	}
		
	if(load(filename, segment)==-1){
		printf("failed\n");
		return -1;
	}
	printf("success!\n");
	for(i=1;i<=12;i++)
		put_word(0, segment, -2*i);
	running->usp = -24;
	
	put_word(segment, segment, -2*12);
	put_word(segment, segment, -2*11);
	put_word(segment, segment, -2*2);
	put_word(0x0200 , segment, -2*1);
}

int do_tswitch()
{
  printf("proc %d tswitch()\n", running->pid);
  tswitch();
  printf("proc %d resumes\n", running->pid);
}

int do_kfork()
{
  PROC *p;
  printf("proc %d kfork a child\n", running->pid);
  p = kfork("/bin/u1");
  if (p==0)
    printf("kfork failed\n");
  else
    printf("child pid = %d\n", p->pid);
}

int do_exit()
{
  int exitValue;
  if (running->pid == 1 && nproc > 2){
      printf("other procs still exist, P1 can't die yet !%c\n",007);
      return -1;
  }
  printf("enter an exitValue (0-9) : ");
  exitValue = (getc()&0x7F) - '0'; 
  printf("%d\n", exitValue);
  kexit(exitValue);
}

int do_wait()
{
  int child, status;
  child = kwait(&status);
  if (child<0){
    printf("proc %d wait error : no child\n", running->pid);
    return -1;
  }
  printf("proc %d found a ZOMBIE child %d exitValue=%d\n", 
	   running->pid, child, status);
  return child;
}
int do_sleep()
{
	int i;
	printf("input an event to sleep on: ");
	i = getc();
	printf("%c\nproc %d going to sleep on event %c\n", i, running->pid, i);
	ksleep(i);
}
int do_wakeup()
{
	int i;
	printf("input a value to wakeup on: ");
	i = getc();
	printf("%c\nwakeup procs sleeping on event %d\n", i, i);
	kwakeup(i);
}
int do_goUmode()
{ 
	printf("doing go umode at segment = %x...\n", running->uss);
	goUmode();
}
int body()
{
  char c;
  printf("proc %d resumes to body()\n", running->pid);
  while(1){
    printf("-----------------------------------------\n");
    printList("freelist  ", freeList);
    //printQueue("readyQueue", readyQueue);
    //printList("sleepList ", sleepList);//printSleepList()
    printf("-----------------------------------------\n");

    printf("proc %d running: parent=%d\n",
	   running->pid, running->ppid);

    printf("enter a char [s|f|q|w|u|z|a] : ");
    c = getc(); printf("%c\n", c);
    switch(c){
       case 's' : do_tswitch();   break;
       case 'f' : do_kfork();     break;
       case 'q' : do_exit();      break;
       case 'w' : do_wait();      break;
       case 'u' : do_goUmode();	  break;
	   case 'z' : do_sleep();	  break;
	   case 'a' : do_wakeup();	  break;
    }
  }
}

int kps()
{
	char *stat = "0";
	switch(running->status){
		case 0: stat = "FREE";   break;
		case 1: stat = "READY";  break;
		case 2: stat = "SLEEP";  break;
		case 3: stat = "ZOMBIE"; break;
		case 4: stat = "BLOCK";  break;
	}
	printf("PROC %d STATUS : %s\n", running->pid, stat);
	return running->status;
}
int kmode()
{
	printf("FUG IUNO\n");
}
int chname()
{
	printf("chname func\n");
}
int printList()
{
	printf("printlist\n");
}
