#include "ucode.c"

main(int argc, char *argv[])
{
  int in=0, out=1,rd=0,i=0;
  char buf[1],tty[16];
  STAT fdstat,ttystat;
  gettty(tty);
  if(argc>1){
    if(argc<3){
      printf("usage: l2u f1 f2\n");
      exit();
    }
    close(0);close(1);
    in=1;
    open(argv[1],O_RDONLY);
    creat(argv[2]);
    out = open(argv[2],O_WRONLY);
    if(out==-1)
      exit();
  } 
  else
    rd=putcShelper(tty);

  fstat(0,   &fdstat);
   stat(tty,&ttystat);
  if(fdstat.st_ino!=ttystat.st_ino)
    i=1;

  while(read(0,buf,1)){
    if(buf[0]>96 && buf[0]<123)
      buf[0]=buf[0]-32;
    if(buf[0]!=13)
      putcS(rd,tty,buf[0]);
  }
  putcSclose(rd);
  //close(1);
  //open(tty,O_RDWR);
  //printf("done\n");
  //exit();
}
