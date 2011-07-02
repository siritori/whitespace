#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_NUM_ARRAY_SIZE (sizeof(int) * CHAR_BIT)

char* num2space(int num) {
   int i;
   char *ret = (char*)malloc(MAX_NUM_ARRAY_SIZE);
   if(ret == NULL) {
      fprintf(stderr, "FATAL ERRPR: failed to allocate memory.\n");
      exit(EXIT_FAILURE);
   }
   for(i = 0; i < MAX_NUM_ARRAY_SIZE; ++i) {
      ret[(MAX_NUM_ARRAY_SIZE-1) - i] = num % 2;
      num /= 2;
   }
   return ret;
}

int main(int argc, char *argv[]) {
   int i;
   char *ret = num2space(atoi(argv[1]));
   for(i = 0; i < MAX_NUM_ARRAY_SIZE; ++i) {
      putchar('0' + ret[i]);
   }
   return 0;
}

