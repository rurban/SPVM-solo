#include <stdint.h>

#include "sperl.h"
#include "sperl_allocator.h"
#include "sperl_env.h"

SPerl_ENV* SPerl_ENV_new(SPerl* sperl) {
  SPerl_ENV* env = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_ENV));

  env->call_stack_capacity = 255;
  env->call_stack = (int64_t*) SPerl_ALLOCATOR_safe_malloc(env->call_stack_capacity, sizeof(int64_t));
  env->abort = 0;
  
  return env;
}