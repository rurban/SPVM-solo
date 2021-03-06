#include <string.h>
#include <stdlib.h>

#include "spvm.h"
#include "spvm_bytecode_array.h"
#include "spvm_allocator_util.h"
#include "spvm_parser.h"

SPVM_BYTECODE_ARRAY* SPVM_BYTECODE_ARRAY_new(SPVM* spvm) {
  (void)spvm;
  
  SPVM_BYTECODE_ARRAY* bytecodes = SPVM_ALLOCATOR_UTIL_safe_malloc_i32(1, sizeof(SPVM_BYTECODE_ARRAY));
  bytecodes->capacity = 64;
  bytecodes->length = 0;
  
  uint8_t* values = SPVM_ALLOCATOR_UTIL_safe_malloc_i32(bytecodes->capacity, sizeof(uint8_t));
  bytecodes->values = values;
  
  return bytecodes;
}

void SPVM_BYTECODE_ARRAY_push(SPVM* spvm, SPVM_BYTECODE_ARRAY* bytecodes, uint8_t value) {
  (void)spvm;
  
  int32_t length = bytecodes->length;
  int32_t capacity = bytecodes->capacity;
  
  if (length >= capacity) {
    int32_t new_capacity = capacity * 2;
    bytecodes->values = (uint8_t*) SPVM_ALLOCATOR_UTIL_safe_realloc_i32(bytecodes->values, new_capacity, sizeof(uint8_t));
    memset(bytecodes->values + capacity, 0, (new_capacity - capacity) * sizeof(uint8_t));
    bytecodes->capacity = new_capacity;
  }
  bytecodes->values[length] = value;
  bytecodes->length++;
}

void SPVM_BYTECODE_ARRAY_free(SPVM* spvm, SPVM_BYTECODE_ARRAY* bytecodes) {
  (void)spvm;
  
  free(bytecodes->values);
  free(bytecodes);
}
