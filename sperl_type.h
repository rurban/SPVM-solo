#ifndef SPERL_TYPE_H
#define SPERL_TYPE_H

#include "sperl_core_type.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"
#include "sperl_array.h"
#include "sperl_word.h"
#include "sperl_subtype.h"
#include "sperl_class.h"

enum SPerl_type_type {
  SPerl_TYPE_TYPE_CLASS,
  SPerl_TYPE_TYPE_SUBTYPE,
  SPerl_TYPE_TYPE_TYPEDEF,
  SPerl_TYPE_TYPE_UNKNOWN,
  SPerl_TYPE_TYPE_WORDTYPE
};

struct SPerl_type {
  SPerl_char type;
  SPerl_WORD* name_word;
  union {
    SPerl_CLASS* class;
    SPerl_SUBTYPE* subtype;
    SPerl_TYPE* type;
  } uv;
};

SPerl_TYPE* SPerl_TYPE_new(SPerl_PARSER* parser);

#endif
