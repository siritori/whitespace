#include "ws.h"
#include <stdio.h>

#define HEAP_SIZE 2048
static heap[HEAP_SIZE];

void ws_put(void) {
   int addr = stack_peek(1);
   int num  = stack_peek(0);
   ws_dsc();
   ws_dsc();
   printf("heap[%4d] <= %d\n", addr, num);
   if(addr >= HEAP_SIZE || addr < 0) {
      fprintf(stderr, "RUNTIME ERROR: Invalid address %d\n", addr);
      exit(EXIT_FAILURE);
   }
   heap[addr] = num;
}
void ws_get(void) {
   int addr = stack_peek(0);
   printf("heap[%d] => stack\n", addr);
   ws_psh(heap[addr]);
   return;
}


