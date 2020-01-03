#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include "consts.h"


// union semun {
//   int              val;    /* Value for SETVAL */
//   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
//   unsigned short  *array;  /* Array for GETALL, SETALL */
//   struct seminfo  *__buf;  /* Buffer for IPC_INFO
//                               (Linux-specific) */
//};
int main(int argc, char *argv[]){
  if(argc!=2){
    printf("incorrect format \n");
    return 0;
  }

  union semun us;
  struct sembuf sb;
  int semd, shmd;
  int v, r;
  int out;
  char input[3];
  us.val = 1;
  sb.sem_num = 0;
  sb.sem_op = -1;

  if(!strcmp(argv[1],"-c")){
    //create
    semd = semget(KEY, 1, IPC_CREAT | 0644);
    printf("made sem \n");
    semctl(semd, 0, SETVAL, us);
    shmd = shmget(KEY, sizeof(char *), IPC_CREAT | 0644);
    printf("Shared memory created\n");
    out = open(OUT, O_CREAT | O_TRUNC | O_RDWR, 0644);
    return 0;
  }
  if(!strcmp(argv[1],"-r")){
    semd = semget(KEY, 1, 0);
    semop(semd, &sb, 1);
    shmd = shmget(KEY, sizeof(char *), 0);
    out = open(OUT, O_RDONLY);
    char buff[SEG_SIZE];
    read(out, buff, SEG_SIZE);
    printf("The story so far:\n");
    printf("%s\n", buff);
    close(out);
    shmctl(shmd, IPC_RMID, 0);
    printf("rm shared memory\n");
    semctl(semd, IPC_RMID, 0);
    printf("rm semaphore\n");
    remove(OUT);
    printf("File removed\n");

    return 0;
  }
  if(!strcmp(argv[1],"-v")){
    out = open(OUT, O_RDONLY);
    char buff[SEG_SIZE];
    read(out, buff, SEG_SIZE);
    printf("The story so far:\n");
    printf("%s\n", buff);
    close(out);
    return 0;
  }
  printf("argument not recognized\n");
  return 0;
}