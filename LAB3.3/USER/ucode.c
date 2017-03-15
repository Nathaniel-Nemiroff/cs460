// ucode.c file

char *cmd[]={"getpid", "ps", "chname", "kfork", "switch", "wait", "exit","fork", "exec", "pipe", "read", "write", "close", "pfd", 0};

#define LEN 64

int show_menu()
{
   printf("***************** Menu *********************************\n");
   printf("*ps chname kfork switch wait exit fork exec and pipe...*\n");
   printf("********************************************************\n");
}
int show_menu_u2()
{
   printf("********************* Menu ****************************\n");
   printf("* ps  chllama  kfork  cambio  espera  salida fork ejec*\n");
   printf("*******************************************************\n");
 
}

int find_cmd(char *name)
{
  // return command index
	int i = 0;
	while(strcmp(cmd[i++],name));
	return i-1;
}

int getpid()
{
   return syscall(0,7,0);
}

int ps()
{
   return syscall(1, 9, 0);
}

int chname()
{
    char s[32];
    printf("input new name : ");
    gets(s);
    return syscall(2, s, 0);
}

int kfork()
{   
  int child, pid;
  pid = getpid();
  printf("proc %d enter kernel to kfork a child\n", pid); 
  child = syscall(3, 0, 0);
  printf("proc %d kforked a child %d\n", pid, child);
}    

int kswitch()
{
    return syscall(4,0,0);
}

int wait()
{
    int child, exitValue;
    printf("proc %d enter Kernel to wait for a child to die\n", getpid());
    child = syscall(5, &exitValue, 0);
    printf("proc %d back from wait, dead child=%d", getpid(), child);
    if (child>=0)
        printf("exitValue=%d", exitValue);
    printf("\n"); 
} 

int geti()
{
  // return an input integer
}

int exit()
{
   int exitValue;
   printf("enter an exitValue: ");
   exitValue = getc();
   printf("\nenter kernel to die with exitValue=%c\n", exitValue);
   syscall(6, exitValue, 0);
   //_exit(exitValue);
}

int fork()
{
	return syscall(7, 0, 0);
}
int exec()
{
	char s[32];
	printf("input cmd: ");
	gets(s);

	return syscall(8, s, 0);	
}

int pipe()
{
	return syscall(30, 0, 0);
}
int read()
{
	int i, j;
	char *inp;
	char *s;
	printf("read from which fd? ");
	i = (getc()&0xFF)-48;
	putc(i+48);putc('\n');putc('\r');
	printf("how much? ");
	gets(inp);
	j = atoi(inp);
	printf("%d\n", j);
	syscall(31, i, s, j);
	printf("PROC %d reads:'%s'\n",getpid(), s);
	return 0;
}
int write()
{
	int i;
	char imp[64];
	char *buf=&imp;
	printf("write to which fd? ");
	i = (getc()&0xFF)-48;
	printf("write what? ");
	gets(imp);
	strcpy(buf, imp);
	printf("writing %s...\n", buf);
	return syscall(32, i, imp);
}
int close()
{
	int i;
	printf("close which? ");
	i = getc() - 48;
	printf("(got %d\n", i);
	return syscall(33, i, 0);
}
int pfd()
{
	return syscall(34, 0, 0);
}

int _exit(int exitValue)
{
  return syscall(6,exitValue,0);
}

int invalid(char *name)
{
    printf("Invalid command : %s\n", name);
}

