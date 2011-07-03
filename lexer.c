#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "ws.h"

static enum IMP_TYPE read_imp(FILE *fp) {
   char ch;
   ch = fgetc(fp);
   switch(ch) {
   case ' ' : return IMP_STACK;
   case '\n': return IMP_FLOW_CTRL;
   case '\t':
      ch = fgetc(fp);
      switch(ch) {
      case ' ' : return IMP_ARITHMETIC;
      case '\t': return IMP_HEAP_ACCESS;
      case '\n': return IMP_IO;
      default:
         fprintf(stderr, "0SYNTAX ERROR: (in IMP)expected '\\t' or '\\n' or ' '\n");
         exit(EXIT_FAILURE);
      }
   default:
      fprintf(stderr, "1SYNTAX ERROR: (in IMP)expected '\\t' or '\\n' or ' '\n");
      exit(EXIT_FAILURE);
   }
}


static enum CMD_TYPE read_stack_cmd(FILE *fp) {
   char ch;
   ch = fgetc(fp);
   switch(ch) {
   case ' ' : return CMD_PSH;
   case '\t':
      ch = fgetc(fp);
      switch(ch) {
      case ' ' : return CMD_CPY;
      case '\n': return CMD_SLD;
      default:
         fprintf(stderr, "2SYNTAX ERROR: (in CMD)expected ' ' or '\\n'\n");
         exit(EXIT_FAILURE);
      }
   case '\n':
      ch = fgetc(fp);
      switch(ch) {
      case ' ' : return CMD_DUP;
      case '\n': return CMD_DSC;
      case '\t': return CMD_SWP;
      default:
         fprintf(stderr, "3SYNTAX ERROR: (in CMD)expected ' ' or '\\n' or '\\t'\n");
         exit(EXIT_FAILURE);
      }
   default:
      fprintf(stderr, "4SYNTAX ERROR: (in IMP)expected '\\t' or '\\n' or ' '\n");
      exit(EXIT_FAILURE);
   }
}

static enum CMD_TYPE read_arithmetic_cmd(FILE *fp) {
   char ch;
   ch = fgetc(fp);
   switch(ch) {
   case ' ' :
      ch = fgetc(fp);
      switch(ch) {
      case ' ' : return CMD_ADD;
      case '\t': return CMD_SUB;
      case '\n': return CMD_MUL;
      default:
         fprintf(stderr, "5SYNTAX ERROR: (in CMD)expected ' ' or '\\n' or '\\t'\n");
         exit(EXIT_FAILURE);
      }
   case '\t':
      ch = fgetc(fp);
      switch(ch) {
      case ' ' : return CMD_DIV;
      case '\t': return CMD_MOD;
      default:
         fprintf(stderr, "6SYNTAX ERROR: (in CMD)expected ' ' or '\\t'\n");
         exit(EXIT_FAILURE);
      }
   default:
      fprintf(stderr, "7SYNTAX ERROR: (in CMD)expected ' ' or '\\t'\n");
      exit(EXIT_FAILURE);
   }
}

static enum CMD_TYPE read_head_access_cmd(FILE *fp) {
   char ch;
   ch = fgetc(fp);
   switch(ch) {
   case ' ' : return CMD_PUT;
   case '\t': return CMD_GET;
   default:
      fprintf(stderr, "8SYNTAX ERROR: (in CMD)expected ' ' or '\\t'\n");
      exit(EXIT_FAILURE);
   }
}

static enum CMD_TYPE read_flow_ctrl_cmd(FILE *fp) {
   char ch;
   ch = fgetc(fp);
   switch(ch) {
   case '\n': return CMD_END;
   case '\t':
      ch = fgetc(fp);
      switch(ch) {
      case ' ' : return CMD_JSZ;
      case '\t': return CMD_JSN;
      case '\n': return CMD_RET;
      default:
         fprintf(stderr, "9SYNTAX ERROR: (in CMD)expected ' ' or '\\n' or '\\t'\n");
         exit(EXIT_FAILURE);
      }
   case ' ' :
      ch = fgetc(fp);
      switch(ch) {
      case ' ' : return CMD_LBL;
      case '\t': return CMD_JAL;
      case '\n': return CMD_JMP;
      default:
         fprintf(stderr, "10SYNTAX ERROR: (in CMD)expected ' ' or '\\n' or '\\t'\n");
         exit(EXIT_FAILURE);
      }
   default:
      fprintf(stderr, "11 %02X(%02ld) SYNTAX ERROR: (in CMD)expected ' ' or '\\n' or '\\t'\n", ch, ftell(fp));
      exit(EXIT_FAILURE);
   }
}

