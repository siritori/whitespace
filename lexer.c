#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "ws.h"

char next_ch(FILE *fp) {
   char ch;
   while(1) {
      ch = fgetc(fp);
      switch(ch) {
      case ' ' :
      case '\t':
      case '\n':
      case EOF:
         return ch;
      }
   }
}

static enum IMP_TYPE read_imp(FILE *fp) {
   char ch;
   if(feof(fp)) return IMP_END;
   ch = next_ch(fp);
   switch(ch) {
   case EOF : return IMP_END;
   case ' ' : return IMP_STACK;
   case '\n': return IMP_FLOW_CTRL;
   case '\t':
      ch = next_ch(fp);
      switch(ch) {
      case ' ' : return IMP_ARITHMETIC;
      case '\n': return IMP_IO;
      case '\t': return IMP_HEAP_ACCESS;
      default:
         fprintf(stderr, "SYNTAX ERROR: (in IMP)expected '\\t' or '\\n' or ' '\n");
         exit(EXIT_FAILURE);
      }
   }
}

static enum CMD_TYPE read_stack_cmd(FILE *fp) {
   char ch;
   ch = next_ch(fp);
   switch(ch) {
   case ' ' : return CMD_PSH;
   case '\n':
      ch = next_ch(fp);
      switch(ch) {
      case ' ' : return CMD_DUP;
      case '\n': return CMD_DSC;
      case '\t': return CMD_SWP;
      default:
         fprintf(stderr, "SYNTAX ERROR: (in IMP_STACK)expected '\\t' or '\\n' or ' '\n");
         exit(EXIT_FAILURE);
      }
   case '\t':
      ch = next_ch(fp);
      switch(ch) {
      case ' ' : return CMD_CPY;
      case '\n': return CMD_SLD;
      default:
         fprintf(stderr, "SYNTAX ERROR: (in IMP_STACK)expected '\\n' or ' '\n");
         exit(EXIT_FAILURE);
      }
   default:
      fprintf(stderr, "SYNTAX ERROR: (in IMP_STACK)expected '\\t' or '\\n' or ' '\n");
      exit(EXIT_FAILURE);
   }
}

static enum CMD_TYPE read_arithmetic_cmd(FILE *fp) {
   char ch;
   ch = next_ch(fp);
   switch(ch) {
   case ' ' :
      ch = next_ch(fp);
      switch(ch) {
      case ' ' : return CMD_ADD;
      case '\n': return CMD_MUL;
      case '\t': return CMD_SUB;
      default:
         fprintf(stderr, "SYNTAX ERROR: (in IMP_ARITHMETIC)expected '\\t' or '\\n' or ' '\n");
         exit(EXIT_FAILURE);
      }
   case '\t':
      ch = next_ch(fp);
      switch(ch) {
      case ' ' : return CMD_DIV;
      case '\t': return CMD_MOD;
      default:
         fprintf(stderr, "SYNTAX ERROR: (in IMP_ARITHMETIC)expected '\\t' or ' '\n");
         exit(EXIT_FAILURE);
      }
   default:
      fprintf(stderr, "SYNTAX ERROR: (in IMP_ARITHMETIC)expected '\\t' or ' '\n");
      exit(EXIT_FAILURE);
   }
}

static enum CMD_TYPE read_head_access_cmd(FILE *fp) {
   char ch;
   ch = next_ch(fp);
   switch(ch) {
   case ' ' : return CMD_PUT;
   case '\t': return CMD_GET;
   default:
      fprintf(stderr, "SYNTAX ERROR: (in IMP_HEAP_ACCESS)expected ' ' or '\\t'\n");
      exit(EXIT_FAILURE);
   }
}

static enum CMD_TYPE read_flow_ctrl_cmd(FILE *fp) {
   char ch;
   ch = next_ch(fp);
   switch(ch) {
   case ' ' :
      ch = next_ch(fp);
      switch(ch) {
      case ' ' : return CMD_LBL;
      case '\n': return CMD_JMP;
      case '\t': return CMD_JAL;
      default:
         fprintf(stderr, "SYNTAX ERROR: (in IMP_FLOW_CTRL)expected '\\t' or '\\n' or ' '\n");
         exit(EXIT_FAILURE);
      }
   case '\n':
      ch = next_ch(fp);
      if(ch == '\n') return CMD_END;
   case '\t':
      ch = next_ch(fp);
      switch(ch) {
      case ' ' : return CMD_JSZ;
      case '\n': return CMD_RET;
      case '\t': return CMD_JSN;
      default:
         fprintf(stderr, "SYNTAX ERROR: (in IMP_FLOW_CTRL)expected '\\t' or '\\n' or ' '\n");
         exit(EXIT_FAILURE);
      }
   default:
      fprintf(stderr, "SYNTAX ERROR: (in IMP_FLOW_CTRL)expected '\\t' or '\\n' or ' '\n");
      exit(EXIT_FAILURE);
   }
}

static enum CMD_TYPE read_io_cmd(FILE *fp) {
   char ch;
   ch = next_ch(fp);
   switch(ch) {
   case ' ' :
      ch = next_ch(fp);
      switch(ch) {
      case ' ' : return CMD_PCH;
      case '\t': return CMD_PNM;
      default:
         fprintf(stderr, "SYNTAX ERROR: (in IMP_IO)expected '\\t' or ' '\n");
         exit(EXIT_FAILURE);
      }
   case '\t':
      ch = next_ch(fp);
      switch(ch) {
      case ' ' : return CMD_GCH;
      case '\t': return CMD_GNM;
      default:
         fprintf(stderr, "SYNTAX ERROR: (in CMD)expected '\\t' or ' '\n");
         exit(EXIT_FAILURE);
      }
   default:
      fprintf(stderr, "SYNTAX ERROR: (in CMD)expected '\\t' or ' '\n");
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

static int read_label(FILE *fp) {
   char ch;
   int ret = 0;
   while((ch = next_ch(fp)) != '\n') {
      if(ch == EOF) {
         fprintf(stderr, "SYNTAX ERROR: (in label)expected '\\t' or '\\n' or ' '\n");
         exit(EXIT_FAILURE);
      }
      ret = (unsigned)(ret * 3 + 7) + ((ch == '\t')? 2 : 5);
      ret %= SYMBOL_TABLE_SIZE;
   }
   return ret;
}

static int read_param(FILE *fp) {
   int i;
   char ch;
   char temp[MAX_NUM_ARRAY_SIZE];
   for(i = 0; i < MAX_NUM_ARRAY_SIZE; ++i) {
      if((ch = next_ch(fp)) == '\n') break;
      else if(ch == EOF) {
         fprintf(stderr, "SYNTAX ERROR: (in parameter)expected '\\t' or '\\n' or ' '\n");
         exit(EXIT_FAILURE);
      }
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
      char ch;
      enum IMP_TYPE imp_t;
      enum CMD_TYPE cmd_t;
      int param;
      imp_t = read_imp(fp);
      if(imp_t == IMP_END) {
         p->imp_t = imp_t;
         break;
      }
      cmd_t = read_cmd(fp, imp_t);
      if(REQUIRE_PARAM(cmd_t)) {
         param = (imp_t == IMP_FLOW_CTRL)? read_label(fp): read_param(fp);
      } else {
         param = -1;
      }
      p->imp_t = imp_t;
      p->cmd_t = cmd_t;
      p->param = param;
      ++p;
   }
   return text;
}

