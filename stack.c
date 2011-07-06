#include <stdio.h>
#include <string.h>
#include "ws.h"

#define STACK_SIZE 2048

static int stack[STACK_SIZE];
static int *sp = &stack[0];

int stack_dump(void) {
   int *p = &stack[0];
   printf("[");
   while(p <= sp) {
      printf("%d,", *p);
      ++p;
   }
   puts("]");
}
int stack_peek(int n) {
   return *(sp - n);
}
int stack_pop(void) {
   return *(sp--);
}
void ws_psh(int num) {
   *(++sp) = num;
   return;
}
void ws_dup(void) {
   ws_psh(*sp);
   return;
}
void ws_cpy(int n) {
   int num;
   num = *(sp - n);
   ws_psh(num);
}
void ws_swp(void) {
   int num;
   num = *(sp - 1);
   *(sp-1)  = *sp;
   *sp = num;
   return;
}
void ws_dsc(void) {
   stack_pop();
   return;
}
void ws_sld(void) {
   // do nothing
   return;
}
void ws_add(void) {
   int a, b;
   b = stack_pop();
   a = stack_pop();
   ws_psh(a + b);
}
void ws_sub(void) {
   int a, b;
   b = stack_pop();
   a = stack_pop();
   ws_psh(a - b);
}
void ws_mul(void) {
   int a, b;
   b = stack_pop();
   a = stack_pop();
   ws_psh(a * b);
}
void ws_div(void) {
   int a, b;
   b = stack_pop();
   a = stack_pop();
   ws_psh(a / b);
}
void ws_mod(void) {
   int a, b;
   b = stack_pop();
   a = stack_pop();
   ws_psh(a % b);
}
