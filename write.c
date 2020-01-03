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
int main(int argc, char *argv[]){
  int shmd, semd;
  int out;
  struct sembuf sb;
  int r, v;

  semd = semget(KEY, 1, 0);
  sb.sem_num = 0;
  sb.sem_op = -1;
  semop(semd, &sb, 1);
  shmd = shmget(KEY, sizeof(char *), 0);
  if (shmd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  out = open(OUT, O_WRONLY | O_APPEND);
  char *end = shmat(shmd, 0, 0); // get the last line from shared memory
  printf("Last line: %s\n\n", end);
  char newline[SEG_SIZE];
  printf("Your line: ");
  fgets(newline, SEG_SIZE, stdin); // read in the next line
  printf("\n");

  write(out, newline, strlen(newline)); // write that to the file
  close(out);
  strcpy(end, newline); // the last addition becomes what was just added
  shmdt(end);
  sb.sem_op = 1;
  semop(semd, &sb, 1);
  return 0;
}