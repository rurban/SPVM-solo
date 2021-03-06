#include "spvm_switch_info.h"
#include "spvm.h"
#include "spvm_allocator_parser.h"
#include "spvm_parser.h"

SPVM_SWITCH_INFO* SPVM_SWITCH_INFO_new(SPVM* spvm) {
  return SPVM_ALLOCATOR_PARSER_alloc_memory_pool(spvm, spvm->parser->allocator, sizeof(SPVM_SWITCH_INFO));
}
