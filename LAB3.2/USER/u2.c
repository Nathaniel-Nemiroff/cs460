#include "ucode.c"
int color;
main()
{ 
  char name[64]; int pid, cmd;
  while(1){
    pid = getpid();
    color = 0x0C;
       
    printf("----------------------------------------------\n");
    printf("Yo soy proc %d en moda U : ejecutando segmento=%x\n",pid, getcs());
    show_menu_u2();
    printf("Comando ? ");
    gets(name); 
    if (name[0]==0) 
        continue;
    cmd = find_cmd(name);

    switch(cmd){
           case 0 : getpid();   break;
           case 1 : ps();       break;
           case 2 : chname();   break;
           case 3 : kfork();    break;
           case 4 : kswitch();  break;
           case 5 : wait();     break;
           case 6 : exit();     break;
	   case 7 : fork();	break;
	   case 8 : exec();	break;

           default: invalid(name); break;
    }
	
  }

}




