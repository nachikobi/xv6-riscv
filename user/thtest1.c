#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/uthreads.h"

void foo() {
  int c = 0; for (;;) {
    printf("foo : %d\n", c);
    c += 1;
    yield();
  }
}

void bar() {
  int c = 0; for (;;) {
    printf("bar : %d\n", c);
    yield();
    c += 2;
  }
}

void baz() {
  int c = 0; for (;;) {
    printf("baz : %d\n", c);
    yield();
    c += 3;
    printf("baz : %d\n", c);
    yield();
    c += 3;
  }
}

int main() {
  make_uthread(foo);
  make_uthread(bar);
  make_uthread(baz);
  start_uthreads();
  exit(0);
}
