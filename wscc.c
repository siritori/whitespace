#include <stdio.h>
#include <stdlib.h>
#include "ws.h"

int symbol_table[SYMBOL_TABLE_SIZE] = { -1 };

int hash(int num) {
   return (num * 171 + 5) % SYMBOL_TABLE_SIZE;
}

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
      if(p->cmd_t == CMD_LBL) {
         symbol_table[hash(p->param)] = p - text;
         printf("%5d -> %ld(%d)\n", p->param, p - text, hash(p->param));
      } else if(REQUIRE_PARAM(p->cmd_t)) {
         printf(" %4d\n", p->param);
      } else {
         putchar('\n');
      }
  }
   fclose(fp);
   free(text);
   return 0;
}

