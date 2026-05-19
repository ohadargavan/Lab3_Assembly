#include "util.h"

#define SYS_WRITE 4
#define STDOUT 1
#define SYS_OPEN 5
#define O_RDWR 2
#define SYS_SEEK 19
#define SEEK_SET 0
#define SHIRA_OFFSET 0x291

extern int system_call();

int main (int argc , char* argv[], char* envp[])
{
  int i;
  char newline = '\n';

  /*go through all arguments passed through the command line*/
  for (i = 0; i < argc; i++) {
    /*
    *SYS_WRITE = 4: the number of the system call
    *STDOUT = 1: the printing target
    *arg[i] : the actual string
    *strlen(argv[i]): length of the string
    */    
    
    system_call(SYS_WRITE, STDOUT, argv[i], strlen(argv[i]));
    
    /*print new line between arguments*/
      system_call(SYS_WRITE, STDOUT, &newline, 1);
  }

  return 0;

}