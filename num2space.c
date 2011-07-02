#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_NUM_ARRAY_SIZE (sizeof(int) * CHAR_BIT)

static int _num2space(const int num, char *ret) {
   int idx;
   if(num == 1) {
      ret[0] = '\t';
      return 1;
   } else if(num == 0) {
      ret[0] = ' ';
      return 1;
   }
   idx = _num2space(num/2, ret);
   ret[idx] = (num % 2 == 1)?'\t':' ';
   return idx+1;
}

char* num2space(const int num) {
   int idx;
   char *ret = (char*)malloc(sizeof(int) * CHAR_BIT);
   if(ret == NULL) {
      fprintf(stderr, "FATAL ERROR: Failed to allocate memory.\n");
      exit(EXIT_FAILURE);
   }
   idx = _num2space(num, ret);
   ret[idx] = '\n';
   return ret;
}

static int _space2num(const char *p, int *ret) {
   int cardinal_num;
   if(*(p+1) == '\n') {
      *ret += (*p == '\t')?1:0;
      return 2;
   }
   cardinal_num = _space2num(p+1, ret);
   *ret += (*p == '\t')? cardinal_num : 0;
   return cardinal_num * 2;
}

int space2num(const char *p) {
   int cardinal_num;
   int ret = 0;
   cardinal_num = _space2num(p, &ret);
   return ret;
}

