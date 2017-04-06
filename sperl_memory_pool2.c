#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_memory_pool2.h"
#include "sperl_allocator_util.h"

SPerl_MEMORY_POOL2* SPerl_MEMORY_POOL2_new(SPerl* sperl, int64_t page_byte_size) {
  (void)sperl;
  
  SPerl_MEMORY_POOL2* memory_pool = (SPerl_MEMORY_POOL2*) SPerl_ALLOCATOR_UTIL_safe_malloc_zero(1, sizeof(SPerl_MEMORY_POOL2));
  
  if (page_byte_size == 0) {
    memory_pool->page_byte_size = 0xFF;
  }
  else {
    memory_pool->page_byte_size = page_byte_size;
  }
  
  uint8_t* page = SPerl_ALLOCATOR_UTIL_safe_malloc(memory_pool->page_byte_size, sizeof(uint8_t));
  memory_pool->pages_length = 1;
  memory_pool->pages = SPerl_ALLOCATOR_UTIL_safe_malloc(memory_pool->pages_length, sizeof(uint8_t*));
  memory_pool->pages[0] = page;
  memory_pool->current_page = 0;
  
  return memory_pool;
}

void* SPerl_MEMORY_POOL2_alloc(SPerl* sperl, SPerl_MEMORY_POOL2* memory_pool, int64_t byte_size) {
  (void)sperl;
  
  assert(byte_size > 0);
  assert(byte_size <= memory_pool->page_byte_size);
  
  int64_t pages_length = memory_pool->pages_length;
  int64_t page_byte_size = memory_pool->page_byte_size;
  int64_t current_page = memory_pool->current_page;
  
  uint8_t* next_address = (uint8_t*)&memory_pool->pages[current_page] + memory_pool->current_offset;
  
  memory_pool->current_offset += byte_size;
  
  // Adjust alignment
  int64_t aligned_byte_size = (byte_size - 1) + (sperl->alignment - ((byte_size - 1) % sperl->alignment));
  
  return next_address;
}

void SPerl_MEMORY_POOL2_free(SPerl* sperl, SPerl_MEMORY_POOL2* memory_pool) {
  (void)sperl;
  
  for (int64_t i = 0; i < memory_pool->pages_length; i++) {
    free(memory_pool->pages[i]);
  }
  free(memory_pool->pages);
  
  free(memory_pool);
}