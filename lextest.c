#include <stdio.h>
#include <stdlib.h>
#include "ws.h"

int main(int argc, char *argv[]) {
   FILE *fp;
   INSTRUCTION *p, *text;
   if(argc < 2) {
      fp = stdin;
   } else {
      if((fp = fopen(argv[1], "r")) == NULL) {
         fprintf(stderr, "ERROR: Failed to open file '%s'\n", argv[1]);
         exit(EXIT_FAILURE);
      }
   }
   text = lexcal_analysis(fp);
   for(p = text; p->cmd_t != CMD_END; ++p) {
      printf("%15s %s", imp2str(p->imp_t), cmd2str(p->cmd_t));
      if(REQUIRE_PARAM(p->cmd_t)) {
         printf("%d\n", p->param);
      } else {
         putchar('\n');
      }
   }
   fclose(fp);
   free(text);
   return 0;
}

