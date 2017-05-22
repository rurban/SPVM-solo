#ifndef SPVM_DATA_STRING_H
#define SPVM_DATA_STRING_H

#include "spvm_value.h"
#include "spvm_base.h"

struct SPVM_data_string {
  int8_t type;
  int16_t byte_size;
  int32_t ref_count;
  SPVM_SV* sv;
};

#endif