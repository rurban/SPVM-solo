#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "sperl_allocator_util.h"

void* SPerl_ALLOCATOR_UTIL_safe_malloc_i32(int32_t count, int32_t size) {
  
  assert(count >= 0);
  assert(size >= 0);
  assert(count <= INT32_MAX / size);
  
  int32_t block_size = count * size;
  void* block = malloc(block_size);
  
  if (!block) {
    fprintf(stderr, "Failed to allocate memory. Sperl will exit.\n");
    exit(EXIT_FAILURE);
  }
  
  return block;
}

void* SPerl_ALLOCATOR_UTIL_safe_realloc_i32(void* ptr, int32_t count, int32_t size) {

  assert(count >= 0);
  assert(size >= 0);
  assert(count <= INT32_MAX / size);

  int32_t block_size = count * size;
  void* block = realloc(ptr, block_size);
  
  if (!block) {
    fprintf(stderr, "Failed to reallocate memory. Sperl will exit.\n");
    exit(EXIT_FAILURE);
  }

  return block;
}
