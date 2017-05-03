#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "spvm.h"
#include "spvm_parser.h"
#include "spvm_bytecode_builder.h"
#include "spvm_bytecode.h"
#include "spvm_bytecode_array.h"
#include "spvm_constant.h"
#include "spvm_op.h"
#include "spvm_resolved_type.h"
#include "spvm_array.h"
#include "spvm_sub.h"
#include "spvm_var.h"
#include "spvm_my_var.h"
#include "spvm_allocator_parser.h"
#include "spvm_package.h"
#include "spvm_name_info.h"
#include "spvm_hash.h"
#include "spvm_field.h"
#include "spvm_switch_info.h"
#include "spvm_constant_pool.h"
#include "spvm_type.h"
#include "spvm_constant_pool_sub.h"
#include "spvm_limit.h"

void SPVM_BYTECODE_BUILDER_push_inc_bytecode(SPVM* spvm, SPVM_BYTECODE_ARRAY* bytecode_array, SPVM_OP* op_inc, int32_t value) {
  
  SPVM_VAR* var = op_inc->first->uv.var;
  SPVM_MY_VAR* my_var = var->op_my_var->uv.my_var;
  
  SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_inc);
  if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
    if (my_var->address > 0xFF || (value < -128 || value > 127)) {
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_WIDE);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BINC);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (my_var->address >> 8) & 0xFF);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, my_var->address & 0xFF);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (value >> 8) & 0xFF);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, value & 0xFF);
    }
    else {
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BINC);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, my_var->address);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, value);
    }
  }
  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
    if (my_var->address > 0xFF || (value < -128 || value > 127)) {
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_WIDE);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SINC);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (my_var->address >> 8) & 0xFF);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, my_var->address & 0xFF);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (value >> 8) & 0xFF);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, value & 0xFF);
    }
    else {
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SINC);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, my_var->address);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, value);
    }
  }
  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
    if (my_var->address > 0xFF || (value < -128 || value > 127)) {
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_WIDE);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IINC);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (my_var->address >> 8) & 0xFF);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, my_var->address & 0xFF);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (value >> 8) & 0xFF);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, value & 0xFF);
    }
    else {
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IINC);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, my_var->address);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, value);
    }
  }
  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
    if (my_var->address > 0xFF || (value < -128 || value > 127)) {
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_WIDE);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LINC);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (my_var->address >> 8) & 0xFF);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, my_var->address & 0xFF);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (value >> 8) & 0xFF);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, value & 0xFF);
    }
    else {
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LINC);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, my_var->address);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, value);
    }
  }
}

void SPVM_BYTECODE_BUILDER_push_load_bytecode(SPVM* spvm, SPVM_BYTECODE_ARRAY* bytecode_array, SPVM_OP* op_var) {
  
  (void)spvm;
  
  SPVM_VAR* var = op_var->uv.var;
  
  int32_t my_var_address = var->op_my_var->uv.my_var->address;

  if (my_var_address > 0xFF) {
    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_WIDE);
  }
  
  _Bool has_operand = 0;
  if (my_var_address == 0) {
    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LOAD_0);
  }
  else if (my_var_address == 1) {
    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LOAD_1);
  }
  else if (my_var_address == 2) {
    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LOAD_2);
  }
  else if (my_var_address == 3) {
    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LOAD_3);
  }
  else {
    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LOAD);
    has_operand = 1;
  }
  
  if (has_operand) {
    if (my_var_address > 0xFF) {
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (my_var_address >> 8) & 0xFF);
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, my_var_address);
    }
    else {
      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, my_var_address);
    }
  }
}

