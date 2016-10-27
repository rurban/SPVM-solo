#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_parser.h"
#include "sperl_method.h"
#include "sperl_class.h"
#include "sperl_const_value.h"
#include "sperl_my_var.h"
#include "sperl_field.h"
#include "sperl_memory_pool.h"
#include "sperl_op.h"
#include "sperl_var.h"
#include "sperl_word.h"
#include "sperl_enum_value.h"
#include "sperl_descripter.h"
#include "sperl_type.h"
#include "sperl_type_core.h"
#include "sperl_type_enum.h"

static SPerl_char* _type_to_str(SPerl_PARSER* parser, SPerl_TYPE* type) {
  if (type->code == SPerl_TYPE_C_CODE_CORE) {
    return SPerl_TYPE_CORE_C_CODE_NAMES[type->uv.type_core->code];
  }
  else if (type->code == SPerl_TYPE_C_CODE_UNKNOWN) {
    return type->uv.name_word->value;
  }
}

SPerl_PARSER* SPerl_PARSER_new() {
  SPerl_PARSER* parser = (SPerl_PARSER*)calloc(1, sizeof(SPerl_PARSER));
  
  // Manipulate memory
  parser->array_ptrs = SPerl_ARRAY_new(0);
  parser->hash_ptrs = SPerl_ARRAY_new(0);
  parser->long_str_ptrs = SPerl_ARRAY_new(0);
  parser->memory_pool = SPerl_MEMORY_POOL_new(0);
  
  parser->types = SPerl_PARSER_new_array(parser, 0);
  parser->type_symtable = SPerl_PARSER_new_hash(parser, 0);
  parser->const_values = SPerl_PARSER_new_array(parser, 0);
  parser->use_stack = SPerl_PARSER_new_array(parser, 0);
  parser->typemap = SPerl_PARSER_new_hash(parser, 0);
  
  parser->const_pool_capacity = 1024;
  parser->const_pool = (SPerl_int*)calloc(parser->const_pool_capacity, sizeof(SPerl_int));
  
  parser->next_var_id = 1;
  
  parser->current_methods = SPerl_PARSER_new_array(parser, 0);
  parser->include_pathes = SPerl_PARSER_new_array(parser, 0);
  
  parser->bufptr = "";
  
  // Add boolean type
  {
    SPerl_TYPE_CORE* type_core = SPerl_TYPE_CORE_new(parser);
    type_core->code = SPerl_TYPE_CORE_C_CODE_BOOLEAN;
    type_core->size = 1;
    SPerl_TYPE* type = SPerl_TYPE_new(parser);
    type->code = SPerl_TYPE_C_CODE_CORE;
    type->uv.type_core = type_core;
    SPerl_ARRAY_push(parser->types, type);
    SPerl_HASH_insert(parser->type_symtable, "boolean", strlen("boolean"), type);
  }
  
  // Add char type
  {
    SPerl_TYPE_CORE* type_core = SPerl_TYPE_CORE_new(parser);
    type_core->code = SPerl_TYPE_CORE_C_CODE_CHAR;
    type_core->size = 1;
    SPerl_TYPE* type = SPerl_TYPE_new(parser);
    type->code = SPerl_TYPE_C_CODE_CORE;
    type->uv.type_core = type_core;
    SPerl_ARRAY_push(parser->types, type);
    SPerl_HASH_insert(parser->type_symtable, "char", strlen("char"), type);
  }
  
  // Add byte type
  {
    SPerl_TYPE_CORE* type_core = SPerl_TYPE_CORE_new(parser);
    type_core->code = SPerl_TYPE_CORE_C_CODE_BYTE;
    type_core->size = 1;
    SPerl_TYPE* type = SPerl_TYPE_new(parser);
    type->code = SPerl_TYPE_C_CODE_CORE;
    type->uv.type_core = type_core;
    SPerl_ARRAY_push(parser->types, type);
    SPerl_HASH_insert(parser->type_symtable, "byte", strlen("byte"), type);
  }
  
  // Add short type
  {
    SPerl_TYPE_CORE* type_core = SPerl_TYPE_CORE_new(parser);
    type_core->code = SPerl_TYPE_CORE_C_CODE_SHORT;
    type_core->size = 1;
    SPerl_TYPE* type = SPerl_TYPE_new(parser);
    type->code = SPerl_TYPE_C_CODE_CORE;
    type->uv.type_core = type_core;
    SPerl_ARRAY_push(parser->types, type);
    SPerl_HASH_insert(parser->type_symtable, "short", strlen("short"), type);
  }
  
  // Add int type
  {
    SPerl_TYPE_CORE* type_core = SPerl_TYPE_CORE_new(parser);
    type_core->code = SPerl_TYPE_CORE_C_CODE_INT;
    type_core->size = 1;
    SPerl_TYPE* type = SPerl_TYPE_new(parser);
    type->code = SPerl_TYPE_C_CODE_CORE;
    type->uv.type_core = type_core;
    SPerl_ARRAY_push(parser->types, type);
    SPerl_HASH_insert(parser->type_symtable, "int", strlen("int"), type);
  }
  
  // Add long type
  {
    SPerl_TYPE_CORE* type_core = SPerl_TYPE_CORE_new(parser);
    type_core->code = SPerl_TYPE_CORE_C_CODE_LONG;
    type_core->size = 1;
    SPerl_TYPE* type = SPerl_TYPE_new(parser);
    type->code = SPerl_TYPE_C_CODE_CORE;
    type->uv.type_core = type_core;
    SPerl_ARRAY_push(parser->types, type);
    SPerl_HASH_insert(parser->type_symtable, "long", strlen("long"), type);
  }
  
  // Add float type
  {
    SPerl_TYPE_CORE* type_core = SPerl_TYPE_CORE_new(parser);
    type_core->code = SPerl_TYPE_CORE_C_CODE_FLOAT;
    type_core->size = 1;
    SPerl_TYPE* type = SPerl_TYPE_new(parser);
    type->code = SPerl_TYPE_C_CODE_CORE;
    type->uv.type_core = type_core;
    SPerl_ARRAY_push(parser->types, type);
    SPerl_HASH_insert(parser->type_symtable, "float", strlen("float"), type);
  }
  
  // Add double type
  {
    SPerl_TYPE_CORE* type_core = SPerl_TYPE_CORE_new(parser);
    type_core->code = SPerl_TYPE_CORE_C_CODE_DOUBLE;
    type_core->size = 1;
    SPerl_TYPE* type = SPerl_TYPE_new(parser);
    type->code = SPerl_TYPE_C_CODE_CORE;
    type->uv.type_core = type_core;
    SPerl_ARRAY_push(parser->types, type);
    SPerl_HASH_insert(parser->type_symtable, "dobule", strlen("dobule"), type);
  }
  return parser;
}

