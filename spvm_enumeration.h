#ifndef SPVM_ENUMERATION_H
#define SPVM_ENUMERATION_H

#include "spvm_base.h"

struct SPVM_enumeration {
  SPVM_ARRAY* enumeration_values;
};

SPVM_ENUMERATION* SPVM_ENUMERATION_new(SPVM* spvm);

#endif
