#ifndef SPERL_ALLOCATOR_UTIL_H
#define SPERL_ALLOCATOR_UTIL_H

void* SPerl_ALLOCATOR_UTIL_safe_malloc_i32(int32_t count, int32_t size);
void* SPerl_ALLOCATOR_UTIL_safe_realloc_i32(void* ptr, int32_t count, int32_t size);
void* SPerl_ALLOCATOR_UTIL_safe_malloc_i64(int64_t count, int64_t size);

#endif
