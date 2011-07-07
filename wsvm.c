#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ws.h"

/* NOTICE: These two pointers are used also flow_ctrl.c and fun_stack.c*/
INSTRUCTION *pp;     // Program Pointer
INSTRUCTION *text;   // Text Data

void (*funtable[NUM_CMD])(int);

void init_funtable(void) {
   funtable[CMD_PSH] = ws_psh;
   funtable[CMD_DUP] = ws_dup;
   funtable[CMD_CPY] = ws_cpy;
   funtable[CMD_SWP] = ws_swp;
   funtable[CMD_DSC] = ws_dsc;
   funtable[CMD_SLD] = ws_sld;
   funtable[CMD_ADD] = ws_add;
   funtable[CMD_SUB] = ws_sub;
   funtable[CMD_MUL] = ws_mul;
   funtable[CMD_DIV] = ws_div;
   funtable[CMD_MOD] = ws_mod;
   funtable[CMD_PUT] = ws_put;
   funtable[CMD_GET] = ws_get;
   funtable[CMD_LBL] = ws_lbl;
   funtable[CMD_JAL] = ws_jal;
   funtable[CMD_JMP] = ws_jmp;
   funtable[CMD_JSZ] = ws_jsz;
   funtable[CMD_JSN] = ws_jsn;
   funtable[CMD_RET] = ws_ret;
   funtable[CMD_PCH] = ws_pch;
   funtable[CMD_PNM] = ws_pnm;
   funtable[CMD_GCH] = ws_gch;
   funtable[CMD_GNM] = ws_gnm;
   return;
}

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
   /* allocate text data */
   text_size = get_sizeof_instruction(argv[1]);
   if((text = (INSTRUCTION*)malloc(text_size)) == NULL) {
      fprintf(stderr, "FATAL ERROR: Failed to allocate memory.\n");
      exit(EXIT_FAILURE);
   }
   /* load program */
   for(i = 0; i < text_size; ++i) {
      fread(&text[i], 1, sizeof(INSTRUCTION), fp);
   }
   fclose(fp);
   /* init funtable */
   init_funtable();
   for(pp = text; pp->cmd_t != CMD_END ; ++pp) {
//      stack_dump();
//      print_instruction(pp, pp-text);
      (*funtable[pp->cmd_t])(pp->param);
   }
   free(text);
   return 0;
}

