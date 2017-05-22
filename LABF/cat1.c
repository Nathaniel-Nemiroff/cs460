#include "ucode.c"

main(int argc, char *argv[])
{
  int rd=0;//FD of our redirect output file(if we have one)
  int chk=0;
  char buf[1],tty[16];
  gettty(tty);
  if(argc>1){
    close(0);
    if(open(argv[1], O_RDONLY))
      exit();
  }
  else
    rd=putcShelper(tty);//check to see if we are directly outputting to the terminal
  while(read(0, buf, 1)){
    if(chk){
      chk=0;
      if(buf[0]=='\r')
	continue;
    }
    if(buf[0]=='\n')
      chk=1;
    putcS(rd,tty,buf[0]);//if we're not, this function will switch write terinal input
  }			 //to both the terminal and the file
  //putcSclose(rd);
  //exit();
}
