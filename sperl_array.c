#include <string.h>
#include <stdlib.h>

#include "sperl_array.h"
#include "sperl_allocator.h"

SPerl_ARRAY* SPerl_ARRAY_new(size_t capacity) {
  
  SPerl_ARRAY* array = SPerl_ALLOCATOR_safe_malloc(1, sizeof(SPerl_ARRAY));
  array->length = 0;
  
  if (capacity == 0) {
    array->capacity = 128;
  }
  else {
    array->capacity = capacity;
  }
  
  void** values = SPerl_ALLOCATOR_safe_malloc_zero(array->capacity, sizeof(void*));
  array->values = values;
  
  return array;
}

void SPerl_ARRAY_push(SPerl_ARRAY* array, const void* value) {
  size_t length = array->length;
  size_t capacity = array->capacity;
  
  if (length >= capacity) {
    if (capacity > SIZE_MAX / 2) {
      SPerl_ALLOCATOR_exit_with_malloc_failure();
    }
    size_t new_capacity = capacity * 2;
    array->values = (void**) SPerl_ALLOCATOR_safe_realloc(array->values, new_capacity, sizeof(void*));
    memset(array->values + capacity, 0, (new_capacity - capacity) * sizeof(void*));
    array->capacity = new_capacity;
  }
  
  /* Casting away a const qualification, I know what I'm doing. */
  array->values[length] = (void*) value;
  array->length++;
}

void* SPerl_ARRAY_fetch(SPerl_ARRAY* array, size_t index) {
  if (array == NULL || index >= array->length) {
    return NULL;
  }
  else {
    return array->values[index];
  }
}

void* SPerl_ARRAY_pop(SPerl_ARRAY* array) {
  if (array->length == 0) {
    return NULL;
  }

  array->length--;
  
  return array->values[array->length];
}

void SPerl_ARRAY_free(SPerl_ARRAY* array) {
  free(array->values);
  free(array);
}
