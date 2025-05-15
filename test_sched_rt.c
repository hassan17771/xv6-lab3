#include "types.h"
#include "stat.h"
#include "user.h"

char *argv[] = { "dummywork", 0 };  // dummy program each child will exec

int
main(void)
{
  int i;
  int pid;

  // Spawn some normal processes
  for (i = 0; i < 2; i++) {
    pid = fork();
    if (pid == 0) {
      // Child process
      printf(1, "Child %d: launching normal task\n", getpid());
      exec("dummywork", argv);  // Normal process
      printf(1, "exec normal failed\n");
      exit();
    }
  }

  // Spawn some real-time processes with deadlines
  for (i = 0; i < 3; i++) {
    int deadline = 100 + 50 * i;  // Dummy deadlines
    pid = fork_rt(deadline);
    if (pid == 0) {
      printf(1, "Child %d: launching RT task with deadline %d\n", getpid(), deadline);
      exec("dummywork", argv);  // Real-time process
      printf(1, "exec_rt failed\n");
      exit();
    }
  }

  // Parent waits for all
  for (i = 0; i < 5; i++) {
    wait();
  }

  exit();
}