SPerl_ARRAY* SPerl_PARSER_new_array(SPerl_PARSER* parser, SPerl_int capacity) {
  SPerl_ARRAY* array = SPerl_ARRAY_new(capacity);
  
  SPerl_ARRAY_push(parser->array_ptrs, array);
  
  return array;
}

SPerl_HASH* SPerl_PARSER_new_hash(SPerl_PARSER* parser, SPerl_int capacity) {
  SPerl_HASH* hash = SPerl_HASH_new(capacity);
  
  SPerl_ARRAY_push(parser->hash_ptrs, hash);
  
  return hash;
}

SPerl_char* SPerl_PARSER_new_string(SPerl_PARSER* parser, SPerl_int length) {
  SPerl_char* str = (SPerl_char*)malloc(length + 1);
  
  if (length < 40) {
    str = SPerl_MEMORY_POOL_alloc(parser->memory_pool, 40);
  }
  else {
    str = (SPerl_char*)malloc(length + 1);
    SPerl_ARRAY_push(parser->long_str_ptrs, str);
  }
  
  return str;
}

void SPerl_PARSER_free(SPerl_PARSER* parser) {

  // Free all array pointers
  for (SPerl_int i = 0; i < parser->array_ptrs->length; i++) {
    SPerl_ARRAY* array = SPerl_ARRAY_fetch(parser->array_ptrs, i);
    SPerl_ARRAY_free(array);
  }
  SPerl_ARRAY_free(parser->array_ptrs);
  
  // Free all hash pointers
  for (SPerl_int i = 0; i < parser->hash_ptrs->length; i++) {
    SPerl_HASH* hash = SPerl_ARRAY_fetch(parser->hash_ptrs, i);
    SPerl_HASH_free(hash);
  }
  SPerl_ARRAY_free(parser->hash_ptrs);
  
  // Free all string pointers;
  for (SPerl_int i = 0; i < parser->long_str_ptrs->length; i++) {
    SPerl_char* str = SPerl_ARRAY_fetch(parser->long_str_ptrs, i);
    free(str);
  }
  SPerl_ARRAY_free(parser->long_str_ptrs);
  
  // Free memory pool */
  SPerl_MEMORY_POOL_free(parser->memory_pool);
  
  free(parser->const_pool);
  free(parser);
}

