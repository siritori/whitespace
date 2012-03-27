#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
   char ch;
   FILE *fp;

   /* Open file stream */
   if(argc < 2) { // no input file
      fp = stdin;
   } else if(argc == 2) { // file is specified
      if((fp = fopen(argv[1], "r")) == NULL) {
         fprintf(stderr, "ERROR: input file is invalid '%s'\n", argv[1]);
         exit(EXIT_FAILURE);
      }
   } else { // illegal arg
      fprintf(stderr, "Usage: ws_trans [file]\n");
      fprintf(stderr, "if there're no args, input is stdin.");
      exit(EXIT_SUCCESS);
   }
   /* translation */
   while((ch = fgetc(fp)) != EOF) {
      switch(ch) {
      case 'T': putchar('\t'); break;
      case 'S': putchar(' '); break;
      case 'N': putchar('\n'); break;
      }
   }
   fclose(fp);
   return 0;
}

