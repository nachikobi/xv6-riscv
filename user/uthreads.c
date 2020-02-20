#include "kernel/types.h"   // for uint
#include "kernel/stat.h"
#include "user/user.h"
#include "user/uthreads.h"
#include <stddef.h>

#define NTHREAD 64  // maximum number of threads
uthread uthreads[NTHREAD];

static uthread *head;
static uthread *tail;
uthread *old;
uthread *new;



uthread *NEW(void (*fun)(), uthread *next) {
  uthread *x = malloc(sizeof *x);
  x->fun = *fun;
  x->next = next;
  x->context.ra = (uint64)fun;
  x->context.sp = (uint64)(malloc(sizeof (uint64)*STACK_DEPTH) + STACK_DEPTH);   //スタック領域を動的確保
  /*if (x->context.sp == NULL) {
    return NULL;
  }*/
  return x;
}

/* キューに追加していく */
uthread *make_uthread(void (*fun)()) {
  if (head == NULL) {
    head = (tail = NEW(*fun, head));
    return tail;
  }
  tail->next = NEW(*fun, NULL);
  tail = tail->next;
  return tail;
}


/* デキューしたものを実行 */
void start_uthreads() {
  //デキュー
  old = head;
  head = head->next;

  void (*funcp)();
  funcp = old->fun;
  (*funcp)();

}

/* 呼ばれた時に実行していた関数をエンキューして、デキューしたものを実行 */
void yield() {
  //エンキュー
  if (head == NULL) {
    head = old;
    old->next = head;
  } else {
    tail->next = old;
    old->next = NULL;
    tail = tail->next;
  }

  //デキュー
  new = head;
  head = head->next;

  //スレッド切り替え
  struct context *old_ctx = &(old->context);
  struct context *new_ctx = &(new->context);
  old = new; //現在実行しているものを更新
  swtch(old_ctx, new_ctx);
}
