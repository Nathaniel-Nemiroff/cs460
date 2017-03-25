/******************** timer.c file *************************************/
#define LATCH_COUNT     0x00	   /* cc00xxxx, c = channel, x = any */
#define SQUARE_WAVE     0x36	   /* ccaammmb, a = access, m = mode, b = BCD */

#define TIMER_FREQ   1193182L	   /* clock frequency for timer in PC and AT */
#define TIMER_COUNT  TIMER_FREQ/60 /* initial value for counter*/

#define TIMER0       0x40
#define TIMER_MODE   0x43
#define TIMER_IRQ       0

u16 tick;
u16 currtime;//current (system) time
int timedswitch;
int defaultproctime;

int enable_irq(u16 irq_nr)
{
  lock();
    out_byte(0x21, in_byte(0x21) & ~(1 << irq_nr));

}

int timer_init(int i, int j)
{
  /* Initialize channel 0 of the 8253A timer to e.g. 60 Hz. */

  printf("timer init\n");
  tick = 0; 
  currtime = 0;
  timedswitch    =i;
  defaultproctime=j;
  out_byte(TIMER_MODE, SQUARE_WAVE);	// set timer to run continuously
  out_byte(TIMER0, TIMER_COUNT);	// timer count low byte
  out_byte(TIMER0, TIMER_COUNT >> 8);	// timer count high byte 
  enable_irq(TIMER_IRQ); 
}

// inturrupt timer
int pause(int t)
{
   itimer(t);
   ksleep(running->pid);
}
// assigned itimer function
int itimer(int time)
{
// 1 fill in TQE[pid] information, e.g. proc pointer, action.
   int t = time;
   TQE *curr = 0;
   int pid = running->pid;
   int total;
   tqe[pid].next = 0;
   tqe[pid].time = t;
   tqe[pid].data = running;
   tqe[pid].action = 0;
   lock();
// 3 traverse timer queue to compute the position to insert tqe;
   if(TQ==0||TQ->time > t){
      tqe[pid].next = TQ;
      TQ = &tqe[pid];
      if(TQ->next)
	TQ->next->time-=t;
   }
   else
   {
      curr = TQ;
      total = curr->time;
      while(curr->next!=0){
	 total+=curr->next->time;
	 if(total>t)
	 {
	    tqe[pid].time = t-(total-curr->next->time);//-curr->next->time);
	    curr->next->time-=tqe[pid].time;
	    break;
	 }
         curr=&curr->next;
      }
// 4 insert the tqe and update the time of the next tqe
      tqe[pid].next = curr->next;
      curr->next = &tqe[pid]; 
    }
   unlock();
   ksleep(&tqe[pid].data->pid);
}

// added function printTime() and printTQE
printTQE()
{
   int i = 22;
   TQE *curr = TQ;
   while(curr)
   {
     printAtLoc(                    "[P", 71, i);
     putcAtLoc (  curr->data->pid    +48, 73, i);
     printAtLoc(                    ", ", 74, i);
     putcAtLoc (((curr->time/100)%10)+48, 76, i);
     putcAtLoc (((curr->time/10 )%10)+48, 77, i);
     putcAtLoc (( curr->time     %10)+48, 78, i);
     putcAtLoc (                     ']', 79, i);
     //putcAtLoc((&curr+48)%10, 79, 3);
     //curr=0;//
     curr = curr->next;
     i--;
   }
   printAtLoc("Timer Queue", 69, i);
}
printTime()
{ 
  int i,j;
  for(i=0;i<25;i++)
     for(j=69;j<80;j++)
	putcAtLoc(0, j, i);
  putcAtLoc(            'P', 69, 23);
  putcAtLoc(running->pid+48, 70, 23);
  putcAtLoc(            ' ', 71, 23);

  if(running->inkmode > 1)
     putcAtLoc('K', 72, 23);
  else
     putcAtLoc('U', 72, 23);
  printAtLoc("mode ", 73, 23);
  putcAtLoc((running->time/10)+48, 78, 23);
  putcAtLoc((running->time%10)+48, 79, 23);

  putcAtLoc( ((currtime/3600)    /10)+48, 72, 24);
  putcAtLoc( ((currtime/3600)    %10)+48, 73, 24);
  putcAtLoc(                         ':', 74, 24);
  putcAtLoc((((currtime%3600)/60)/10)+48, 75, 24);
  putcAtLoc((((currtime%3600)/60)%10)+48, 76, 24);
  putcAtLoc(                         ':', 77, 24);
  putcAtLoc( ((currtime%  60)    /10)+48, 78, 24);
  putcAtLoc( ((currtime%  60)    %10)+48, 79, 24);

  printTQE();
}

/*===================================================================*
 *		    timer interrupt handler       		     *
 *===================================================================*/
int thandler()
{
  int is = 0;//inturrupt switch
  tick++; 
  tick %= 60;
  
  if (tick == 0){                      // at each second
     currtime++;
     printTime();
    //printf("1s itr in %d:", running->pid);
      if(running->inkmode <= 1){
	if(running->time <= 0)
	   is = 1;
	else
	   running->time--;
      }
      //printf("mode : %ds left...", running->time);
      if(TQ){
      if(TQ->time < 1){
	kwakeup(&TQ->data->pid);
	if(TQ->next)
	   TQ = TQ->next;
	else
	   TQ = 0;
      }
      TQ->time--;
      }
  }
  out_byte(0x20, 0x20);                // tell 8259 PIC EOI
  if(is)
	tswitch();
}

