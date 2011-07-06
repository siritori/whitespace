#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_TEXT_SIZE 64
#define MAX_NUM_ARRAY_SIZE (sizeof(int) * CHAR_BIT)
#define SYMBOL_TABLE_SIZE 4271 // prime
#define DEFAULT_OUT_FILE "a.wb"

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
   IMP_IO,           // [Tab][LF]
   IMP_END           // input end
};

enum CMD_TYPE {
   /* IMP_STACK */
   CMD_PSH, // push the number onto the stack
   CMD_DUP, // duplicate the top item on the stack
   CMD_CPY, // copy the Nth item on the stack onto the stack top
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
   CMD_JSZ, // jump to a label if the stack top is zero
   CMD_JSN, // jump to a label if the stack top is negative
   CMD_RET, // end a subroutine and transfer control back to the caller
   CMD_END, // end of the program
   /* IMP_IO */
   CMD_PCH, // output the character at the stack top
   CMD_PNM, // output the number at the end of the stack
   CMD_GCH, // read a character and place it in the location(the stack top)
   CMD_GNM  // read a number and place it in the location(the stack top)
};

#define REQUIRE_PARAM(CMD) (\
      CMD == CMD_PSH\
   || CMD == CMD_CPY\
   || CMD == CMD_SLD\
   || CMD == CMD_LBL\
   || CMD == CMD_JAL\
   || CMD == CMD_JMP\
   || CMD == CMD_JSZ\
   || CMD == CMD_JSN\
)

typedef struct __INSTRUCTION {
   enum IMP_TYPE imp_t;
   enum CMD_TYPE cmd_t;
   int param;
} INSTRUCTION;

/* debug.c */
void print_instruction(INSTRUCTION *p, int addr);
char* imp2str(const enum IMP_TYPE imp_t);
char* cmd2str(const enum CMD_TYPE cmd_t);

/* num2space.c */
char* num2space(const int num); // encoded WhiteSpace-style byte string
int   space2num(const char *p); // decoded normal number

/* stack.c */
int stack_dump(void);
int stack_peek(int n);
int stack_pop(void);
void ws_psh(int num);
void ws_dup(void);
void ws_cpy(int num);
void ws_swp(void);
void ws_dsc(void);
void ws_sld(void);
void ws_add(void);
void ws_sub(void);
void ws_mul(void);
void ws_div(void);
void ws_mod(void);

/* heap.c */
void ws_put(void);
void ws_get(void);

/* lexer.c */
INSTRUCTION* lexcal_analysis(FILE *fp);