void SPerl_PARSER_dump_ast(SPerl_PARSER* parser, SPerl_OP* op, SPerl_int depth) {
      
  if (!op) {
    return;
  }
  
  for (SPerl_int i = 0; i < depth; i++) {
    printf(" ");
  }
  SPerl_int code = op->code;
  printf("%s", SPerl_OP_C_CODE_NAMES[code]);
  if (code == SPerl_OP_C_CODE_CONST_VALUE) {
    SPerl_CONST_VALUE* const_value = op->uv.pv;
    switch(const_value->code) {
      case SPerl_CONST_VALUE_C_CODE_BOOLEAN:
        printf(" boolean %d", const_value->uv.int_value);
        break;
      case SPerl_CONST_VALUE_C_CODE_CHAR:
        printf(" char '%c'", const_value->uv.int_value);
        break;
      case SPerl_CONST_VALUE_C_CODE_BYTE:
        printf(" byte %d", const_value->uv.int_value);
        break;
      case SPerl_CONST_VALUE_C_CODE_SHORT:
        printf(" short %d", const_value->uv.int_value);
        break;
      case SPerl_CONST_VALUE_C_CODE_INT:
        printf(" int %d", const_value->uv.int_value);
        break;
      case SPerl_CONST_VALUE_C_CODE_LONG:
        printf(" long %ld", const_value->uv.long_value);
        break;
      case SPerl_CONST_VALUE_C_CODE_FLOAT:
        printf(" float %f", const_value->uv.float_value);
        break;
      case SPerl_CONST_VALUE_C_CODE_DOUBLE:
        printf(" double %f", const_value->uv.double_value);
        break;
      case SPerl_CONST_VALUE_C_CODE_STRING:
        printf(" string \"%s\"", const_value->uv.string_value);
        break;
    }
  }
  else if (code == SPerl_OP_C_CODE_VAR) {
    SPerl_VAR* var = op->uv.pv;
    printf(" \"%s\"", var->name_word->value);
  }
  else if (code == SPerl_OP_C_CODE_WORD) {
    SPerl_WORD* word = op->uv.pv;
    printf(" \"%s\"", word->value);
  }
  printf("\n");

  if (op->first) {
    depth++;
    SPerl_PARSER_dump_ast(parser, op->first, depth);
    depth--;
  }
  
  if (op->moresib) {
    SPerl_PARSER_dump_ast(parser, SPerl_OP_sibling(parser, op), depth);
  }
}

void SPerl_PARSER_dump_parser(SPerl_PARSER* parser) {
  printf("\n[Abstract Syntax Tree]\n");
  SPerl_PARSER_dump_ast(parser, parser->op_grammer, 0);
  
  printf("\n[Type infomation]\n");
  SPerl_PARSER_dump_types(parser, parser->types);
  
  printf("\n[Constant information]\n");
  SPerl_PARSER_dump_const_values(parser, parser->const_values);
  
  printf("\n[Constant pool]\n");
  SPerl_PARSER_dump_const_pool(parser, parser->const_pool, parser->const_pool_pos);
}

void SPerl_PARSER_dump_const_values(SPerl_PARSER* parser, SPerl_ARRAY* const_values) {
  for (SPerl_int i = 0; i < const_values->length; i++) {
    SPerl_CONST_VALUE* const_value = (SPerl_CONST_VALUE*)SPerl_ARRAY_fetch(const_values, i);
    printf("    const_value[%" PRId32 "]\n", i);
    SPerl_PARSER_dump_const_value(parser, const_value);
  }
}

