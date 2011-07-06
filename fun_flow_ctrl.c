#include <stdlib.h>
#include "ws.h"

#define PROG_STACK_SIZE 512
extern INSTRUCTION *pp;
extern INSTRUCTION *text;

static int stack[PROG_STACK_SIZE];
static int *sp = &stack[0];

void ws_lbl(int _label_) {
   return; // do nothing
}

void ws_jal(int label) {
   *(sp++) = pp - text;
   pp = &text[label];
   return;
}
void ws_jmp(int label) {
   pp = &text[label];
   return;
}

void ws_jsz(int label) {
   int num;
   num = stack_pop();
   if(num == 0) {
      pp = &text[label];
   }
   return;
}

void ws_jsn(int label) {
   int num;
   num = stack_pop();
   if(num < 0) {
      pp = &text[label];
   }
   return;
}

void ws_ret(int _label) {
   pp = &text[*(--sp)];
   return;
}

void ws_end(int _label) {
   puts("END OF PROGRAM");
   pp = NULL;
   printf("pp = %p\n", pp);
   return;
}

