#include "sperl_package.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"
#include "sperl_constant_pool.h"

SPerl_PACKAGE* SPerl_PACKAGE_new(SPerl_PARSER* parser) {
  SPerl_PACKAGE* package = SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_PACKAGE));
  
  package->op_constants = SPerl_ALLOCATOR_new_array(parser, 0);
  package->constant_pool = SPerl_CONSTANT_POOL_new();
  package->op_subs = SPerl_ALLOCATOR_new_array(parser, 0);
  package->op_fields = SPerl_ALLOCATOR_new_array(parser, 0);
  
  return package;
}
