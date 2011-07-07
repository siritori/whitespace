#include <stdio.h>
#include <string.h>
#include "ws.h"

#define STACK_SIZE 2048
#define NUM_ITEMS (sp-stack)

extern INSTRUCTION *text;
extern INSTRUCTION *pp;
static int stack[STACK_SIZE];
static int *sp = &stack[0];

void stack_dump(void) {
   int *p = &stack[1];
   printf("[");
   while(p <= sp) {
      printf("%d,", *p);
      ++p;
   }
   puts("]");
}
int stack_peek(int n) {
   if(NUM_ITEMS == 0) {
      fprintf(stderr, "RUNTIME ERROR(on %04ld): stack under flow\n", pp-text);
      exit(EXIT_FAILURE);
   }
   return *(sp - n);
}
int stack_pop(void) {
   if(NUM_ITEMS == 0) {
      fprintf(stderr, "RUNTIME ERROR(on %04ld): couldn't pop because stack was empty\n", pp-text);
      exit(EXIT_FAILURE);
   }
   return *(sp--);
}
void ws_psh(int num) {
   if(NUM_ITEMS == STACK_SIZE) {
      fprintf(stderr, "RUNTIME ERROR(on %04ld): stack over flow\n", pp-text);
      exit(EXIT_FAILURE);
   }
   *(++sp) = num;
   return;
}
void ws_dup(int _num) {
   if(NUM_ITEMS == 0) {
      fprintf(stderr, "RUNTIME ERROR(on %04ld): couldn't duplicate because stack was empty\n", pp-text);
      exit(EXIT_FAILURE);
   }
   ws_psh(*sp);
   return;
}
void ws_cpy(int n) {
   int num;
   if(NUM_ITEMS-n <= 0) {
      fprintf(stderr, "RUNTIME ERROR(on %04ld): too few items to copy %d\n", pp-text, n);
      exit(EXIT_FAILURE);
   }
   num = *(sp - n);
   ws_psh(num);
}
void ws_swp(int _num) {
   int num;
   if(NUM_ITEMS < 2) {
      fprintf(stderr, "RUNTIME ERROR(on %04ld): couldn't swap because stack size was not enough\n", pp-text);
      exit(EXIT_FAILURE);
   }
   num = *(sp - 1);
   *(sp-1)  = *sp;
   *sp = num;
   return;
}
void ws_dsc(int _num) {
   if(NUM_ITEMS == 0) {
      fprintf(stderr, "RUNTIME ERROR(on %04ld): couldn't discard because stack was empty\n", pp-text);
      exit(EXIT_FAILURE);
   }
   stack_pop();
   return;
}
void ws_sld(int num) {
   if(NUM_ITEMS < num) {
      fprintf(stderr, "RUNTIME ERROR(on %04ld): too few items to slide off\n", pp-text);
      exit(EXIT_FAILURE);
   }
   return;
}

static void pop_two_items(int *a, int *b) {
   if(NUM_ITEMS < 2) {
      fprintf(stderr, "RUNTIME ERROR(on %04ld): too few items to calculate\n", pp-text);
      exit(EXIT_FAILURE);
   }
   *b = stack_pop();
   *a = stack_pop();
   return;
}

void ws_add(int _num) {
   int a, b;
   pop_two_items(&a, &b);
   ws_psh(a + b);
}
void ws_sub(int _num) {
   int a, b;
   pop_two_items(&a, &b);
   ws_psh(a - b);
}
void ws_mul(int _num) {
   int a, b;
   pop_two_items(&a, &b);
   ws_psh(a * b);
}
void ws_div(int _num) {
   int a, b;
   pop_two_items(&a, &b);
   ws_psh(a / b);
}
void ws_mod(int _num) {
   int a, b;
   pop_two_items(&a, &b);
   ws_psh(a % b);
}

