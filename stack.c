#include <stdio.h>
#include <string.h>
#include "ws.h"

#define STACK_SIZE 2048

static int stack[STACK_SIZE];
static int *sp = &stack[0];

int stack_dump(void) {
   int *p = &stack[1];
   puts("-------");
   while(p <= sp) {
      printf("%d\n", *p);
      ++p;
   }
   puts("-------");
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
   --sp;
   return;
}
void ws_sld(void) {
   // do nothing
   return;
}
void ws_add(void) {
   int num = *(sp-1) + (*sp);
   *(--sp) = num;
}
void ws_sub(void) {
   int num = *(sp-1) - (*sp);
   *(--sp) = num;
}
void ws_mul(void) {
   int num = *(sp-1) * (*sp);
   *(--sp) = num;
}
void ws_div(void) {
   int num = *(sp-1) / (*sp);
   *(--sp) = num;
}
void ws_mod(void) {
   int num = *(sp-1) % (*sp);
   *(--sp) = num;
}

