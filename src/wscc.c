#include <stdio.h>
#include <stdlib.h>
#include "ws.h"

int symbol_table[SYMBOL_TABLE_SIZE] = { -1 };

int hash(int num) {
   return ((unsigned)num * 3 + 5) % SYMBOL_TABLE_SIZE;
}

int main(int argc, char *argv[]) {
   FILE *fp_in, *fp_out;
   INSTRUCTION *p, *text;

   /* open stream */
   fp_in = stdin;
   if(argc == 2) {
      if((fp_out = fopen(argv[1], "wb")) == NULL) {
         fprintf(stderr, "ERROR: Failed to open file '%s'\n", argv[1]);
         exit(EXIT_FAILURE);
      }
   } else {
      if((fp_out = fopen(DEFAULT_OUT_FILE, "wb")) == NULL) {
         fprintf(stderr, "ERROR: Failed to open file '%s'\n", DEFAULT_OUT_FILE);
         exit(EXIT_FAILURE);
      }
   }

   /* get text */
   text = lexcal_analysis(fp_in);
   fclose(fp_in);
   /* register symbol_table */
   for(p = text; p->imp_t != IMP_END; ++p) {
      if(p->cmd_t == CMD_LBL) {
         printf("register : label%d(hash:%d) = %d\n", p->param, hash(p->param), p-text);
         symbol_table[hash(p->param)] = p - text;
      }
   }
   /* replace on symbol_table */
   for(p = text; p->imp_t != IMP_END; ++p) {
      if(p->imp_t == IMP_FLOW_CTRL && REQUIRE_PARAM(p->cmd_t)) {
         if(symbol_table[hash(p->param)] < 0) {
            fprintf(stderr, "ERROR: undefind label %d\n", p->param);
            exit(EXIT_FAILURE);
         }
         printf("replace: %d --> addr%d\n", p->param, symbol_table[hash(p->param)]);
         p->param = symbol_table[hash(p->param)];
      }
   }
   for(p = text; p->imp_t != IMP_END; ++p) {
      print_instruction(p, p-text);
      fwrite(p, 1, sizeof(INSTRUCTION), fp_out);
   }
   fwrite(p, 1, sizeof(INSTRUCTION), fp_out);
   fclose(fp_out);
   free(text);
   puts("done!");
   return 0;
}

