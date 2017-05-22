struct semaphore{
  //byte lock;
  int value;
  PROC *queue;
};

int SEMBLK(struct semaphore *s)
{
	running->status = BLOCK;
	enqueue(&s->queue, running);
	//s->lock = 0;
	tswitch();
	int_on();
}
int SEMSIG(struct semaphore *s)
{
	PROC *p;
	p = dequeue(&s->queue);
	p->status = READY;
	enqueue(&readyQueue, p);
}
int P(struct semaphore *s)
{
  // write YOUR C code for P()
/*	enterCR(&s->lock);
	s->value--;
	if(s->value < 0)
	   BLOCK(s);
	else exitCR(&s->lock);*/
	s->value--;
	if(s->value<0) SEMBLK(s);
}

int V(struct semaphore *s)
{
  // write YOUR C code for V()
	/*enterCR(&s->lock);
	s->value++;
	if(s->value <=0)
	   SIGNAL(s);
	exitCR(&s->lock);*/
	s->value++;
	if(s->value<=0) SEMSIG(s);
}
/*
int enterCR(byte *lock)
{
	int_off;
	while(TS(*lock));//???
}
int exitCR(byte *lock)
{
	*lock = 0;
	int_on;
}
*/
