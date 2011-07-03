#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ws.h"

int get_num_instructions(const char *file_name) {
   struct stat st;
   stat(file_name, &st);
   return (int)(st.st_size / sizeof(INSTRUCTION));
}

int main(int argc, char *argv[]) {
   int i, text_size;
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
      switch(text[i].cmd_t) {
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
      if(REQUIRE_PARAM(text[i].cmd_t)) {
         printf(" %3d\n", text[i].param);
      } else {
         putchar('\n');
      }
   }
   free(text);
   fclose(fp);
   return 0;
}

