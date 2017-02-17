#ifndef SPERL_H
#define SPERL_H

#include <stdio.h>

#include "sperl_base.h"

enum {
  SPerl_C_ARRAY_HEADER_LENGTH = sizeof(int64_t),
};

// Parser information
struct sperl_ {
  // Memory_pool
  SPerl_MEMORY_POOL* memory_pool;
  
  // All array pointers
  SPerl_ARRAY* array_ptrs;
  
  // All hash pointers
  SPerl_ARRAY* hash_ptrs;
  
  // All long string pointers
  SPerl_ARRAY* long_str_ptrs;
  
  // Parser
  SPerl_PARSER* parser;
  
  // Constant pool
  SPerl_CONSTANT_POOL* constant_pool;
  
  // Bytecodes
  SPerl_BYTECODE_ARRAY* bytecode_array;
};

SPerl* SPerl_new();
void SPerl_free(SPerl* sperl);

#endif
