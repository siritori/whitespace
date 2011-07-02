#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_NUM_ARRAY_SIZE (sizeof(int) * CHAR_BIT)

char* num2space(int num) {
   int i;
   char *ret = (char*)malloc(MAX_NUM_ARRAY_SIZE);
   if(ret == NULL) {
      fprintf(stderr, "FATAL ERROR: failed to allocate memory.\n");
      exit(EXIT_FAILURE);
   }
   for(i = 0; i < MAX_NUM_ARRAY_SIZE; ++i) {
      ret[(MAX_NUM_ARRAY_SIZE-1) - i] = (num % 2 == 0)?' ':'\t';
      num /= 2;
   }
   ret[(MAX_NUM_ARRAY_SIZE-1) - i] = '\n';
   return ret;
}

int main(int argc, char *argv[]) {
   int i;
   char *ret = num2space(atoi(argv[1]));
   for(i = 0; i < MAX_NUM_ARRAY_SIZE; ++i) {
      switch(ret[i]) {
      case '\t': putchar('1'); break;
      case ' ' : putchar('0'); break;
      case '\n': goto LOOP_END;
      default  :
         fprintf(stderr, "FATAL ERROR: Illegal char '%c'\n", ret[i]);
         exit(EXIT_FAILURE);
      }
   }
LOOP_END:
   return 0;
}

