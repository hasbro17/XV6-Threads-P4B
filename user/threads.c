#include "types.h"
#include "stat.h"
#include "user.h"

int stack[4096] __attribute__ ((aligned (4096)));
int x = 0;

int main(int argc, char *argv[]) {
  printf(1, "Stack is at %p by pid=%d\n", stack, getpid());
  // int tid = fork();
  int tid = clone(stack);

  if (tid < 0) {
    printf(2, "error!\n");
  } else if (tid == 0) {
    // child
    for(;;) {
      x++;
      printf(1, "T2 :: x = %d\n", x);
      sleep(100);
    }
  } else {
    // parent
    for(;;) {
      printf(1, "T1 :: x = %d\n", x);
      sleep(100);
    }
  }

  exit();
}
