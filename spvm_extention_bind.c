#include <string.h>
#include "spvm.h"
#include "spvm_extention_bind.h"
#include "spvm_sub.h"
#include "spvm_extention.h"

SPVM_EXTENTION_BIND_bind_core_extentions(SPVM* spvm, SPVM_SUB* sub, const char* sub_abs_name) {

  if (strcmp(sub_abs_name, "std::println_byte") == 0) {
    sub->native_address = SPVM_EXTENTION_std__println_byte;
  }
  else if (strcmp(sub_abs_name, "std::println_short") == 0) {
    sub->native_address = SPVM_EXTENTION_std__println_short;
  }
  else if (strcmp(sub_abs_name, "std::println_int") == 0) {
    sub->native_address = SPVM_EXTENTION_std__println_int;
  }
  else if (strcmp(sub_abs_name, "std::println_long") == 0) {
    sub->native_address = SPVM_EXTENTION_std__println_long;
  }
  else if (strcmp(sub_abs_name, "std::println_float") == 0) {
    sub->native_address = SPVM_EXTENTION_std__println_float;
  }
  else if (strcmp(sub_abs_name, "std::println_double") == 0) {
    sub->native_address = SPVM_EXTENTION_std__println_double;
  }
  else if (strcmp(sub_abs_name, "std::println") == 0) {
    sub->native_address = SPVM_EXTENTION_std__println;
  }
  else if (strcmp(sub_abs_name, "std::sum_int") == 0) {
    sub->native_address = SPVM_EXTENTION_std__sum_int;
  }
  else if (strcmp(sub_abs_name, "std::test_call1") == 0) {
    sub->native_address = SPVM_EXTENTION_std__test_call1;
  }
  else if (strcmp(sub_abs_name, "std::test_call2") == 0) {
    sub->native_address = SPVM_EXTENTION_std__test_call2;
  }
}
