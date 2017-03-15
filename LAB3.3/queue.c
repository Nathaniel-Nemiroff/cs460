extern PROC *readyQueue, *freeList;

/****************** utility.c file ***********************/
PROC *get_proc(PROC **list)  // allocate a FREE proc from freeList
{// return pointer from list
	PROC *retproc;
	if(!list)
	{
		printf("NO MORE PROCS IN LIST\n");
		return 0;
	}
	retproc = list[0];
	*list = list[0]->next;
	retproc->next = 0;
	return retproc;
}

int put_proc(PROC **list, PROC *p)   // return a PROC to freeList
{//put p into list
	int i = 0;
	PROC *iter = list[0];
	if(!list)
		*list = p;
	while(iter->next){iter = iter->next;}
	p->next = 0;
	iter->next = p;
}

// enter a PROC into queue by priority
int enqueue(PROC **queue, PROC *p)
{//put p into queue by priority (high is first, low is last)
	int i = 0;
	PROC *iter = queue[0];
	if(!queue[0])
	{
		p->next = 0;
		*queue = p;
		return;
	}
	if(iter->priority < p->priority)
	{
		p->next = iter;
		*queue = p;
		return;
	}
	while(iter->next)
	{
		if(iter->next->priority < p->priority)
		{
			p->next = iter->next;
			iter->next = p;
			return;
		}
		iter = iter->next;
	}
	iter->next = p;
}

// remove and return first PROC in queue 
PROC *dequeue(PROC **queue)
{//straight forward
	return get_proc(queue);
}

// print queue and print list functions 
int printSet(char *name, PROC *p, int type)
{//straight forward
	PROC *print = p;
	printf("%s ", name);
	while(print){
		printf("--> %d", print->pid);
		if(type)
			printf("[%d] ", print->priority);
		print = print->next;
	}
	printf("-> NULL\n");
}
int printList(char *name, PROC *p)
{
	printSet(name, p, 0);
}
int printQueue(char *name, PROC *p)
{//same as printlist
	printSet(name, p, 1);
}


