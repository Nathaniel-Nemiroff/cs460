#include "ucode.c"

main(int argc, char *argv[])
{
  int rd=0,chk=0;
  char buf[1],tty[16];
  gettty(tty);
  if(argc<3)
  {printf("USAGE: cp f1 f2\n");return 0;}
  printf("Copying %s into %s...\n",argv[1],argv[2]);
  close(0);
  close(1);
  open(argv[1],O_RDONLY);
  creat(argv[2]);
  open(argv[2],O_WRONLY);

rd=putcShelper(tty);
  while(read(0,buf,1)){
    if(buf[0]!='\r')
    write(rd,&buf[0],1);
  }
}