static enum CMD_TYPE read_io_cmd(FILE *fp) {
   char ch;
   ch = fgetc(fp);
   switch(ch) {
   case ' ' :
      ch = fgetc(fp);
      switch(ch) {
      case ' ' : return CMD_PCH;
      case '\t': return CMD_PNM;
      default:
         fprintf(stderr, "12SYNTAX ERROR: (in CMD)expected ' ' or '\\t'\n");
         exit(EXIT_FAILURE);
      }
   case '\t':
      ch = fgetc(fp);
      switch(ch) {
      case ' ' : return CMD_GCH;
      case '\t': return CMD_GNM;
      default:
         fprintf(stderr, "13SYNTAX ERROR: (in CMD)expected ' ' or '\\t'\n");
         exit(EXIT_FAILURE);
      }
   default:
      fprintf(stderr, "14SYNTAX ERROR: (in CMD)expected ' ' or '\\t'\n");
      exit(EXIT_FAILURE);
   }
}

static enum CMD_TYPE read_cmd(FILE *fp, const enum IMP_TYPE imp_t) {
   switch(imp_t) {
   case IMP_STACK       :  return read_stack_cmd(fp);
   case IMP_ARITHMETIC  :  return read_arithmetic_cmd(fp);
   case IMP_HEAP_ACCESS :  return read_head_access_cmd(fp);
   case IMP_FLOW_CTRL   :  return read_flow_ctrl_cmd(fp);
   case IMP_IO          :  return read_io_cmd(fp);
   default:
      fprintf(stderr, "FATAL ERROR: no-match IMP\n");
      exit(EXIT_FAILURE);
   }
}

static int read_param(FILE *fp) {
   int i;
   char ch;
   char temp[MAX_NUM_ARRAY_SIZE];
   for(i = 0; i < MAX_NUM_ARRAY_SIZE; ++i) {
      if((ch = fgetc(fp)) == '\n') break;
      temp[i] = ch;
   }
   temp[i] = ch;
   return space2num(temp);
}

INSTRUCTION* lexcal_analysis(FILE *fp) {
   INSTRUCTION *p;
   INSTRUCTION *text = (INSTRUCTION*)malloc(sizeof(INSTRUCTION) * DEFAULT_TEXT_SIZE);
   if(text == NULL) {
      fprintf(stderr, "FATAL ERROR: Failed to allocate memory.\n");
      exit(EXIT_FAILURE);
   }
   p = text;
   while(1) {
      enum IMP_TYPE imp_t;
      enum CMD_TYPE cmd_t;
      int param;
      imp_t = read_imp(fp);
      cmd_t = read_cmd(fp, imp_t);
      param = REQUIRE_PARAM(cmd_t)? read_param(fp): 0;
      p->imp_t = imp_t;
      p->cmd_t = cmd_t;
      p->param = param;
      if(imp_t == IMP_FLOW_CTRL && cmd_t == CMD_END) break;
      ++p;
   }
   return text;
}
/*
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
      switch(p->imp_t) {
      case IMP_STACK       : printf("%16s ", "IMP_STACK");        break;
      case IMP_ARITHMETIC  : printf("%16s ", "IMP_ARITHMETIC");   break;
      case IMP_HEAP_ACCESS : printf("%16s ", "IMP_HEAP_ACCESS");  break;
      case IMP_FLOW_CTRL   : printf("%16s ", "IMP_FLOW_CTRL");    break;
      case IMP_IO          : printf("%16s ", "IMP_IO");           break;
      }
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
      case CMD_LBL : printf("LBL"); break;
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
      }
      if(REQUIRE_PARAM(p->cmd_t)) {
         printf(" %4d\n", p->param);
      } else {
         putchar('\n');
      }
   }
   puts("END");
   fclose(fp);
   free(text);
   return 0;
}
*/

