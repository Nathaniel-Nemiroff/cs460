#include "ucode.c"

main(int argc, char *argv[])
{
  int pid, i;
  char cmd;
  while(1){
    printf("NCMD # ");
    gets(cmd);
    if(!strcmp(cmd, "logout") || !strcmp(cmd, "exit"))
      exit();
    
    pid=fork();
    if(pid){
      if(!ch4amp(cmd))
	pid = wait(&i);//i now = status
      continue;
    }
    else
      do_pipe(cmd, 0);

    /*if(scan(cmd, head, tail))
      printf("cmd=\"%s\", \"%s\"\n", head, tail);
    else
      printf("cmd=\"%s\"\n", cmd);*/

    //pid=fork();
    //if(pid){
      
  }
}
int do_pipe(char *cmdLine, int *pd)
{
  int i, pid, lpd[2];
  char head[64], tail[64];
  i=0;
  while(i<64)
    head[i]=tail[i++]=0;
  if(pd)//we're a writer here
  {close(pd[0]);dup2(pd[1],1);close(pd[1]);}

  if(scan(cmdLine, head, tail)){
    pipe(lpd);
    pid = fork();
    if(pid){//we're a reader now
      close(lpd[1]);dup2(lpd[0],0);close(lpd[0]);
      do_command(tail);
    }
    else{
      do_pipe(head, lpd);
    }
  }
  do_command(cmdLine);
}
int do_command(char *cmdLine)
{
  int i, pid, rd, pd[2],s=0;
  char head[64], tail[64], temp[1];
  i=0;
  while(i<64)
    head[i]=tail[i++]=0;
  rd=ch4RD(cmdLine, head, tail);
    //printf("debug \"%s\", \"%s\"\n", head, tail);
  if(!rd){
    s=100000;
    while(s--);
    //pause(1);
    exec(cmdLine);
    exit();
  }//1 = out(newfile), 2 = append, 3 = in
  else if(rd==1){
    close(1);
    creat(tail);
    open(tail, O_WRONLY);
  }
  else if(rd==2){
    close(1);
    open(tail, O_RDWR|O_APPEND);
  }
  else if(rd==3){
    close(0);
    open(tail, O_RDONLY);
  }
  /*pipe(pd);
  //pid = fork();
  if(pid){//READER
    
    close(pd[1]);dup2(pd[0],0);close(pd[0]);
  }
  else{   //WRITER
    close(pd[0]);dup2(pd[1],1);close(pd[1]);
  }*/
  s=100000;
  while(s--);
  exec(head);
  exit();
}
int scan(char *cmdLine, char head[], char tail[])
{
  int i=0, j=0;
  char *h = head;
  char *t = tail;
  while(cmdLine[i]){
    *h++ = cmdLine[i];
    if(cmdLine[i++]=='|')
      j=i-1;
  }
  head[i]=0;
  if(j){
    head[j++]=0;
    t = tail;
    while(cmdLine[j]){
      *t++ = cmdLine[j++];
    }
    return 1;
  }
  return 0;
}
int ch4RD(char *cmdLine, char head[], char tail[])
{
  int i=0,j=0,k=0;
  char *h = head;
  char *t = tail;
  while(cmdLine[i]){
    *h++ = cmdLine[i];
    switch(cmdLine[i++]){
      case '>': k=cmdLine[i++]=='>'?2:1;j=i-2;break;
      case '<': k=3;j=i-1;break;
      default : break;
    }
  }
  head[i]=0;
  if(j){
    head[j++]=0;
    t=tail;
    if(k==2)
      j++;
    while(cmdLine[j])
      if(cmdLine[j++]!=' ')
	*t++ = cmdLine[j-1];
    return k;
  }
  return 0;
}
int ch4amp(char *cmdLine)
{
  int i=0;
  while(cmdLine[i])
    if(cmdLine[i++]=='&')
      return i;
  return 0;
}
