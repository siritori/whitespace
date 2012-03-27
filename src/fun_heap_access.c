#include "ws.h"
#include <stdio.h>

#define HEAP_SIZE 2048
static heap[HEAP_SIZE];

void ws_put(int _num) {
   int num, addr;
   num  = stack_pop();
   addr = stack_pop();
//   printf("heap[%4d] <= %d\n", addr, num);
   if(addr >= HEAP_SIZE || addr < 0) {
      fprintf(stderr, "RUNTIME ERROR: Invalid address %d\n", addr);
      exit(EXIT_FAILURE);
   }
   heap[addr] = num;
}
void ws_get(int _num) {
   int addr = stack_pop();
//   printf("heap[%d] => stack\n", addr);
   ws_psh(heap[addr]);
   return;
}


