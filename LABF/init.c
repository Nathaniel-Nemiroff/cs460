#include "ucode.c"

int console;
int p1, p2, p3;
int spin;
main(int argc, char *argv[])
{
  int in, out;
  in  = open("/dev/tty0", O_RDONLY);
  out = open("/dev/tty0", O_WRONLY);
  printf("NEMIROFF's INIT\n");

  p1 = fork();
  if(p1){
    p2 = fork();
    if(p2){
      p3 = fork();
      if(p3)
      	parent();
      else{
	printf("\r");
        spin = 30000000;
        while(spin--);
	exec("login /dev/ttyS1");
      }
    }
    else{
      printf("\r");
      spin = 30000000;
      while(spin--);
      exec("login /dev/ttyS0");
    }
    parent();
  }
  else{
    exec("login /dev/tty0");
  }
  return 0;
}
int parent()
{
  int pid, status;
  char *tty;
  while(1){
    printf("NEWINIT : wait for ZOMBIE child\n");
    pid = wait(&status);

         if(pid==p1)
      {printf("p1\n");tty="login /dev/tty0"; p1=fork();console=p1;}
    else if(pid==p2)
      {printf("p2\n");tty="login /dev/ttyS0";p2=fork();console=p2;}
    else if(pid==p3)
      {printf("p3\n");tty="login /dev/ttyS1";p3=fork();console=p3;}
    else
      {printf("NEWINIT: buried orphan P%d...\n", pid); continue;}

    printf("NEWINIT: forks a new console login\n");
    if(console)
      continue;
    else
      exec(tty);
    /*if(pid == console){
      printf("NEWINIT: forks a new console login\n");
      console = fork();
      if(console)
	continue;
      else
	exec("login /dev/tty0");
    }*/
    //printf("NEWINIT: I just buried an orphan child proc %d\n", pid);
    printf("Something weird happens here\n");
  }
}
