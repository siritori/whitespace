#include <stdio.h>
#include <stdlib.h>
#include "ws.h"

int hash(int num) {
   printf("%d -> %d\n", num, ((unsigned)num * 3 + 5) % SYMBOL_TABLE_SIZE);
   return ((unsigned)num * 3 + 5) % SYMBOL_TABLE_SIZE;
}

enum IMP_TYPE find_imp_from_cmd(enum CMD_TYPE cmd_t) {
   switch(cmd_t) {
   case CMD_PSH:
   case CMD_DUP:
   case CMD_CPY:
   case CMD_SWP:
   case CMD_DSC:
      return IMP_STACK;
   case CMD_ADD:
   case CMD_SUB:
   case CMD_MUL:
   case CMD_DIV:
   case CMD_MOD:
      return IMP_ARITHMETIC;
   case CMD_PUT:
   case CMD_GET:
      return IMP_HEAP_ACCESS;
   case CMD_LBL:
   case CMD_JAL:
   case CMD_JSZ:
   case CMD_JSN:
   case CMD_JMP:
   case CMD_RET:
   case CMD_END:
      return IMP_FLOW_CTRL;
   case CMD_PCH:
   case CMD_PNM:
   case CMD_GCH:
   case CMD_GNM:
      return IMP_IO;
   }
}

enum CMD_TYPE str2cmd(char *str) {
   if(!strcmp(str, "psh")) return CMD_PSH;
   else if(!strcmp(str, "dup")) return CMD_DUP;
   else if(!strcmp(str, "cpy")) return CMD_CPY;
   else if(!strcmp(str, "swp")) return CMD_SWP;
   else if(!strcmp(str, "dsc")) return CMD_DSC;
   else if(!strcmp(str, "add")) return CMD_ADD;
   else if(!strcmp(str, "sub")) return CMD_SUB;
   else if(!strcmp(str, "mul")) return CMD_MUL;
   else if(!strcmp(str, "div")) return CMD_DIV;
   else if(!strcmp(str, "mod")) return CMD_MOD;
   else if(!strcmp(str, "put")) return CMD_PUT;
   else if(!strcmp(str, "get")) return CMD_GET;
   else if(!strcmp(str, "lbl")) return CMD_LBL;
   else if(!strcmp(str, "jal")) return CMD_JAL;
   else if(!strcmp(str, "jsz")) return CMD_JSZ;
   else if(!strcmp(str, "jsn")) return CMD_JSN;
   else if(!strcmp(str, "jmp")) return CMD_JMP;
   else if(!strcmp(str, "ret")) return CMD_RET;
   else if(!strcmp(str, "end")) return CMD_END;
   else if(!strcmp(str, "pch")) return CMD_PCH;
   else if(!strcmp(str, "pnm")) return CMD_PNM;
   else if(!strcmp(str, "gch")) return CMD_GCH;
   else if(!strcmp(str, "gnm")) return CMD_GNM;
   else {
      fprintf(stderr, "unknown command%s\n", str);
      fflush(stdin);
      return CMD_END;
   }
}


int main(void) {
   int i;
   FILE *fp = fopen("a.wb", "wb");
   enum CMD_TYPE cmd_t;
   INSTRUCTION ins;
   char cmd[16];
   int param;
   for(i = 0; ; ++i) {
      param = 0;
      printf("%03d>", i);
      scanf("%s", cmd);
      if(!strcmp("exit", cmd)) break;
      cmd_t = str2cmd(cmd);
      if(REQUIRE_PARAM(cmd_t)) {
         scanf("%d", &param);
      }
      ins.imp_t = find_imp_from_cmd(cmd_t);
      ins.cmd_t = cmd_t;
      ins.param = param;
      fwrite(&ins, 1, sizeof(INSTRUCTION), fp);
   }
   fclose(fp);
   return 0;
}

