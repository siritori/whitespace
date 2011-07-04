#include "ws.h"

char* imp2str(const enum IMP_TYPE imp_t) {
   switch(imp_t) {
      case IMP_STACK       : return "IMP_STACK";
      case IMP_ARITHMETIC  : return "IMP_ARITHMETIC";
      case IMP_HEAP_ACCESS : return "IMP_HEAP_ACCESS";
      case IMP_FLOW_CTRL   : return "IMP_FLOW_CTRL";
      case IMP_IO          : return "IMP_IO";
      default:
         return NULL;
   }
}

char* cmd2str(const enum CMD_TYPE cmd_t) {
   switch(cmd_t) {
      case CMD_PSH : return "PSH";
      case CMD_DUP : return "DUP";
      case CMD_CPY : return "CPY";
      case CMD_SWP : return "SWP";
      case CMD_DSC : return "DSC";
      case CMD_SLD : return "SLD";
      case CMD_ADD : return "ADD";
      case CMD_SUB : return "SUB";
      case CMD_MUL : return "MUL";
      case CMD_DIV : return "DIV";
      case CMD_MOD : return "MOD";
      case CMD_PUT : return "PUT";
      case CMD_GET : return "GET";
      case CMD_LBL : return "LBL";
      case CMD_JAL : return "JAL";
      case CMD_JMP : return "JMP";
      case CMD_JSZ : return "JSZ";
      case CMD_JSN : return "JSN";
      case CMD_RET : return "RET";
      case CMD_END : return "END";
      case CMD_PCH : return "PCH";
      case CMD_PNM : return "PNM";
      case CMD_GCH : return "GCH";
      case CMD_GNM : return "GNM";
      default:
         return NULL;
   }
}

