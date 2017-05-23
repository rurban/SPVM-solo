#ifndef SPVM_REF_ARRAY_H
#define SPVM_REF_ARRAY_H

struct SPVM_ref_array {
  int32_t ref_count;
  int8_t type;
  int8_t value_type;
  int16_t byte_size;
  int32_t length;
};

enum {
  SPVM_REF_ARRAY_C_VALUE_TYPE_REF,
  SPVM_REF_ARRAY_C_VALUE_TYPE_BYTE,
  SPVM_REF_ARRAY_C_VALUE_TYPE_SHORT,
  SPVM_REF_ARRAY_C_VALUE_TYPE_INT,
  SPVM_REF_ARRAY_C_VALUE_TYPE_LONG,
  SPVM_REF_ARRAY_C_VALUE_TYPE_FLOAT,
  SPVM_REF_ARRAY_C_VALUE_TYPE_DOUBLE,
};

#endif
