#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ws.h"

/* NOTICE: These two pointers are used also flow_ctrl.c */
INSTRUCTION *pp;     // Program Pointer
INSTRUCTION *text;   // Text Data

int get_sizeof_instruction(const char *file_name) {
   struct stat st;
   stat(file_name, &st);
   return (int)(st.st_size / sizeof(INSTRUCTION));
}

int main(int argc, char *argv[]) {
   int i, text_size, num;
   unsigned file_size;
   FILE *fp;
   /* arg is only two */
   if(argc != 2) {
      fprintf(stderr, "Usage: wsvm wb-file\n");
      exit(EXIT_FAILURE);
   }
   /* open file */ 
   if((fp = fopen(argv[1], "rb")) == NULL) {
      fprintf(stderr, "ERROR: Failed to open file '%s'\n", argv[1]);
      exit(EXIT_FAILURE);
   }
   text_size = get_sizeof_instruction(argv[1]);
   if((text = (INSTRUCTION*)malloc(text_size)) == NULL) {
      fprintf(stderr, "FATAL ERROR: Failed to allocate memory.\n");
      exit(EXIT_FAILURE);
   }
   for(i = 0; i < text_size; ++i) {
      fread(&text[i], 1, sizeof(INSTRUCTION), fp);
   }
   for(pp = text; pp->cmd_t != CMD_END ; ++pp) {
//      stack_dump();
//      print_instruction(p, p-text);
      switch(pp->cmd_t) {
      case CMD_PSH: ws_psh(pp->param);  break;
      case CMD_DUP: ws_dup(pp->param);  break;
      case CMD_CPY: ws_cpy(pp->param);  break;
      case CMD_SWP: ws_swp(pp->param);  break;
      case CMD_DSC: ws_dsc(pp->param);  break;
      case CMD_SLD: ws_sld(pp->param);  break;
      case CMD_ADD: ws_add(pp->param);  break;
      case CMD_SUB: ws_sub(pp->param);  break;
      case CMD_MUL: ws_mul(pp->param);  break;
      case CMD_DIV: ws_div(pp->param);  break;
      case CMD_MOD: ws_mod(pp->param);  break;
      case CMD_PUT: ws_put(pp->param);  break;
      case CMD_GET: ws_get(pp->param);  break;
      case CMD_PCH: ws_pch(pp->param);  break;
      case CMD_PNM: ws_pnm(pp->param);  break;
      case CMD_GCH: ws_gch(pp->param);  break;
      case CMD_GNM: ws_gnm(pp->param);  break;
      case CMD_JAL: ws_jal(pp->param);  break;
      case CMD_JMP: ws_jmp(pp->param);  break;
      case CMD_JSZ: ws_jsz(pp->param);  break;
      case CMD_JSN: ws_jsn(pp->param);  break;
      case CMD_RET: ws_ret(pp->param);  break;
      }
   }
END_OF_PROGRAM:
   free(text);
   fclose(fp);
   return 0;
}

