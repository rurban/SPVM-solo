#ifndef SPERL_VM_H
#define SPERL_VM_H

#include "sperl_base.h"

// Field information
struct SPerl_vm {
  SPerl_OP* op_type;
  SPerl_OP* op_name;
  int32_t id;
};

SPerl_VM* SPerl_VM_new(SPerl_PARSER* parser);
void SPerl_VM_run(SPerl_PARSER* parser);

#endif
