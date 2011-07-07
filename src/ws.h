#include <stdio.h>
#include <stdlib.h>

#define NUM_CMD 24
#define DEFAULT_TEXT_SIZE 64
#define MAX_NUM_ARRAY_SIZE (sizeof(int) * CHAR_BIT)
#define SYMBOL_TABLE_SIZE 4271 // prime
#define DEFAULT_OUT_FILE "a.wb"
#define DUMMY_ARG 0

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
void stack_dump(void);
int stack_peek(int n);
int stack_pop(void);
void ws_psh(int num);
void ws_dup(int _num);
void ws_cpy(int num);
void ws_swp(int _num);
void ws_dsc(int _num);
void ws_sld(int _num);
void ws_add(int _num);
void ws_sub(int _num);
void ws_mul(int _num);
void ws_div(int _num);
void ws_mod(int _num);

/* heap.c */
void ws_put(int _num);
void ws_get(int _num);

/* io.c */
void ws_pch(int _num);
void ws_pnm(int _num);
void ws_gch(int _num);
void ws_gnm(int _num);

/* flow_ctrl.c */
void ws_lbl(int _label);
void ws_jal(int label);
void ws_jmp(int label);
void ws_jsz(int label);
void ws_jsn(int label);
void ws_ret(int _label);
void ws_end(int _label);

/* lexer.c */
INSTRUCTION* lexcal_analysis(FILE *fp);