void SPerl_PARSER_dump_types(SPerl_PARSER* parser, SPerl_ARRAY* types) {
  for (SPerl_int i = 0; i < types->length; i++) {
    
    // Type
    SPerl_TYPE* type = SPerl_ARRAY_fetch(types, i);
    printf("type[%d]\n", i);
    
    // Core type
    if (type->code == SPerl_TYPE_C_CODE_CORE) {
      SPerl_TYPE_CORE* type_core = type->uv.type_core;
      printf("  code => \"core\"\n");
      printf("  name => \"%s\"\n", SPerl_TYPE_CORE_C_CODE_NAMES[type_core->code]);
    }
    // Class type
    else if (type->code == SPerl_TYPE_C_CODE_CLASS) {
      SPerl_CLASS* class = type->uv.class;
      
      printf("  code => \"class\"\n");
      printf("  name => \"%s\"\n", class->name->value);
      printf("  descripters => ");
      SPerl_ARRAY* descripters = class->descripters;
      if (descripters && descripters->length) {
        for (SPerl_int i = 0; i < descripters->length; i++) {
          SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, i);
          printf("%s ", SPerl_DESCRIPTER_CODE_NAMES[descripter->code]);
        }
      }
      else {
        printf("(None)");
      }
      printf("\n");
      printf("  op_block => %x\n", class->op_block);
      
      // Field information
      printf("  fields\n");
      SPerl_ARRAY* fields = class->fields;
      for (SPerl_int j = 0; j < fields->length; j++) {
        SPerl_FIELD* field = SPerl_ARRAY_fetch(fields, j);
        printf("    field[%" PRId32 "]\n", j);
        SPerl_PARSER_dump_field(parser, field);
      }
      
      // Method information
      printf("  methods\n");
      SPerl_ARRAY* methods = class->methods;
      for (SPerl_int j = 0; j < methods->length; j++) {
        SPerl_METHOD* method = SPerl_ARRAY_fetch(methods, j);
        printf("    method[%" PRId32 "]\n", j);
        SPerl_PARSER_dump_method(parser, method);
      }
    }
    // Enum type
    else if (type->code == SPerl_TYPE_C_CODE_ENUM) {
      SPerl_TYPE_ENUM* type_enum = type->uv.type_enum;
      
      // Enum value information
      printf("  code => \"enum\"\n");
      printf("  name => \"%s\"\n", type_enum->name_word->value);
      printf("  enum_values\n");
      SPerl_ARRAY* enum_values = type_enum->enum_values;
      for (SPerl_int j = 0; j < enum_values->length; j++) {
        SPerl_ENUM_VALUE* enum_value = SPerl_ARRAY_fetch(enum_values, j);
        printf("    enum_value[%" PRId32 "]\n", j);
        SPerl_PARSER_dump_enum_value(parser, enum_value);
      }
    }
    else if (type->code == SPerl_TYPE_C_CODE_TYPEDEF) {
      printf("  code => \"typedef\"\n");
    }
  }
}

void SPerl_PARSER_dump_const_pool(SPerl_PARSER* parser, SPerl_int* const_pool, SPerl_int const_pool_pos) {
  for (SPerl_int i = 0; i < const_pool_pos; i++) {
    printf("const_pool[%d] %d\n", i, const_pool[i]);
  }
}

void SPerl_PARSER_dump_const_value(SPerl_PARSER* parser, SPerl_CONST_VALUE* const_value) {
  switch(const_value->code) {
    case SPerl_CONST_VALUE_C_CODE_BOOLEAN:
      printf("      boolean %" PRId32 "\n", const_value->uv.int_value);
      break;
    case SPerl_CONST_VALUE_C_CODE_CHAR:
      printf("      char '%c'\n", const_value->uv.int_value);
      break;
    case SPerl_CONST_VALUE_C_CODE_BYTE:
      printf("      byte %" PRId32 "\n", const_value->uv.int_value);
      break;
    case SPerl_CONST_VALUE_C_CODE_SHORT:
      printf("      short %" PRId32 "\n", const_value->uv.int_value);
      break;
    case SPerl_CONST_VALUE_C_CODE_INT:
      printf("      int %" PRId32 "\n", const_value->uv.int_value);
      break;
    case SPerl_CONST_VALUE_C_CODE_LONG:
      printf("      long %" PRId64 "\n", const_value->uv.long_value);
      break;
    case SPerl_CONST_VALUE_C_CODE_FLOAT:
      printf("      float %f\n", const_value->uv.float_value);
      break;
    case SPerl_CONST_VALUE_C_CODE_DOUBLE:
      printf("      double %f\n", const_value->uv.double_value);
      break;
    case SPerl_CONST_VALUE_C_CODE_STRING:
      printf("      string \"%s\"\n", const_value->uv.string_value);
      break;
  }
  printf("      pool_pos => %d\n", const_value->pool_pos);
}

