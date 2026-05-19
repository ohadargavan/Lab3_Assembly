#include "util.h" 

/* declaring usage of the function we wrote in start.s */
extern int system_call(int eax, int ebx, int ecx, int edx);

/* constants for system calls*/
#define SYS_EXIT 1
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_GETDENTS 141

#define O_RDONLY 0
#define BUF_SIZE 8192 /*by instructions, we assume 8192 is enough for our data*/

/*The structure that the OS returns for every file in folder*/
struct linux_dirent {
    unsigned long  d_ino;     /* inode (like file id)*/
    long           d_off;  /*distance from beginning of the folder*/
    unsigned short d_reclen; /*length of current struct. used for jumping to next file*/
    char           d_name[];  /*file name finishing with \0*/
};

int main(int argc, char *argv[]) {
    int fd, nread;
    char buf[BUF_SIZE];
    struct linux_dirent *d;
    int bpos;

    /*variables for scanning arguments and keek the prefix*/
    int i;
    char prefix = 0;

    /*scanning the arguments to find the -a flag*/
    for (i = 1; i < argc; i++) {
        if (strncmp(argv[i], "-a", 2) == 0) {
            prefix = argv[i][2]; /*retrieve third letter*/
        }
    }

    /*open current folder for reading*/
    fd = system_call(SYS_OPEN, (int)".", O_RDONLY, 0);
    
    /*check if we got a negative number, we exit with the required error exit code*/
    if (fd < 0) {
        system_call(SYS_EXIT, 0x55, 0, 0); 
    }

    /*Request the OS to read the folder content into the buffer*/
    nread = system_call(SYS_GETDENTS, fd, (int)buf, BUF_SIZE);
    
    if (nread < 0) {
        system_call(SYS_EXIT, 0x55, 0, 0); 
    }

    /*iterate through the buffer and print file names*/
    bpos = 0;
    while (bpos < nread) {
        /*convert current adress in the buffer to a pointer of our structure*/
        d = (struct linux_dirent *) (buf + bpos);

        /*either no filtering or name fits prefix*/
        if (prefix == 0 || d->d_name[0] == prefix) {

            /*print file name with sys_write (1= screen)*/
            system_call(SYS_WRITE, 1, (int)d->d_name, strlen(d->d_name));
            
            /* print \n */
            system_call(SYS_WRITE, 1, (int)"\n", 1);
        }

        /*advance the pointer in the cur struc size to get to the next struct */
        bpos += d->d_reclen;
    }
    /*close the folder*/
    system_call(SYS_CLOSE, fd, 0, 0);

    return 0;
}
#include "util.h" 

/* declaring usage of the function we wrote in start.s */
extern int system_call(int eax, int ebx, int ecx, int edx);

/* constants for system calls*/
#define SYS_EXIT 1
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_GETDENTS 141

#define O_RDONLY 0
#define BUF_SIZE 8192 /*by instructions, we assume 8192 is enough for our data*/

/*The structure that the OS returns for every file in folder*/
struct linux_dirent {
    unsigned long  d_ino;     /* inode (like file id)*/
    long           d_off;  /*distance from beginning of the folder*/
    unsigned short d_reclen; /*length of current struct. used for jumping to next file*/
    char           d_name[];  /*file name finishing with \0*/
};

int main(int argc, char *argv[]) {
    int fd, nread;
    char buf[BUF_SIZE];
    struct linux_dirent *d;
    int bpos;

    /*open current folder for reading*/
    fd = system_call(SYS_OPEN, (int)".", O_RDONLY, 0);
    
    /*check if we got a negative number, we exit with the required error exit code*/
    if (fd < 0) {
        system_call(SYS_EXIT, 0x55, 0, 0); 
    }

    /*Request the OS to read the folder content into the buffer*/
    nread = system_call(SYS_GETDENTS, fd, (int)buf, BUF_SIZE);
    
    if (nread < 0) {
        system_call(SYS_EXIT, 0x55, 0, 0); 
    }

    /*iterate through the buffer and print file names*/
    bpos = 0;
    while (bpos < nread) {
        /*convert current adress in the buffer to a pointer of our structure*/
        d = (struct linux_dirent *) (buf + bpos);

        /*print file name with sys_write (1= screen)*/
        system_call(SYS_WRITE, 1, (int)d->d_name, strlen(d->d_name));
        
        /* print \n */
        system_call(SYS_WRITE, 1, (int)"\n", 1);

        /*advance the pointer in the cur struc size to get to the next struct */
        bpos += d->d_reclen;
    }
    /*close the folder*/
    system_call(SYS_CLOSE, fd, 0, 0);

    return 0;
}