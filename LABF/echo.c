#include "ucode.c"

main(int argc, char *argv[])
{
  char buf[64];
  if(argc>1){
    close(0);
    open(argv[1], O_RDONLY);
  }
  read(0, buf, 64);
  printf("%s\n",  buf);
  exit();
}
