#include "ucode.c"

int in, out, err;
main(int argc, char *argv[])
{
  int pfd, gid, uid;//passwd fd
  char *STRTOK, fline[256], user[64], pswd[64];
  // close file descriptors 0 and 1, inhereted from init
  close(0); close(1);
  // open argv[1] 3 times in in(0), out(1), err(2)
  in  = open(argv[1], 0);
  out = open(argv[1], 1);
  err = open(argv[1], 2);
  settty(argv[1]);
  // open /etc/passwd for READ
  pfd = open("/etc/passwd", O_RDONLY);
  printf("please log in\n");

  while(1){
    printf("username: ");
    //get username
    gets(user);
    printf("password: ");
    // get password
    gets(pswd);
    while(fGetLine(pfd, fline, 256)){
    //user format:
    //username:password:gid:uid:fullname:HOMEDIR:program
      STRTOK = strtok(fline, ":");

      if(!strcmp(user, STRTOK)){
	STRTOK = strtok(NULL, ":");
	if(!strcmp(pswd, STRTOK)){
	  gid=atoi(strtok(NULL, ":"));
	  uid=atoi(strtok(NULL, ":"));
	  chuid(uid, gid);
	  STRTOK=strtok(NULL, ":");
	  chdir(STRTOK);
	  close(pfd);
	  exec("sh");
	}
      }
    }
    close(pfd);
    pfd = open("/etc/passwd", O_RDONLY);
    printf("login failed, try again\n");
  }
}
int fGetLine(int fd, char fline[], int n)
{
  int i = 0;
  char *fn = fline;
  while(read(fd, fn++, 1)&&fline[i]!='\n'&&++i<n);
  if(i>n){
    printf("ERROR IN LOGIN FILE READ\n");
    return 0; 
  }
  return i;
}
