#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_array.h"
#include "sperl_allocator_util.h"
#include "sperl_parser.h"

SPerl_ARRAY* SPerl_ARRAY_new(SPerl* sperl, int64_t capacity) {
  
  SPerl_ARRAY* array = SPerl_ALLOCATOR_UTIL_safe_malloc(1, sizeof(SPerl_ARRAY));
  array->length = 0;
  
  if (capacity == 0) {
    array->capacity = 128;
  }
  else {
    array->capacity = capacity;
  }
  
  SPerl_VALUE_T* values = SPerl_ALLOCATOR_UTIL_safe_malloc(array->capacity, sizeof(SPerl_VALUE_T));
  array->values = values;
  
  return array;
}

void SPerl_ARRAY_maybe_realloc(SPerl* sperl, SPerl_ARRAY* array) {
  int64_t length = array->length;
  assert(length >= 0);
  
  int64_t capacity = array->capacity;
  
  if (length >= capacity) {
    if (capacity > SIZE_MAX / 2) {
      SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
    }
    int64_t new_capacity = capacity * 2;
    array->values = SPerl_ALLOCATOR_UTIL_safe_realloc(array->values, new_capacity, sizeof(SPerl_VALUE_T));
    array->capacity = new_capacity;
  }
}

void SPerl_ARRAY_push_address(SPerl* sperl, SPerl_ARRAY* array, const void* value) {
  
  SPerl_ARRAY_maybe_realloc(sperl, array);
  
  int64_t length = array->length;
  
  /* Casting away a const qualification, I know what I'm doing. */
  *(void**)&array->values[length] = value;
  array->length++;
}

void* SPerl_ARRAY_fetch_address(SPerl* sperl, SPerl_ARRAY* array, int64_t index) {
  (void)sperl;
  
  assert(index >= 0);
  
  if (array == NULL || index >= array->length) {
    return NULL;
  }
  else {
    return *(void**)&array->values[index];
  }
}

void* SPerl_ARRAY_pop_address(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  if (array->length == 0) {
    abort();
  }

  array->length--;
  
  assert(array->length >= 0);
  
  return *(void**)&array->values[array->length];
}

void SPerl_ARRAY_free(SPerl* sperl, SPerl_ARRAY* array) {
  free(array->values);
  free(array);
}
