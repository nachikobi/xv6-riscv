#include "kernel/types.h"

struct context {
  uint64 ra;
  uint64 sp;
  // callee-saved
  uint64 s0;
  uint64 s1;
  uint64 s2;
  uint64 s3;
  uint64 s4;
  uint64 s5;
  uint64 s6;
  uint64 s7;
  uint64 s8;
  uint64 s9;
  uint64 s10;
  uint64 s11;
};

#define STACK_DEPTH 512

// スレッドの型(構造体)
typedef struct uthread {
  void (*fun)();
  struct context context;
  struct uthread *next;
}uthread;

// swtchの宣言
void swtch(struct context*, struct context*);

// 関数funを実行するスレッドを作成する.funは停止しないものとする.
// uthread構造体とスタックのためのメモリを動的に確保し，
// uthread構造体へのポインタを返値とする.メモリの確保ができない
// 場合はNULLを返す.
// 作成したスレッドはstart_threadsを呼び出すまで実行しないこと.
uthread *make_uthread(void (*fun)());

// 作成されたスレッドを起動する.スレッドの実行中は戻らない.
void start_uthreads();

// 他のスレッドに実行を譲る.
// この関数はスレッド内で実行される関数内でのみ呼び出される.
void yield();
