#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ws.h"

#define PROG_STACK_SIZE 512
static int prog_stack[PROG_STACK_SIZE];
static int *prog_sp = &prog_stack[0];

int get_sizeof_instruction(const char *file_name) {
   struct stat st;
   stat(file_name, &st);
   return (int)(st.st_size / sizeof(INSTRUCTION));
}

int main(int argc, char *argv[]) {
   int i, text_size, num;
   unsigned file_size;
   INSTRUCTION *p, *text;
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
   text_size = get_num_instructions(argv[1]);
   if((text = (INSTRUCTION*)malloc(text_size)) == NULL) {
      fprintf(stderr, "FATAL ERROR: Failed to allocate memory.\n");
      exit(EXIT_FAILURE);
   }
   for(i = 0; i < text_size; ++i) {
      fread(&text[i], 1, sizeof(INSTRUCTION), fp);
//      print_instruction(&text[i], i);
   }
   p = text;
   while(1) {
//      stack_dump();
//      print_instruction(p, p-text);
      switch(p->cmd_t) {
      case CMD_PSH: ws_psh(p->param);  break;
      case CMD_DUP: ws_dup();          break;
      case CMD_CPY: ws_cpy(p->param);  break;
      case CMD_SWP: ws_swp();          break;
      case CMD_DSC: ws_dsc();          break;
      case CMD_SLD: ws_sld();          break;
      case CMD_ADD: ws_add();          break;
      case CMD_SUB: ws_sub();          break;
      case CMD_MUL: ws_mul();          break;
      case CMD_DIV: ws_div();          break;
      case CMD_MOD: ws_mod();          break;
      case CMD_PUT: ws_put();          break;
      case CMD_GET: ws_get();          break;
      case CMD_LBL: break; // do nothing
      case CMD_JAL:
         *(prog_sp++) = p - text;
         p = &text[p->param];
         break;
      case CMD_JMP:
         p = &text[p->param];
         break;
      case CMD_JSZ:
         num = stack_pop();
         if(num == 0) {
            p = &text[p->param];
         }
         break;
      case CMD_JSN:
         num = stack_pop();
         if(num < 0) {
            p = &text[p->param];
         }
         break;
      case CMD_RET:
         p = &text[*(--prog_sp)];
         break;
      case CMD_END:
         printf("end of the program.\n");
         goto END_OF_PROGRAM;
      case CMD_PCH:
         putchar(stack_pop());
         break;
      case CMD_PNM:
         printf("%d", stack_pop());
         break;
      case CMD_GCH: {
         char ch;
         ch = getchar();
         ws_psh(ch);
         ws_put();
         break;
      }
      case CMD_GNM: {
         int num;
         scanf("%d", &num);
         ws_psh(num);
         ws_put();
         break;
      }
      }
      ++p;
   }
END_OF_PROGRAM:
   free(text);
   fclose(fp);
   return 0;
}

