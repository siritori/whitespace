#include <stdio.h>
#include <stdlib.h>
#include "ws.h"

int symbol_table[SYMBOL_TABLE_SIZE] = { -1 };

int hash(int num) {
   return (num * 171 + 5) % SYMBOL_TABLE_SIZE;
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
   for(p = text; p->cmd_t != CMD_END; ++p) {
      if(p->cmd_t == CMD_LBL) {
         symbol_table[hash(p->param)] = p - text;
      }
   }
   /* replace on symbol_table */
   for(p = text; p->cmd_t != CMD_END; ++p) {
      switch(p->cmd_t) {
      case CMD_PSH : printf("PSH"); break;
      case CMD_DUP : printf("DUP"); break;
      case CMD_CPY : printf("CPY"); break;
      case CMD_SWP : printf("SWP"); break;
      case CMD_DSC : printf("DSC"); break;
      case CMD_SLD : printf("SLD"); break;
      case CMD_ADD : printf("ADD"); break;
      case CMD_SUB : printf("SUB"); break;
      case CMD_MUL : printf("MUL"); break;
      case CMD_DIV : printf("DIV"); break;
      case CMD_MOD : printf("MOD"); break;
      case CMD_PUT : printf("PUT"); break;
      case CMD_GET : printf("GET"); break;
      case CMD_JAL : printf("JAL"); break;
      case CMD_JMP : printf("JMP"); break;
      case CMD_JSZ : printf("JSZ"); break;
      case CMD_JSN : printf("JSN"); break;
      case CMD_RET : printf("RET"); break;
      case CMD_END : printf("END"); break;
      case CMD_PCH : printf("PCH"); break;
      case CMD_PNM : printf("PNM"); break;
      case CMD_GCH : printf("GCH"); break;
      case CMD_GNM : printf("GNM"); break;
      default:
         break;
      }
      if(p->imp_t == IMP_FLOW_CTRL && REQUIRE_PARAM(p->cmd_t)) {
         p->param = symbol_table[hash(p->param)];
      }
      if(REQUIRE_PARAM(p->cmd_t)) {
         printf(" %4d\n", p->param);
      } else {
         putchar('\n');
      }
   }
   for(p = text; p->cmd_t != CMD_END; ++p) {
      fwrite(p, 1, sizeof(INSTRUCTION), fp_out);
   }
   fwrite(p, 1, sizeof(INSTRUCTION), fp_out);
   fclose(fp_out);
   free(text);
   return 0;
}

