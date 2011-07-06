#include <stdio.h>
#include <stdlib.h>
#include "ws.h"

void ws_pch(int _num) {
   putchar(stack_pop());
   return;
}

void ws_pnm(int _num) {
   printf("%d", stack_pop());
   return;
}

void ws_gch(int _num) {
   char ch;
   ch = getchar();
   ws_psh(ch);
   ws_put(DUMMY_ARG);
   return;
}
void ws_gnm(int _num) {
   int num;
   scanf("%d", &num);
   ws_psh(num);
   ws_put(DUMMY_ARG);
   return;
}