void SPVM_BYTECODE_BUILDER_build_bytecode_array(SPVM* spvm) {
  
  // Parser
  SPVM_PARSER* parser = spvm->parser;
  
  // Bytecode
  SPVM_BYTECODE_ARRAY* bytecode_array = spvm->bytecode_array;
  
  for (int32_t package_pos = 0; package_pos < parser->op_packages->length; package_pos++) {
    SPVM_OP* op_package = SPVM_ARRAY_fetch(spvm, parser->op_packages, package_pos);
    SPVM_PACKAGE* package = op_package->uv.package;
    
    for (int32_t sub_pos = 0; sub_pos < package->op_subs->length; sub_pos++) {
      SPVM_OP* op_sub = SPVM_ARRAY_fetch(spvm, package->op_subs, sub_pos);
      SPVM_SUB* sub = op_sub->uv.sub;
      
      if (sub->is_constant || sub->is_native) {
        continue;
      }
      
      sub->bytecode_base = bytecode_array->length;
      
      // Run OPs
      SPVM_OP* op_base = SPVM_OP_get_op_block_from_op_sub(spvm, op_sub);
      SPVM_OP* op_cur = op_base;
      _Bool finish = 0;
      
      // IFXXX Bytecode address(except loop)
      SPVM_ARRAY* if_address_stack = SPVM_ALLOCATOR_PARSER_alloc_array(spvm, parser->allocator, 0);

      // GOTO Bytecode address for last
      SPVM_ARRAY* goto_last_address_stack = SPVM_ALLOCATOR_PARSER_alloc_array(spvm, parser->allocator, 0);

      // GOTO Bytecode address for end of if block
      SPVM_ARRAY* goto_if_block_end_address_stack = SPVM_ALLOCATOR_PARSER_alloc_array(spvm, parser->allocator, 0);
      
      // GOTO bytecode address for loop start
      SPVM_ARRAY* goto_loop_start_address_stack = SPVM_ALLOCATOR_PARSER_alloc_array(spvm, parser->allocator, 0);
      
      // GOTO exception handler address
      SPVM_ARRAY* goto_exception_handler_stack = SPVM_ALLOCATOR_PARSER_alloc_array(spvm, parser->allocator, 0);

      // try stack
      SPVM_ARRAY* try_stack = SPVM_ALLOCATOR_PARSER_alloc_array(spvm, parser->allocator, 0);
      
      // Current switch
      SPVM_OP* cur_op_switch_info = NULL;
      
      // Current case addresses
      int32_t cur_switch_address = -1;
      
      int32_t cur_default_address = -1;
      SPVM_ARRAY* cur_case_addresses = NULL;
      SPVM_ARRAY* cur_op_cases = NULL;
      
      while (op_cur) {
        // [START]Preorder traversal position
        
        switch (op_cur->code) {
          case SPVM_OP_C_CODE_TRY: {
            
            SPVM_ARRAY_push(spvm, try_stack, op_cur);
            
            break;
          }
          case SPVM_OP_C_CODE_SWITCH: {
            cur_op_switch_info = op_cur;
            cur_case_addresses = SPVM_ALLOCATOR_PARSER_alloc_array(spvm, parser->allocator, 0);
            cur_op_cases = SPVM_ALLOCATOR_PARSER_alloc_array(spvm, parser->allocator, 0);
            break;
          }
          case SPVM_OP_C_CODE_BLOCK: {
            if (op_cur->flag & SPVM_OP_C_FLAG_BLOCK_LOOP) {
              // Add goto
              SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_GOTO);
              
              int32_t* address_ptr = SPVM_ALLOCATOR_PARSER_alloc_int(spvm, parser->allocator);
              *address_ptr = bytecode_array->length - 1;
              
              SPVM_ARRAY_push(spvm, goto_loop_start_address_stack, address_ptr);
              
              SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
              SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
            }
          }
        }
        
        // [END]Preorder traversal position
        
        if (op_cur->first) {
          op_cur = op_cur->first;
        }
        else {
          while (1) {
            // [START]Postorder traversal position
            switch (op_cur->code) {
              case SPVM_OP_C_CODE_CATCH: {
                if (try_stack->length > 0) {
                  SPVM_ARRAY_pop(spvm, try_stack);
                }
                
                int32_t pop_count = goto_exception_handler_stack->length;
                for (int32_t i = 0; i < pop_count; i++) {
                  assert(goto_exception_handler_stack->length > 0);
                  
                  int32_t* address_ptr = SPVM_ARRAY_pop(spvm, goto_exception_handler_stack);
                  int32_t address = *address_ptr;
                  
                  int32_t jump_offset = bytecode_array->length - address;
                  
                  bytecode_array->values[address + 1] = (jump_offset >> 8) & 0xFF;
                  bytecode_array->values[address + 2] = jump_offset & 0xFF;
                }
                
                break;
              }
              case SPVM_OP_C_CODE_SWITCH_CONDITION: {
                
                SPVM_SWITCH_INFO* switch_info = cur_op_switch_info->uv.switch_info;
                
                // tableswitch
                if (switch_info->code == SPVM_SWITCH_INFO_C_CODE_TABLESWITCH) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_TABLESWITCH);
                  
                  cur_switch_address = bytecode_array->length - 1;
                  
                  int64_t max = switch_info->max;
                  int64_t min = switch_info->min;
                  
                  // Machine address to calculate padding
                  cur_switch_address = bytecode_array->length - 1;
                  
                  // Padding
                  int32_t padding = ((int32_t)sizeof(int64_t) - 1) - (cur_switch_address % (int32_t)sizeof(int64_t));
                  
                  for (int32_t i = 0; i < padding; i++) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                  }
                  
                  // Default
                  for (int32_t i = 0; i < (int32_t)sizeof(int64_t); i++) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                  }
                  
                  // Low
                  for (int32_t i = 0; i < (int32_t)sizeof(int64_t); i++) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                  }
                  *(int64_t*)&bytecode_array->values[bytecode_array->length - sizeof(int64_t)] = min;
                  
                  // Low
                  for (int32_t i = 0; i < (int32_t)sizeof(int64_t); i++) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                  }
                  *(int64_t*)&bytecode_array->values[bytecode_array->length - sizeof(int64_t)] = max;
                  
                  // Offsets
                  for (int64_t i = 0; i < (max - min + 1) * (int64_t)sizeof(int64_t); i++) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                  }
                }
                // lookupswitch
                else if (switch_info->code == SPVM_SWITCH_INFO_C_CODE_LOOKUPSWITCH) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LOOKUPSWITCH);
                  
                  if (switch_info->op_cases->length > SPVM_LIMIT_C_CASES) {
                    fprintf(stderr, "Invalid AST: too many cases in switch statement\n");
                    exit(1);
                  }
                  int32_t const length = switch_info->op_cases->length;

                  // Machine address to calculate padding
                  cur_switch_address = bytecode_array->length - 1;
                  
                  // Padding
                  int32_t padding = (sizeof(int64_t) - 1) - (cur_switch_address % sizeof(int64_t));
                  
                  for (int32_t i = 0; i < padding; i++) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                  }
                  
                  // Default
                  for (int32_t i = 0; i < (int32_t)sizeof(int64_t); i++) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                  }
                  
                  // Case count
                  for (int32_t i = 0; i < (int32_t)sizeof(int64_t); i++) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                  }
                  *(int64_t*)&bytecode_array->values[bytecode_array->length - (int32_t)sizeof(int64_t)] = length;
                  
                  int32_t size_of_match_offset_pairs = length * (int32_t)sizeof(int64_t) * 2;
                  for (int32_t i = 0; i < size_of_match_offset_pairs; i++) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                  }
                }
                
                break;
              }
              case SPVM_OP_C_CODE_CASE: {
                
                int32_t* address_ptr = SPVM_ALLOCATOR_PARSER_alloc_int(spvm, parser->allocator);
                *address_ptr = bytecode_array->length;
                
                SPVM_ARRAY_push(spvm, cur_case_addresses, address_ptr);
                SPVM_ARRAY_push(spvm, cur_op_cases, op_cur);
                
                break;
              }
              case SPVM_OP_C_CODE_DEFAULT: {
                
                cur_default_address = bytecode_array->length;
                
                break;
              }
              case SPVM_OP_C_CODE_SWITCH: {
                
                SPVM_SWITCH_INFO* switch_info = cur_op_switch_info->uv.switch_info;
                
                // tableswitch
                if (switch_info->code == SPVM_SWITCH_INFO_C_CODE_TABLESWITCH) {
                  int32_t padding = (sizeof(int64_t) - 1) - (cur_switch_address % sizeof(int64_t));
                  
                  
                  // Default offset
                  int64_t default_offset;
                  if (cur_default_address == -1) {
                    default_offset = bytecode_array->length - cur_switch_address;
                  }
                  else {
                    default_offset = cur_default_address - cur_switch_address;
                  }
                  bytecode_array->values[cur_switch_address + padding + 1] = *(int64_t*)&default_offset;
                  
                  // min
                  int64_t min = *(int64_t*)&bytecode_array->values[cur_switch_address + padding + sizeof(int64_t) + 1];
                  
                  // max
                  int64_t max = *(int64_t*)&bytecode_array->values[cur_switch_address + padding + sizeof(int64_t) * 2 + 1];
                  
                  int64_t length = max - min + 1;
                  
                  int64_t case_pos = 0;
                  for (int64_t i = 0; i < length; i++) {
                    SPVM_OP* op_case = SPVM_ARRAY_fetch(spvm, cur_op_cases, case_pos);
                    SPVM_OP* op_constant = op_case->first;
                    if (op_constant->uv.constant->uv.long_value - min == i) {
                      // Case
                      int64_t* case_address_ptr = SPVM_ARRAY_fetch(spvm, cur_case_addresses, case_pos);
                      int64_t case_address = *case_address_ptr;
                      int64_t case_offset = case_address - cur_switch_address;
                      
                      *(int64_t*)&bytecode_array->values[cur_switch_address + padding + sizeof(int64_t) * 3 + 1 + (sizeof(int64_t) * i)] = case_offset;
                      
                      case_pos++;
                    }
                    else {
                      // Default
                      *(int64_t*)&bytecode_array->values[cur_switch_address + padding + sizeof(int64_t) * 3 + 1 + (sizeof(int64_t) * i)] = default_offset;
                    }
                  }
                }
                // lookupswitch
                else if (switch_info->code == SPVM_SWITCH_INFO_C_CODE_LOOKUPSWITCH) {
                  int64_t padding = (sizeof(int64_t) - 1) - (cur_switch_address % sizeof(int64_t));

                  // Default offset
                  int64_t default_offset;
                  if (cur_default_address == -1) {
                    default_offset = bytecode_array->length - cur_switch_address;
                  }
                  else {
                    default_offset = cur_default_address - cur_switch_address;
                  }
                  *(int64_t*)&bytecode_array->values[cur_switch_address + padding + 1] = default_offset;
                  
                  // Note: Here it's assumed that the number of cases can be expressed by a int64_t variable.
                  if (switch_info->op_cases->length > SPVM_LIMIT_C_CASES) {
                    fprintf(stderr, "Invalid AST: too many cases in switch statement\n");
                    exit(1);
                  }
                  int64_t const length = (int64_t) switch_info->op_cases->length;
                  
                  SPVM_ARRAY* ordered_op_cases = SPVM_ALLOCATOR_PARSER_alloc_array(spvm, parser->allocator, 0);
                  for (int64_t i = 0; i < length; i++) {
                    SPVM_OP* op_case = SPVM_ARRAY_fetch(spvm, cur_op_cases, i);
                    SPVM_ARRAY_push(spvm, ordered_op_cases, op_case);
                  }
                  SPVM_ARRAY* ordered_case_addresses = SPVM_ALLOCATOR_PARSER_alloc_array(spvm, parser->allocator, 0);
                  for (int64_t i = 0; i < length; i++) {
                    int64_t* case_address_ptr = SPVM_ARRAY_fetch(spvm, cur_case_addresses, i);
                    SPVM_ARRAY_push(spvm, ordered_case_addresses, case_address_ptr);
                  }
                  
                  // sort by asc order
                  for (int64_t i = 0; i < length; i++) {
                    for (int64_t j = i + 1; j < length; j++) {
                      SPVM_OP* op_case_i = SPVM_ARRAY_fetch(spvm, ordered_op_cases, i);
                      SPVM_OP* op_case_j = SPVM_ARRAY_fetch(spvm, ordered_op_cases, j);
                      int64_t match_i = op_case_i->first->uv.constant->uv.long_value;
                      int64_t match_j = op_case_j->first->uv.constant->uv.long_value;
                      
                      int64_t* case_address_i = SPVM_ARRAY_fetch(spvm, ordered_case_addresses, i);
                      int64_t* case_address_j = SPVM_ARRAY_fetch(spvm, ordered_case_addresses, j);
                      
                      if (match_i > match_j) {
                        SPVM_ARRAY_store(spvm, ordered_op_cases, i, op_case_j);
                        SPVM_ARRAY_store(spvm, ordered_op_cases, j, op_case_i);
                        
                        SPVM_ARRAY_store(spvm, ordered_case_addresses, i, case_address_j);
                        SPVM_ARRAY_store(spvm, ordered_case_addresses, j, case_address_i);
                      }
                    }
                  }
                  
                  for (int64_t i = 0; i < length; i++) {
                    SPVM_OP* op_case = SPVM_ARRAY_fetch(spvm, ordered_op_cases, i);
                    SPVM_OP* op_constant = op_case->first;
                    int64_t match = op_constant->uv.constant->uv.long_value;

                    int64_t* case_address_ptr = SPVM_ARRAY_fetch(spvm, ordered_case_addresses, i);
                    int64_t case_address = *case_address_ptr;
                    int64_t case_offset = case_address - cur_switch_address;
                    
                    // Match
                    *(int64_t*)&bytecode_array->values[cur_switch_address + padding + sizeof(int64_t) * 2 + 1 + (sizeof(int64_t) * 2 * i)] = match;

                    // Offset
                    *(int64_t*)&bytecode_array->values[cur_switch_address + padding + sizeof(int64_t) * 3 + 1 + (sizeof(int64_t) * 2 * i)] = case_offset;
                  }
                }
                
                cur_op_switch_info = NULL;
                cur_default_address = -1;
                cur_case_addresses = NULL;
                
                break;
              }
              case SPVM_OP_C_CODE_CALL_FIELD: {
                
                if (!op_cur->lvalue) {
                  SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur);
                  
                  if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE)
                  {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BGETFIELD);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SGETFIELD);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IGETFIELD);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LGETFIELD);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FGETFIELD);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DGETFIELD);
                  }
                  else {
                    assert(0);
                  }
                  
                  SPVM_NAME_INFO* name_info = op_cur->uv.name_info;
                  const char* field_name = name_info->resolved_name;
                  SPVM_OP* op_field = SPVM_HASH_search(spvm, parser->op_field_symtable, field_name, strlen(field_name));
                  SPVM_FIELD* field = op_field->uv.field;
                  
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (field->constant_pool_address >> 24) & 0xFF);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (field->constant_pool_address >> 16) & 0xFF);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (field->constant_pool_address >> 8) & 0xFF);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, field->constant_pool_address & 0xFF);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_CALL_SUB: {
                
                // Call subroutine
                SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_INVOKESTATIC_WW);
                SPVM_NAME_INFO* name_info = op_cur->uv.name_info;
                const char* sub_name = name_info->resolved_name;
                
                SPVM_OP* op_sub = SPVM_HASH_search(spvm, parser->op_sub_symtable, sub_name, strlen(sub_name));
                SPVM_SUB* sub = op_sub->uv.sub;
                
                int32_t constant_pool_address = sub->constant_pool_address;
                
                SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (constant_pool_address >> 24) & 0xFF);
                SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (constant_pool_address >> 16) & 0xFF);
                SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (constant_pool_address >> 8) & 0xFF);
                SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, constant_pool_address & 0xFF);
                
                //  Goto exception handler
                if (try_stack->length > 0) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_GOTO);
                  
                  int32_t* address_ptr = SPVM_ALLOCATOR_PARSER_alloc_int(spvm, parser->allocator);
                  *address_ptr = bytecode_array->length - 1;
                  SPVM_ARRAY_push(spvm, goto_exception_handler_stack, address_ptr);
                  
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                }
                // Rethrow exception
                else {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ATHROW);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_DIE: {
                SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ATHROW);
                break;
              }
              case SPVM_OP_C_CODE_LAST: {
                // Add goto
                SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_GOTO);
                
                int32_t* address_ptr = SPVM_ALLOCATOR_PARSER_alloc_int(spvm, parser->allocator);
                *address_ptr = bytecode_array->length - 1;
                
                SPVM_ARRAY_push(spvm, goto_last_address_stack, address_ptr);
                
                SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                break;
              }
              case SPVM_OP_C_CODE_NEXT: {
                int32_t* address_ptr = SPVM_ARRAY_fetch(spvm, goto_loop_start_address_stack, goto_loop_start_address_stack->length - 1);
                int32_t address = *address_ptr;
                
                // Add "goto"
                SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_GOTO);
                
                // Jump offset
                int32_t jump_offset = address - (bytecode_array->length - 1) + 3;
                
                SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (jump_offset >> 8) & 0xFF);
                SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, jump_offset & 0xFF);
                
                break;
              }
              case SPVM_OP_C_CODE_BLOCK: {
                if (op_cur->flag & SPVM_OP_C_FLAG_BLOCK_IF) {
                  
                  // Has else block
                  if (op_cur->flag & SPVM_OP_C_FLAG_BLOCK_HAS_ELSE) {
                    // Prepare to jump to end of else block
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_GOTO);
                    
                    int32_t* address_ptr = SPVM_ALLOCATOR_PARSER_alloc_int(spvm, parser->allocator);
                    *address_ptr = bytecode_array->length - 1;
                    SPVM_ARRAY_push(spvm, goto_if_block_end_address_stack, address_ptr);
                    
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                  }

                  assert(if_address_stack->length > 0);

                  // Set if jump address
                  int32_t* address_ptr = SPVM_ARRAY_pop(spvm, if_address_stack);
                  int32_t address = *address_ptr;
                  
                  // Jump offset
                  int32_t jump_offset = bytecode_array->length - address;
                  
                  // Set jump offset
                  bytecode_array->values[address + 1] = (jump_offset >> 8) & 0xFF;
                  bytecode_array->values[address + 2] = jump_offset & 0xFF;
                }
                else if (op_cur->flag & SPVM_OP_C_FLAG_BLOCK_ELSE) {
                  
                  assert(goto_if_block_end_address_stack->length > 0);
                  
                  int32_t* address_ptr = SPVM_ARRAY_pop(spvm, goto_if_block_end_address_stack);
                  int32_t address = *address_ptr;
                  
                  // Jump offset
                  int32_t jump_offset = bytecode_array->length - address;
                  
                  // Set jump offset
                  bytecode_array->values[address + 1] = (jump_offset >> 8) & 0xFF;
                  bytecode_array->values[address + 2] = jump_offset & 0xFF;
                }
                else if (op_cur->flag & SPVM_OP_C_FLAG_BLOCK_LOOP) {
                  
                  int32_t* goto_loop_start_address_ptr = SPVM_ARRAY_fetch(spvm, goto_loop_start_address_stack, goto_loop_start_address_stack->length - 1);
                  int32_t goto_loop_start_address = *goto_loop_start_address_ptr;
                  
                  // Jump offset
                  int32_t goto_loop_start_offset = bytecode_array->length - goto_loop_start_address;
                  
                  bytecode_array->values[goto_loop_start_address + 1] = (goto_loop_start_offset >> 8) & 0xFF;
                  bytecode_array->values[goto_loop_start_address + 2] = goto_loop_start_offset & 0xFF;
                }
                break;
              }
              case SPVM_OP_C_CODE_IF: {
                
                break;
              }
              case SPVM_OP_C_CODE_LOOP: {
                
                // Set last position
                while (goto_last_address_stack->length > 0) {
                  
                  int32_t* goto_last_address_ptr = SPVM_ARRAY_pop(spvm, goto_last_address_stack);
                  int32_t goto_last_address = *goto_last_address_ptr;
                  
                  // Last offset
                  int32_t goto_last_offset = bytecode_array->length - goto_last_address;
                  
                  bytecode_array->values[goto_last_address + 1] = (goto_last_offset >> 8) & 0xFF;
                  bytecode_array->values[goto_last_address + 2] = goto_last_offset & 0xFF;
                }
                
                break;
              }
              case SPVM_OP_C_CODE_CONDITION: {
                SPVM_OP* op_condition_target = op_cur->first;

                if (op_condition_target->code == SPVM_OP_C_CODE_UNDEF) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFNONNULL);
                }
                else if (op_condition_target->code == SPVM_OP_C_CODE_EQ) {
                  SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_condition_target->first);
                  if (!resolved_type) {
                    resolved_type = SPVM_OP_get_resolved_type(spvm, op_condition_target->last);
                  }
                  
                  if (!resolved_type) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IF_ACMPNE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BCMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFNE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SCMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFNE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ICMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFNE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IF_LCMPNE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FCMPG);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFNE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DCMPG);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFNE);
                  }
                  else {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IF_ACMPNE);
                  }
                }
                else if (op_condition_target->code == SPVM_OP_C_CODE_NE) {
                  SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_condition_target->first);
                  if (!resolved_type) {
                    resolved_type = SPVM_OP_get_resolved_type(spvm, op_condition_target->last);
                  }
                  
                  if (!resolved_type) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IF_ACMPEQ);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BCMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFEQ);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SCMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFEQ);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ICMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFEQ);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IF_LCMPEQ);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FCMPG);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFEQ);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DCMPG);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFEQ);
                  }
                  else {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IF_ACMPEQ);
                  }
                }
                else if (op_condition_target->code == SPVM_OP_C_CODE_GT) {
                  
                  SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_condition_target->first);
                  if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BCMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFLE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SCMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFLE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ICMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFLE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IF_LCMPLE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FCMPL);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFLE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DCMPL);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFLE);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (op_condition_target->code == SPVM_OP_C_CODE_GE) {
                  SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_condition_target->first);
                  if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BCMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFLT);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SCMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFLT);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ICMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFLT);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IF_LCMPLT);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FCMPL);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFLT);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DCMPL);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFLT);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (op_condition_target->code == SPVM_OP_C_CODE_LT) {
                  SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_condition_target->first);
                  if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BCMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFGE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SCMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFGE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ICMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFGE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IF_LCMPGE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FCMPG);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFGE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DCMPG);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFGE);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (op_condition_target->code == SPVM_OP_C_CODE_LE) {
                  SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_condition_target->first);
                  if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BCMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFGT);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SCMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFGT);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ICMP);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFGT);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IF_LCMPGT);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FCMPG);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFGT);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DCMPG);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFGT);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (op_condition_target) {
                  
                  if (op_condition_target->code == SPVM_OP_C_CODE_IF) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFNE);
                  }
                  else {
                    SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_condition_target);
                    if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_B2L);
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFNE);
                    }
                    else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_S2L);
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFNE);
                    }
                    else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_I2L);
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFNE);
                    }
                    else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFNE);
                    }
                    else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_F2L);
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFNE);
                    }
                    else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_D2L);
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFNE);
                    }
                    else {
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IFNONNULL);
                    }
                  }
                }
                
                int32_t address = bytecode_array->length - 1;
                
                if (op_cur->flag & SPVM_OP_C_FLAG_CONDITION_IF) {
                  int32_t* address_ptr = SPVM_ALLOCATOR_PARSER_alloc_int(spvm, parser->allocator);
                  *address_ptr = address;
                  
                  SPVM_ARRAY_push(spvm, if_address_stack, address_ptr);
                  
                  // Prepare for bytecode position of branch
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, 0);
                }
                else if (op_cur->flag & SPVM_OP_C_FLAG_CONDITION_LOOP) {
                  assert(goto_loop_start_address_stack->length > 0);
                  
                  int32_t* goto_loop_start_address_ptr = SPVM_ARRAY_pop(spvm, goto_loop_start_address_stack);
                  int32_t goto_loop_start_address = *goto_loop_start_address_ptr;
                  
                  // Jump offset
                  int32_t goto_loop_start_offset = goto_loop_start_address - (bytecode_array->length - 1) + 3;
                  
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (goto_loop_start_offset >> 8) & 0xFF);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, goto_loop_start_offset & 0xFF);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_ARRAY_LENGTH : {
                SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ARRAYLENGTH);
                
                break;
              }
              case SPVM_OP_C_CODE_LEFT_SHIFT: {
                SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur);
                
                if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BSHL);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SSHL);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ISHL);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LSHL);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_RIGHT_SHIFT: {
                SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur);
                
                if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BSHR);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SSHR);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ISHR);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LSHR);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_RIGHT_SHIFT_UNSIGNED: {
                SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur);
                
                if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BUSHR);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SUSHR);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IUSHR);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LUSHR);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_MALLOC: {
                SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur->first);
                
                if (SPVM_RESOLVED_TYPE_is_core_array(spvm, resolved_type)) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_MALLOC_ARRAY);
                  
                  if (strcmp(resolved_type->name, "byte[]") == 0) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_RESOLVED_TYPE_C_ID_BYTE);
                  }
                  else if (strcmp(resolved_type->name, "short[]") == 0) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_RESOLVED_TYPE_C_ID_SHORT);
                  }
                  else if (strcmp(resolved_type->name, "int[]") == 0) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_RESOLVED_TYPE_C_ID_INT);
                  }
                  else if (strcmp(resolved_type->name, "long[]") == 0) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_RESOLVED_TYPE_C_ID_LONG);
                  }
                  else if (strcmp(resolved_type->name, "float[]") == 0) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_RESOLVED_TYPE_C_ID_FLOAT);
                  }
                  else if (strcmp(resolved_type->name, "double[]") == 0) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_RESOLVED_TYPE_C_ID_DOUBLE);
                  }
                  else if (strcmp(resolved_type->name, "string[]") == 0) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_RESOLVED_TYPE_C_ID_STRING);
                  }
                  else {
                    assert(0);
                  }
                }
                else {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_MALLOC_OBJECT);
                  
                  const char* package_name = op_cur->first->uv.type->resolved_type->name;
                  
                  SPVM_OP* op_package = SPVM_HASH_search(spvm, parser->op_package_symtable, package_name, strlen(package_name));
                  SPVM_PACKAGE* package = op_package->uv.package;
                  
                  int32_t constant_pool_address = package->constant_pool_address;
                  
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (constant_pool_address >> 24) & 0xFF);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (constant_pool_address >> 16) & 0xFF);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (constant_pool_address >> 8) & 0xFF);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, constant_pool_address & 0xFF);
                }
                break;
              }
              
              case SPVM_OP_C_CODE_UNDEF: {
                
                SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ACONST_NULL);
                
                break;
              }
              case SPVM_OP_C_CODE_PRE_INC: {
                SPVM_BYTECODE_BUILDER_push_inc_bytecode(spvm, bytecode_array, op_cur, 1);
                SPVM_BYTECODE_BUILDER_push_load_bytecode(spvm, bytecode_array, op_cur->first);
                
                break;
              }
              case SPVM_OP_C_CODE_POST_INC: {
                SPVM_BYTECODE_BUILDER_push_load_bytecode(spvm, bytecode_array, op_cur->first);
                SPVM_BYTECODE_BUILDER_push_inc_bytecode(spvm, bytecode_array, op_cur, 1);
                
                break;
              }
              case SPVM_OP_C_CODE_PRE_DEC: {
                SPVM_BYTECODE_BUILDER_push_inc_bytecode(spvm, bytecode_array, op_cur, -1);
                SPVM_BYTECODE_BUILDER_push_load_bytecode(spvm, bytecode_array, op_cur->first);
                
                break;
              }
              case SPVM_OP_C_CODE_POST_DEC: {
                SPVM_BYTECODE_BUILDER_push_load_bytecode(spvm, bytecode_array, op_cur->first);
                SPVM_BYTECODE_BUILDER_push_inc_bytecode(spvm, bytecode_array, op_cur, -1);
                
                break;
              }
              case SPVM_OP_C_CODE_BIT_XOR: {
                
                SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur);
                if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BXOR);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SXOR);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IXOR);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LXOR);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_BIT_OR: {
                
                SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur);
                if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BOR);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SOR);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IOR);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LOR);
                }
                              
                break;
              }
              case SPVM_OP_C_CODE_BIT_AND: {
                SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur);
                
                if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BAND);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SAND);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IAND);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LAND);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_ARRAY_ELEM: {
                
                if (op_cur->lvalue) {
                  break;
                }
                
                SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur);
                SPVM_RESOLVED_TYPE* last_resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur->last);
                
                if (last_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                  if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE)
                  {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BALOAD);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SALOAD);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IALOAD);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LALOAD);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FALOAD);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DALOAD);
                  }
                  else {
                    assert(0);
                  }
                }
                else {
                  assert(0);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_INCREFCOUNT: {
                
                SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_INCREFCOUNT);
                
                break;
              }
              case SPVM_OP_C_CODE_DECREFCOUNT: {
                
                SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DECREFCOUNT);
                
                break;
              }
              case SPVM_OP_C_CODE_ASSIGN: {
                
                if (op_cur->first->code == SPVM_OP_C_CODE_VAR) {
                  SPVM_OP* op_var = op_cur->first;
                  int32_t my_var_address = op_var->uv.var->op_my_var->uv.my_var->address;
                  
                  SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_var);
                  
                  if (my_var_address > 0xFF) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_WIDE);
                  }
                  
                  _Bool has_operand = 0;
                  
                  if (SPVM_RESOLVED_TYPE_is_core_value(spvm, resolved_type)) {
                    if (my_var_address == 0) {
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_STORE_0);
                    }
                    else if (my_var_address == 1) {
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_STORE_1);
                    }
                    else if (my_var_address == 2) {
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_STORE_2);
                    }
                    else if (my_var_address == 3) {
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_STORE_3);
                    }
                    else {
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_STORE);
                      has_operand = 1;
                    }
                  }
                  else {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ASTORE);
                    has_operand = 1;
                  }
                  
                  if (has_operand) {
                    if (my_var_address > 0xFF) {
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (my_var_address >> 8) & 0xFF);
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, my_var_address);
                    }
                    else {
                      SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, my_var_address);
                    }
                  }
                }
                else if (op_cur->first->code == SPVM_OP_C_CODE_ARRAY_ELEM) {
                  
                  SPVM_OP* op_array_elem = op_cur->first;
                  SPVM_OP* op_term_index = op_array_elem->last;
                  
                  SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_array_elem);
                  SPVM_RESOLVED_TYPE* index_resolved_type = SPVM_OP_get_resolved_type(spvm, op_term_index);
                  
                  assert(index_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG);
                  
                  if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BASTORE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SASTORE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IASTORE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LASTORE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FASTORE);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DASTORE);
                  }
                  else {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_AASTORE);
                  }
                }
                else if (op_cur->first->code == SPVM_OP_C_CODE_CALL_FIELD) {
                  SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur->first);
                  
                  if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BPUTFIELD);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SPUTFIELD);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IPUTFIELD);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LPUTFIELD);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FPUTFIELD);
                  }
                  else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DPUTFIELD);
                  }
                  else {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_APUTFIELD);
                  }
                  
                  // Call subroutine
                  SPVM_NAME_INFO* name_info = op_cur->first->uv.name_info;
                  const char* field_name = name_info->resolved_name;
                  SPVM_OP* op_field = SPVM_HASH_search(spvm, parser->op_field_symtable, field_name, strlen(field_name));
                  SPVM_FIELD* field = op_field->uv.field;
                  
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (field->constant_pool_address >> 24) & 0xFF);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (field->constant_pool_address >> 16) & 0xFF);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (field->constant_pool_address >> 8) & 0xFF);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, field->constant_pool_address & 0xFF);
                }
                
                break;
              }
              
              case SPVM_OP_C_CODE_RETURN: {
                
                SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur);
                if (!resolved_type) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_RETURN_VOID);
                }
                else {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_RETURN_VALUE);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_NEGATE: {
                
                SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur);
                if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BNEG);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SNEG);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_INEG);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LNEG);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FNEG);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DNEG);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_CONVERT: {
                
                SPVM_OP* op_src_term = op_cur->first;
                SPVM_OP* op_dist_type = op_cur->last;
                
                SPVM_RESOLVED_TYPE* src_resolved_type = SPVM_OP_get_resolved_type(spvm, op_src_term);
                SPVM_RESOLVED_TYPE* dist_resolved_type = SPVM_OP_get_resolved_type(spvm, op_dist_type);
                
                if (src_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                  if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                    // None
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_B2S);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_B2I);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_B2L);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_B2F);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_B2D);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (src_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                  if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_S2B);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                    // None
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_S2I);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_S2L);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_S2F);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_S2D);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (src_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                  if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_I2B);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_I2S);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                    // None
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_I2L);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_I2F);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_I2D);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (src_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                  if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_L2B);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_L2S);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_L2I);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                    // None
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_L2F);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_L2D);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (src_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                  if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_F2B);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_F2S);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_F2I);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_F2L);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                    // None
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_F2D);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (src_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                  if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_D2B);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_D2S);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_D2I);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_D2L);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_D2F);
                  }
                  else if (dist_resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                    // None
                  }
                  else {
                    assert(0);
                  }
                }
                else {
                  assert(0);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_POP: {
                
                SPVM_OP* op_first = op_cur->first;
                
                if (op_first->code != SPVM_OP_C_CODE_ASSIGN && op_first->code != SPVM_OP_C_CODE_RETURN && !op_first->lvalue) {
                  SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_first);
                  
                  if (SPVM_RESOLVED_TYPE_is_core_value(spvm, resolved_type)) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_POP);
                  }
                  else {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_APOP);
                  }
                }
                
                break;
              }
              case SPVM_OP_C_CODE_ADD: {
                
                SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur);
                if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BADD);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SADD);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IADD);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LADD);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FADD);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DADD);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_SUBTRACT: {
                
                SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur);
                if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BSUB);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SSUB);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ISUB);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LSUB);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FSUB);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DSUB);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_MULTIPLY: {
                
                SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur);
                if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BMUL);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SMUL);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IMUL);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LMUL);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FMUL);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DMUL);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_DIVIDE: {
                
                SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur);
                if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BDIV);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SDIV);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IDIV);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LDIV);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FDIV);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DDIV);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_REMAINDER: {
                
                SPVM_RESOLVED_TYPE* resolved_type = SPVM_OP_get_resolved_type(spvm, op_cur);
                if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BREM);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SREM);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_INT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_IREM);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LREM);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FREM);
                }
                else if (resolved_type->id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DREM);
                }
                
                break;
              }
              case SPVM_OP_C_CODE_VAR: {
                if (op_cur->lvalue) {
                  break;
                }
                
                SPVM_BYTECODE_BUILDER_push_load_bytecode(spvm, bytecode_array, op_cur);
                
                break;
              }
              case SPVM_OP_C_CODE_CONSTANT: {
                SPVM_CONSTANT* constant = op_cur->uv.constant;
                
                if (op_cur->flag == SPVM_OP_C_FLAG_CONSTANT_CASE) {
                  break;
                }
                
                _Bool bytecode_set = 0;
                if (constant->code == SPVM_CONSTANT_C_CODE_BYTE) {
                  if (constant->uv.byte_value == 0) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BCONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.byte_value == 1) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BCONST_1);
                    bytecode_set = 1;
                  }
                  else {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BBPUSH);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, constant->uv.byte_value & 0xFF);
                    bytecode_set = 1;
                  }
                }
                else if (constant->code == SPVM_CONSTANT_C_CODE_SHORT) {
                  if (constant->uv.short_value == 0) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SCONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.short_value == 1) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SCONST_1);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.short_value >= -128 && constant->uv.short_value <= 127) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BSPUSH);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, constant->uv.short_value & 0xFF);
                    bytecode_set = 1;
                  }
                  else {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SSPUSH);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (constant->uv.short_value >> 8) & 0xFF);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, constant->uv.short_value & 0xFF);
                    bytecode_set = 1;
                  }
                }
                else if (constant->code == SPVM_CONSTANT_C_CODE_INT) {
                  if (constant->uv.int_value == -1) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ICONST_M1);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 0) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ICONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 1) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ICONST_1);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 2) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ICONST_2);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 3) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ICONST_3);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 4) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ICONST_4);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 5) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_ICONST_5);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value >= -128 && constant->uv.int_value <= 127) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BIPUSH);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, constant->uv.int_value & 0xFF);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value >= -32768 && constant->uv.int_value <= 32767) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SIPUSH);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (constant->uv.int_value >> 8) & 0xFF);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, constant->uv.int_value & 0xFF);
                    bytecode_set = 1;
                  }
                }
                else if (constant->code == SPVM_CONSTANT_C_CODE_LONG) {
                  if (constant->uv.long_value == -1) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LCONST_M1);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value == 0) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LCONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value == 1) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LCONST_1);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value == 2) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LCONST_2);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value == 3) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LCONST_3);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value == 4) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LCONST_4);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value == 5) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LCONST_5);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value >= -128 && constant->uv.long_value <= 127) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_BLPUSH);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, constant->uv.long_value & 0xFF);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value >= -32768 && constant->uv.long_value <= 32767) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_SLPUSH);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (constant->uv.long_value >> 8) & 0xFF);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, constant->uv.long_value & 0xFF);
                    bytecode_set = 1;
                  }
                }
                else if (constant->code == SPVM_CONSTANT_C_CODE_FLOAT) {
                  if (constant->uv.float_value == 0) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FCONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.float_value == 1) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FCONST_1);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.float_value == 2) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_FCONST_2);
                    bytecode_set = 1;
                  }
                }
                else if (constant->code == SPVM_CONSTANT_C_CODE_DOUBLE) {
                  if (constant->uv.double_value == 0) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DCONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.double_value == 1) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_DCONST_1);
                    bytecode_set = 1;
                  }
                }
                else if (constant->code == SPVM_CONSTANT_C_CODE_STRING) {
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_MALLOC_STRING);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (constant->constant_pool_address >> 24) & 0xFF);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (constant->constant_pool_address >> 16) & 0xFF);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (constant->constant_pool_address >> 8) & 0xFF);
                  SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, constant->constant_pool_address & 0xFF);
                  bytecode_set = 1;
                }
                
                if (!bytecode_set) {
                  if (constant->constant_pool_address <= 0xFF) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LDC);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, constant->constant_pool_address & 0xFF);
                  }
                  else if (constant->constant_pool_address <= 0xFFFF) {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LDC_W);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (constant->constant_pool_address >> 8) & 0xFF);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, constant->constant_pool_address & 0xFF);
                  }
                  else {
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, SPVM_BYTECODE_C_CODE_LDC_WW);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (constant->constant_pool_address >> 24) & 0xFF);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (constant->constant_pool_address >> 16) & 0xFF);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, (constant->constant_pool_address >> 8) & 0xFF);
                    SPVM_BYTECODE_ARRAY_push(spvm, bytecode_array, constant->constant_pool_address & 0xFF);
                  }
                }
                
                break;
              }
            }
            
            // [END]Postorder traversal position
            
            if (op_cur == op_base) {
              finish = 1;
              break;
            }
            
            // Next sibling
            if (op_cur->moresib) {
              op_cur = SPVM_OP_sibling(spvm, op_cur);
              break;
            }
            // Next is parent
            else {
              op_cur = op_cur->sibparent;
            }
          }
          if (finish) {
            break;
          }
        }
      }
      sub->bytecode_length = bytecode_array->length - sub->bytecode_base;
      
      // Set bytecode base to sub
      SPVM_CONSTANT_POOL_SUB* constant_pool_sub = (SPVM_CONSTANT_POOL_SUB*)&spvm->constant_pool->values[sub->constant_pool_address];
      constant_pool_sub->bytecode_base = sub->bytecode_base;
    }
  }
}