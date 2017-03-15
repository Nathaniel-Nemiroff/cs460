/************* wait.c file ****************/

int ksleep(int event)
{
  // YOUR ksleep function
  running->event = event;
  running->status = SLEEP;
  tswitch();
}

int kwakeup(int event)
{
  // YOUR kwakeup function
  int i; PROC *p;
  for(i=1; i<NPROC; i++)
  {
	  p = &proc[i];
	  if(p->status == SLEEP && p->event == event)
	  {
		  p->event = 0;
		  p->status = READY;
		  enqueue(&readyQueue, p);
	  }
  }
}

int kexit(int exitValue)
{
  // YOUR kexit function
  //look for children and kill them?
  int i; PROC *p;
  for(i=1; i<NPROC; i++)
  {
	  p = &proc[i];
	  if(p->ppid == running->pid && p->status != FREE)
		  p->ppid = 1;
  }
  running->status = ZOMBIE;
  running->exitCode = exitValue;
  //wakeup parent
  kwakeup(running->ppid);
  tswitch();
}

int kwait(int *status)
{
  // YOUR kwait function
  //if(caller has no child)
  //	return -1: error
  int i; PROC *p;
  int haschild = 0;
  while(1)
  {
  //	search for any zombie child
	for(i=1; i<NPROC; i++)
	{
		p = &proc[i];
		if(p->ppid == running->pid)
		{
			haschild = 1;
			if(p->status == ZOMBIE)//if we find a zombie child
			{
  				*status = p->exitCode;// copy exitcode into status
				p->status = FREE;
				p->ppid = 0;
				put_proc(freeList, p);
				nproc--;
				return p->pid;
			}
		}
	}
	if(!haschild)//if there are no children, return error
		return -1;
	ksleep(running->pid);// if no children are dead, sleep
  }
}

