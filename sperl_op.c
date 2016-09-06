#include "sperl.h"

#include "sperl_op.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

SPerl_OP* SPerl_op_sibling_splice(SPerl_OP* parent, SPerl_OP* start, int del_count, SPerl_OP* insert) {
  SPerl_OP *first;
  SPerl_OP *rest;
  SPerl_OP *last_del = NULL;
  SPerl_OP *last_ins = NULL;

  if (start)
    first = SPerl_OpSIBLING(start);
  else if (!parent)
    goto no_parent;
  else
    first = parent->op_first;

  if (del_count && first) {
    last_del = first;
    while (--del_count && SPerl_OpHAS_SIBLING(last_del))
      last_del = SPerl_OpSIBLING(last_del);
    rest = SPerl_OpSIBLING(last_del);
    SPerl_OpLASTSIB_set(last_del, NULL);
  }
  else
    rest = first;

  if (insert) {
    last_ins = insert;
    while (SPerl_OpHAS_SIBLING(last_ins))
      last_ins = SPerl_OpSIBLING(last_ins);
    SPerl_OpMAYBESIB_set(last_ins, rest, NULL);
  }
  else
    insert = rest;

  if (start) {
    SPerl_OpMAYBESIB_set(start, insert, NULL);
  }
  else {
    if (!parent)
      goto no_parent;
    parent->op_first = insert;
  }

  if (!rest) {
    /* update op_last etc */
    SPerl_OP *lastop;

    if (!parent)
      goto no_parent;

    lastop = last_ins ? last_ins : start ? start : NULL;
    parent->op_last = lastop;

    if (lastop)
      SPerl_OpLASTSIB_set(lastop, parent);
  }
  return last_del ? first : NULL;

  no_parent:
    fprintf(stderr, "panic: op_sibling_splice(): NULL parent");
    exit(1);
}

SPerl_OP* SPerl_op_append_elem(SPerl_OP *first, SPerl_OP *last)
{
  if (!first)
    return last;

  if (!last)
    return first;
  
  if (first->op_type != SPerl_OP_LIST) {
    return SPerl_newOP(SPerl_OP_LIST, 0, first, last);
  }
  
  SPerl_op_sibling_splice(first, first->op_last, 0, last);
  return first;
}

SPerl_OP* SPerl_newOP(I32 type, I32 flags, SPerl_OP* first, SPerl_OP* last) {
        
  SPerl_OP *op;

  op = (SPerl_OP*)malloc(sizeof(SPerl_OP) * 1);
  op->op_type = type;
  op->op_first = first;
  op->op_flags = (U8)flags;
  op->op_private = (U8)(1 | (flags >> 8));
  
  if (last) {
    if (!first) {
      first = (SPerl_OP*)malloc(sizeof(SPerl_OP) * 1 );
      first->op_type = SPerl_OP_NULL;
    }
    
    op->op_last = last;
    SPerl_OpMORESIB_set(first, last);
    if (op->op_last)
      SPerl_OpLASTSIB_set(op->op_last, (SPerl_OP*)op);
  }
  else if (first) {
    SPerl_OpLASTSIB_set(op->op_first, (SPerl_OP*)op);
  }

  return (SPerl_OP *)op;
}
