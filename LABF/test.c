#include "ucode.c"

main(int argc, char *argv[ ])
{
  int i;
  int buf[1];
  STAT fd;
  buf[0]=0;
  //printf("I have successfully added a function\n");

  //printf("argc=%d\n", argc);

  //for (i=0; i<argc; i++){
    //printf("argv[%d]=%s\n", i, argv[i]);
  //}
  if(argc>1)
    stat(argv[1],&fd);
  else
    fstat(1,&fd);
  printf("dev   %d\n",fd.st_dev);
  printf("ino   %d\n",fd.st_ino);
  printf("mode  %d\n",fd.st_mode);
  printf("nlink %d\n",fd.st_nlink);
  printf("uid   %d\n",fd.st_uid);
  printf("gid   %d\n",fd.st_gid);
  printf("rdev  %d\n",fd.st_rdev);
  printf("size  %d\n",fd.st_size);

  printf("the end\n");
}


