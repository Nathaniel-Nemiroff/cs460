#include "ucode.c"

main(int argc, char *argv[])
{
  int pid;
  while(1){
    //get a command line: cmd | cmd 2 | cmd 3...
    // get cmd token from command line
    //if(cmd = cd || logout || su){//built-in commands
      //do cmd directly;
      //continue;
    //}

    pid=fork();
    if(pid){
      if(no & symbol)//???? 
        pid = wait(&status);
      continue;
    }
    else
      do_pipe(cmd_line,0);
  }
}
int do_pipe(char *cmdLine, int *pd)
{
  if(pd){//if has a pipe passed in, as writer on pipe pd:
    close(pd[0]); dup2(pd[1],1); close(pd[1]);
  }
  //divide cmdLine into head and tail by rightmost pipe symbol
  hasPipe = scan(cmdLine, head, tail);
  if(hasPipe){
    create a pipe lpd;//????
    pid = fork();
    if(pid){
      //as reader on lpd;
	close(lpd[1]); dpu2(lpd[0],0); close(lpd[0]);
	do_command(tail);
    }
    else
      do_command(cmdLine);
}
int do_command(char *cmdLine)
{
  scan cmdLine for I/O redirection symbols
  do I/O redirections;
  head = cmdLine BEFORE redirections
  exec(head);
}
int scan(char *cmdLine, char *head, char *tail)
{
  divide cmdLine into head and tail by rightmost | symbol
  cmdLine = cmd1 | cmd2 | ...
  head = cmd1 - cmdn-1, tail = cmdn; return 1
  cmdLine = cmd1 ==> head=cmd1, tail=null; return 0;
}
