#include <stdio.h>
#include <stdlib.h>

/*
   WhiteSpace Instruction format is this:
   IMP COMMAND [PARAM]
   PARAM is opptional. some commands have no PARAM.
   IMP stands for Instruction Modification Parameter.
*/

enum IMP_TYPE {
   IMP_STACK,        // [Space]
   IMP_ARITHMETIC,   // [Tab][Space]
   IMP_HEAP_ACCESS,  // [Tab][Tab]
   IMP_FLOW_CTRL,    // [LF]
   IMP_IO            // [Tab][LF]
};

enum CMD_TYPE {
   /* IMP_STACK */
   CMD_PSH, // push the number onto the stack
   CMD_DUP, // duplicate the top item on the stack
   CMD_CPY, // copy the Nth item on the stack onto the top of the stack
   CMD_SWP, // swap the top two items on the stack
   CMD_DSC, // discard the top item on the stack
   CMD_SLD, // slide N items off the stack, keeping the top item
   /* IMP_ARITHMETIC */
   CMD_ADD, // addition
   CMD_SUB, // substraction
   CMD_MUL, // multiplication
   CMD_DIV, // integer division
   CMD_MOD, // modulo
   /* IMP_HEAP_ACCESS */
   CMD_PUT, // store
   CMD_GET, // retrieve
   /* IMP_FLOW_CTRL */
   CMD_LBL, // mark a location in the program
   CMD_JAL, // call subroutine
   CMD_JMP, // jump unconditionally to a label
   CMD_JSZ, // jump to a label if the top of the stack is zero
   CMD_JSN, // jump to a label if the top of the stack is negative
   CMD_RET, // end a subroutine and transfer control back to the caller
   CMD_END  // end of the program
};

typedef struct __INSTRUCTION {
   enum IMP_TYPE imp_t;
   enum CMD_TYPE cmd_t;
   int param;
} INSTRUCTION;

/* num2space.c */
char* num2space(const int num); // encoded WhiteSpace-style byte string
int   space2num(const char *p); // decoded normal number

//* lexer.c */
//INSTRUCTION* lexcal_analysis(FILE *fp);