void SPerl_PARSER_dump_method(SPerl_PARSER* parser, SPerl_METHOD* method) {
  if (method) {
    if (method->anon) {
      printf("      name => (NONE)\n");
    }
    else {
      printf("      name => \"%s\"\n", method->name_word->value);
    }
    printf("      anon => %d\n", method->anon);
    if (method->return_type->code == SPerl_TYPE_C_CODE_UNKNOWN) {
      SPerl_WORD* return_type_name = method->return_type->uv.name_word;
      printf("      return_type => \"%s\"\n", return_type_name->value);
    }
    SPerl_int i;
    printf("      descripters => ");
    SPerl_ARRAY* descripters = method->descripters;
    if (descripters->length) {
      for (SPerl_int i = 0; i < descripters->length; i++) {
        SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, i);
        printf("%s ", SPerl_DESCRIPTER_CODE_NAMES[descripter->code]);
      }
    }
    else {
      printf("(None)");
    }
    printf("\n");
    printf("      argument_count => %" PRId32 "\n", method->argument_count);
    printf("      my_vars\n");
    SPerl_ARRAY* my_vars = method->my_vars;
    for (SPerl_int i = 0; i < my_vars->length; i++) {
      SPerl_MY_VAR* my_var
        = (SPerl_MY_VAR*)SPerl_ARRAY_fetch(method->my_vars, i);
      printf("        my_var[%d]\n", i);
      SPerl_PARSER_dump_my_var(parser, my_var);
    }
    printf("      op_block => %x\n", method->op_block);
  }
  else {
    printf("      None\n");
  }
}

void SPerl_PARSER_dump_field(SPerl_PARSER* parser, SPerl_FIELD* field) {
  if (field) {
    printf("      name => \"%s\"\n", field->name_word->value);
    SPerl_TYPE* type = field->type;
    if (type->code == SPerl_TYPE_C_CODE_CORE) {
      printf("      type => \"%s\"\n", _type_to_str(parser, type));
    }
    else if (type->code == SPerl_TYPE_C_CODE_UNKNOWN) {
      printf("      type => \"%s\"\n", _type_to_str(parser, type));
    }
    else if (type->code == SPerl_TYPE_C_CODE_SUB) {
      SPerl_TYPE_SUB* type_sub = type->uv.type_sub;
      printf("      type => sub (");
      for (SPerl_int i = 0; i < type_sub->argument_types->length; i++) {
        SPerl_TYPE* argument_type = SPerl_ARRAY_fetch(type_sub->argument_types, i);
        printf("%s " , _type_to_str(parser, argument_type));
      }
      printf(") ");
      SPerl_TYPE* return_type = type_sub->return_type;
      printf("%s\n", _type_to_str(parser, return_type));
    }
    printf("      descripters => ");
    SPerl_ARRAY* descripters = field->descripters;
    if (descripters->length) {
      for (SPerl_int i = 0; i < descripters->length; i++) {
        SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, i);
        printf("%s ", SPerl_DESCRIPTER_CODE_NAMES[descripter->code]);
      }
    }
    else {
      printf("(None)");
    }
    printf("\n");
  }
  else {
    printf("        None\n");
  }
}


void SPerl_PARSER_dump_enum_value(SPerl_PARSER* parser, SPerl_ENUM_VALUE* enum_value) {
  if (enum_value) {
    printf("      name => \"%s\"\n", enum_value->name_word->value);
    printf("      value => %d\n", enum_value->value->uv.int_value);
  }
  else {
    printf("      None\n");
  }
}

void SPerl_PARSER_dump_my_var(SPerl_PARSER* parser, SPerl_MY_VAR* my_var) {
  if (my_var) {
    printf("          name => \"%s\"\n", my_var->name_word->value);
    SPerl_TYPE* type = my_var->type;
    if (my_var->type->code == SPerl_TYPE_C_CODE_CORE) {
      printf("          type => \"%s\"\n", _type_to_str(parser, type));
    }
    else if (my_var->type->code == SPerl_TYPE_C_CODE_UNKNOWN) {
      printf("          type => \"%s\"\n", _type_to_str(parser, type));
    }
    else if (my_var->type->code == SPerl_TYPE_C_CODE_SUB) {
      SPerl_TYPE_SUB* type_sub = my_var->type->uv.type_sub;
      printf("      type => sub (");
      for (SPerl_int i = 0; i < type_sub->argument_types->length; i++) {
        SPerl_TYPE* argument_type = SPerl_ARRAY_fetch(type_sub->argument_types, i);
        printf("%s " , _type_to_str(parser, argument_type));
      }
      printf(") ");
      SPerl_TYPE* return_type = type_sub->return_type;
      printf("%s\n", _type_to_str(parser, return_type));
    }
    
    printf("          descripters => ");
    SPerl_ARRAY* descripters = my_var->descripters;
    if (descripters->length) {
      for (SPerl_int i = 0; i < descripters->length; i++) {
        SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, i);
        printf("%s ", SPerl_DESCRIPTER_CODE_NAMES[descripter->code]);
      }
    }
    else {
      printf("(None)");
    }
    printf("\n");
  }
  else {
    printf("          None\n");
  }
}
