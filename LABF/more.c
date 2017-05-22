#include "ucode.c"

main(int argc, char *argv[])
{
  int rd=0,i=0;
  char buf[1],tty[16];
  gettty(tty);
  printf("THIS IS ACTUALLY MY MORE FUNC\n");
  if(argc>1){
    close(0);
    i=open(argv[1],O_RDONLY);
    rd=dup(i);
    if(i)
      exit();
  }
  else
    rd=dup(0);
  //else
    //rd=putcShelper(tty);
  while(read(0,buf,1)){
    if(buf[0]=='\n'&&i++>24){
      close(0);open(tty,O_RDONLY);
      getc();
      close(0);dup(rd);
      putc('\r');
      putc('\n');
      continue;
    }
    putc(buf[0]);
    //putcS(rd,tty,buf[0]);
  }
}
