#include "ucode.c"

main(int argc, char *argv[])
{
  int rd=0,i=0,j=0,k=0,ln=0,r=0;
  char buf[1],tty[16],line[80],*l=line,tok[80];
  STAT fdstat,ttystat;
  gettty(tty);
  if(argc<2)
  {printf("USAGE: grep [pattern] [file]\n");return 0;}
  strcpy(tok,argv[1]);
  if(argc>=3){
    close(0);
    if(open(argv[2],O_RDONLY))
      exit();
  }
  else
  { rd=putcShelper(tty);
    fstat(0,   &fdstat);
     stat(tty,&ttystat);
    if(fdstat.st_ino==ttystat.st_ino)
	r=1;
  }

  for(ln=0;ln<80;ln++)
    if(!tok[ln])
      break;
  j=ln;
  ln=0;

printf("Greping input...\n");
  while(read(0,buf,1)){
    if(r){
      //putcS(rd,tty,buf[0]);
      if(buf[0]==13)
	putc('\n');
      putc(buf[0]);
    }
    if(buf[0]=='\n'||buf[0]==13){
      if(k){
        if(rd)
	{
	 //close(1);dup(rd);
	  line[ln]='\n';
	  //printf("%s",line);
	  write(rd,line,ln+1);
	 //close(1);open(tty,O_RDWR);}
        }
	else{
	  printf("%s\n", line);
	}
      }
      for(ln=0;ln<80;ln++)
	line[ln]=0;
      k=0; 
      i=0;
      ln=0;
      continue;
    }
    line[ln++]=buf[0];
    if(i<j&&buf[0]==tok[i]){
      i++;
      if(i==j)
        k=1;
    }
    else
      i=0;
  }
  putcSclose(rd);
}
