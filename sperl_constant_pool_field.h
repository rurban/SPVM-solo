#ifndef SPERL_CONSTANT_POOL_FIELD_H
#define SPERL_CONSTANT_POOL_FIELD_H

#include "sperl_base.h"

// Array
struct SPerl_constant_pool_field {
  int32_t abs_name_constant_pool_address;
  int32_t package_byte_offset;
};

#endif
