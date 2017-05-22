#include "ucode.c"

main(int argc, char *argv[])
{
  int rd=0,i=0,j=0,k=0,ln=0,r=0,chk=0;
  char buf[1],tty[16],line[80],*l=line,tok[80];
  STAT fdstat,ttystat;
  gettty(tty);
  if(argc>1){
    if(argc<3){
      printf("usage: l2u f1 f2\n");
      return 0;
    }
    close(0);close(1);
    open(argv[1],O_RDONLY);
    creat(argv[2]);
    open(argv[2],O_WRONLY);
  }
  else
  { rd=putcShelper(tty);
    fstat(0,   &fdstat);
     stat(tty,&ttystat);
    if(fdstat.st_ino==ttystat.st_ino)
	r=1;
  }

  while(read(0,buf,1)){
    if(buf[0]>96 && buf[0]<123)
      buf[0]=buf[0]-32;
   // putcS(rd,tty,buf[0]);
    if(rd){
      putcS(rd,tty,buf[0]);}//write(rd,buf,1);}
    else{// if(buf[0]=='\r')
      if(buf[0]!='\r')
        putc(buf[0]);}
  }
}
