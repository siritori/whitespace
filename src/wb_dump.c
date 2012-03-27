#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ws.h"

int get_sizeof_instruction(const char *file_name) {
   struct stat st;
   stat(file_name, &st);
   return (int)(st.st_size / sizeof(INSTRUCTION));
}

int main(int argc, char *argv[]) {
   FILE *fp;
   int i, text_size;
   INSTRUCTION *p, *text;
   if(argc < 2) return 1;
   if((fp = fopen(argv[1], "rb")) == NULL) return 1;
   text_size = get_sizeof_instruction(argv[1]);
   text = (INSTRUCTION*)malloc(sizeof(INSTRUCTION) * text_size);
   for(i = 0; i < text_size; ++i) {
      fread(&text[i], 1, sizeof(INSTRUCTION), fp);
   }
   fclose(fp);
   for(p = text; p-text < text_size; ++p) {
      print_instruction(p, p-text);
   }
   free(text);
   return 0;
}

