typedef unsigned char   u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define NPROC   16
#define SSIZE 1024

#define PSIZE     20
#define NFD        2
#define WRITE_PIPE 1
#define READ_PIPE  2

/******* PROC status ********/
#define FREE     0
#define READY    1
#define RUNNING  2
#define STOPPED  3
#define SLEEP    4
#define ZOMBIE   5

typedef struct pipe{
	char buf[PSIZE];
	int head, tail;
	int data, room;
	int nreaders, nwriters;
}PIPE;

typedef struct oft{
	int mode;
	int refCount;
	PIPE *pipe_ptr;
}OFT;

typedef struct proc{
    struct proc *next;
    int    *ksp;               // at offset 2

    int    uss, usp;           // at offsets 4,6
    int    inkmode;            // at offset 8

    int    pid;                // add pid for identify the proc
    int    status;             // status = FREE|READY|RUNNING|SLEEP|ZOMBIE    
    int    ppid;               // parent pid
    struct proc *parent;
    int    priority;
    int    event;
    int    exitCode;
    char   name[32];           // name string of PROC
    OFT    *fd[NFD];

    int    kstack[SSIZE];      // per proc stack area
}PROC;


