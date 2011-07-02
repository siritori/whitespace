#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_NUM_ARRAY_SIZE (sizeof(int) * CHAR_BIT)

int _num2space(int num, char *ret) {
   int idx;
   if(num == 1) {
      printf("[0] = 1\n");
      ret[0] = '\t';
      return 1;
   } else if(num == 0) {
      printf("[0] = 0\n");
      ret[0] = ' ';
      return 1;
   }
   idx = _num2space(num/2, ret);
   printf("[%d] = %d\n", idx, num%2);
   ret[idx] = (num % 2 == 1)?'\t':' ';
   return idx+1;
}

char* num2space(int num) {
   int idx;
   char *ret = (char*)malloc(sizeof(int) * CHAR_BIT);
   if(ret == NULL) {
      fprintf(stderr, "FATAL ERROR: Failed to allocate memory.\n");
      exit(EXIT_FAILURE);
   }
   idx = _num2space(num, ret);
   printf("[%d] = '\\n'\n", idx);
   ret[idx] = '\n';
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
   free(ret);
LOOP_END:
   return 0;
}

