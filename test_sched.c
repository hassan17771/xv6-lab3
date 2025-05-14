#include "types.h"
#include "stat.h"
#include "user.h"

#define N 5
#define WORKLOAD 50000000

void
do_work(int id)
{
  volatile int i;
  for(i = 0; i < WORKLOAD; i++) {
  }
  printf(1, "Child %d done.\n", id);
}

int
main(void)
{
  int pid, i;

  printf(1, "test_sched: starting %d children\n", N);

  for(i = 0; i < N; i++){
    pid = fork();
    if(pid < 0){
      printf(1, "Fork failed\n");
      exit();
    }
    if(pid == 0){
      // child process
      printf(1, "child %d started\n", getpid());
      do_work(i);
      exit();
    }
  }

  for(i = 0; i < N; i++)
    wait();

  printf(1, "test_sched: all children finished\n");
  exit();
}
