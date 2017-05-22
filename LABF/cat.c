#include "ucode.c"

main(int argc, char *argv[])
{
  int rd=0;
  int chk=0;
  char buf[1],tty[16];
  gettty(tty);
  if(argc>1){
    close(0);
    if(open(argv[1],O_RDONLY))
      exit();
  }
  //else
    rd=putcShelper(tty);
  while(read(0,buf,1)){
    if(chk){
      chk=0;
      if(buf[0]=='\r')
        continue;
    }
    if(buf[0]=='\n')
      chk=1;
    putcS(rd,tty,buf[0]);

  }
}
