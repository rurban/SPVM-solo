#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "spvm.h"
#include "spvm_constant_pool.h"
#include "spvm_constant.h"
#include "spvm_package.h"
#include "spvm_field.h"
#include "spvm_sub.h"
#include "spvm_constant_pool_package.h"
#include "spvm_constant_pool_field.h"
#include "spvm_constant_pool_sub.h"
#include "spvm_allocator_util.h"
#include "spvm_array.h"
#include "spvm_op.h"
#include "spvm_value.h"

SPVM_CONSTANT_POOL* SPVM_CONSTANT_POOL_new(SPVM* spvm) {
  (void)spvm;
  
  SPVM_CONSTANT_POOL* constant_pool = SPVM_ALLOCATOR_UTIL_safe_malloc_i32(1, sizeof(SPVM_CONSTANT_POOL));
  constant_pool->capacity = 64;
  
  // index 0 is not used.
  constant_pool->length = 1;
  
  SPVM_VALUE* values = SPVM_ALLOCATOR_UTIL_safe_malloc_i32(constant_pool->capacity, sizeof(SPVM_VALUE));
  constant_pool->values = values;
  
  return constant_pool;
}

int32_t SPVM_CONSTANT_POOL_calculate_extend_length(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, int32_t byte_size) {
  (void)spvm;
  (void)constant_pool;
  
  int32_t length = (byte_size + (sizeof(SPVM_VALUE) - 1)) / sizeof(SPVM_VALUE);
  
  return length;
}

void SPVM_CONSTANT_POOL_extend(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, int32_t extend) {
  (void)spvm;
  
  int32_t capacity = constant_pool->capacity;
  
  if (constant_pool->length + extend >= capacity) {
    int32_t new_capacity = capacity * 2;
    constant_pool->values = SPVM_ALLOCATOR_UTIL_safe_realloc_i32(constant_pool->values, new_capacity, sizeof(SPVM_VALUE));
    constant_pool->capacity = new_capacity;
  }
}

void SPVM_CONSTANT_POOL_push_package(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, SPVM_PACKAGE* package) {
  (void)spvm;
  
  // Add package information
  int32_t extend_length = SPVM_CONSTANT_POOL_calculate_extend_length(spvm, constant_pool, sizeof(SPVM_CONSTANT_POOL_PACKAGE));
  SPVM_CONSTANT_POOL_extend(spvm, constant_pool, extend_length);
  
  // Constant pool package information
  SPVM_CONSTANT_POOL_PACKAGE* constant_pool_package = (SPVM_CONSTANT_POOL_PACKAGE*)&constant_pool->values[constant_pool->length];
  constant_pool_package->byte_size = package->byte_size;
  constant_pool_package->name_constant_pool_address = package->name_constant_pool_address;
  constant_pool_package->ref_fields_count = package->ref_fields_count;
  
  constant_pool->length += extend_length;
}

void SPVM_CONSTANT_POOL_push_sub(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, SPVM_SUB* sub) {
  (void)spvm;
  
  // Add sub information
  int32_t extend_length = SPVM_CONSTANT_POOL_calculate_extend_length(spvm, constant_pool, sizeof(SPVM_CONSTANT_POOL_SUB));
  SPVM_CONSTANT_POOL_extend(spvm, constant_pool, extend_length);
  
  SPVM_CONSTANT_POOL_SUB* constant_pool_sub = (SPVM_CONSTANT_POOL_SUB*)&constant_pool->values[constant_pool->length];
  constant_pool_sub->native_address = sub->native_address;
  constant_pool_sub->bytecode_base = sub->bytecode_base;
  constant_pool_sub->my_vars_length = sub->op_my_vars->length;
  constant_pool_sub->operand_stack_max = sub->operand_stack_max;
  constant_pool_sub->args_length = sub->op_args->length;
  constant_pool_sub->is_native = sub->is_native;
  constant_pool_sub->abs_name_constant_pool_address = sub->abs_name_constant_pool_address;
  constant_pool_sub->file_name_constant_pool_address = sub->file_name_constant_pool_address;
  if (sub->op_return_type->code != SPVM_OP_C_CODE_VOID) {
    constant_pool_sub->has_return_value = 1;
  }
  else {
    constant_pool_sub->has_return_value = 0;
  }
  
  constant_pool->length += extend_length;
}

void SPVM_CONSTANT_POOL_push_field(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, SPVM_FIELD* field) {
  (void)spvm;
  
  // Add field information
  int32_t extend_length = SPVM_CONSTANT_POOL_calculate_extend_length(spvm, constant_pool, sizeof(SPVM_CONSTANT_POOL_FIELD));
  SPVM_CONSTANT_POOL_extend(spvm, constant_pool, extend_length);

  // Constant pool field information
  SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool->values[constant_pool->length];
  constant_pool_field->package_byte_offset = field->package_byte_offset;

  constant_pool->length += extend_length;
}

void SPVM_CONSTANT_POOL_push_int(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, int32_t value) {
  (void)spvm;
  
  // Add int value
  SPVM_CONSTANT_POOL_extend(spvm, constant_pool, 1);
  constant_pool->values[constant_pool->length].int_value = value;
  constant_pool->length++;
}

void SPVM_CONSTANT_POOL_push_long(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, int64_t value) {
  (void)spvm;
  
  // Add long value
  SPVM_CONSTANT_POOL_extend(spvm, constant_pool, 1);
  constant_pool->values[constant_pool->length].long_value = value;
  constant_pool->length++;
}

void SPVM_CONSTANT_POOL_push_float(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, float value) {
  (void)spvm;

  // Add float value
  SPVM_CONSTANT_POOL_extend(spvm, constant_pool, 1);
  constant_pool->values[constant_pool->length].float_value = value;
  constant_pool->length++;
}

void SPVM_CONSTANT_POOL_push_double(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, double value) {
  (void)spvm;
  
  // Add double value
  SPVM_CONSTANT_POOL_extend(spvm, constant_pool, 1);
  constant_pool->values[constant_pool->length].double_value = value;
  constant_pool->length++;
}

void SPVM_CONSTANT_POOL_push_string(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, const char* string) {
  
  int32_t string_length = (int32_t)strlen(string);
  
  // Add string length
  SPVM_CONSTANT_POOL_extend(spvm, constant_pool, 1);
  constant_pool->values[constant_pool->length].int_value = string_length;
  constant_pool->length++;
  
  int32_t extend_length = SPVM_CONSTANT_POOL_calculate_extend_length(spvm, constant_pool, string_length + 1);
  
  SPVM_CONSTANT_POOL_extend(spvm, constant_pool, extend_length);
  
  memcpy(&constant_pool->values[constant_pool->length], string, string_length + 1);
  
  constant_pool->length += extend_length;
}

char* SPVM_CONSTANT_POOL_get_string_value(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, int32_t address) {
  (void)spvm;
  
  assert(address);
  
  char* string_value = (char*)&constant_pool->values[address + 1];
  
  return string_value;
}

void SPVM_CONSTANT_POOL_free(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool) {
  (void)spvm;
  
  free(constant_pool->values);
  free(constant_pool);
}
