#ifndef SPERL_BYTECODE_H
#define SPERL_BYTECODE_H

#include "sperl_base.h"



// SPVM bytecode is same as Java bytecode
enum {
  SPerl_BYTECODE_C_CODE_NOP,
  SPerl_BYTECODE_C_CODE_ACONST_NULL,
  SPerl_BYTECODE_C_CODE_ICONST_M1,
  SPerl_BYTECODE_C_CODE_ICONST_0,
  SPerl_BYTECODE_C_CODE_ICONST_1,
  SPerl_BYTECODE_C_CODE_ICONST_2,
  SPerl_BYTECODE_C_CODE_ICONST_3,
  SPerl_BYTECODE_C_CODE_ICONST_4,
  SPerl_BYTECODE_C_CODE_ICONST_5,
  SPerl_BYTECODE_C_CODE_LCONST_M1,
  SPerl_BYTECODE_C_CODE_LCONST_0,
  SPerl_BYTECODE_C_CODE_LCONST_1,
  SPerl_BYTECODE_C_CODE_LCONST_2,
  SPerl_BYTECODE_C_CODE_LCONST_3,
  SPerl_BYTECODE_C_CODE_LCONST_4,
  SPerl_BYTECODE_C_CODE_LCONST_5,
  SPerl_BYTECODE_C_CODE_FCONST_0,
  SPerl_BYTECODE_C_CODE_FCONST_1,
  SPerl_BYTECODE_C_CODE_FCONST_2,
  SPerl_BYTECODE_C_CODE_DCONST_0,
  SPerl_BYTECODE_C_CODE_DCONST_1,
  SPerl_BYTECODE_C_CODE_BIPUSH,
  SPerl_BYTECODE_C_CODE_SIPUSH,
  SPerl_BYTECODE_C_CODE_LDC,
  SPerl_BYTECODE_C_CODE_LDC_W,
  SPerl_BYTECODE_C_CODE_IALOAD,
  SPerl_BYTECODE_C_CODE_LALOAD,
  SPerl_BYTECODE_C_CODE_FALOAD,
  SPerl_BYTECODE_C_CODE_DALOAD,
  SPerl_BYTECODE_C_CODE_BALOAD,
  SPerl_BYTECODE_C_CODE_SALOAD,
  SPerl_BYTECODE_C_CODE_BASTORE,
  SPerl_BYTECODE_C_CODE_CASTORE,
  SPerl_BYTECODE_C_CODE_SASTORE,
  SPerl_BYTECODE_C_CODE_IASTORE,
  SPerl_BYTECODE_C_CODE_LASTORE,
  SPerl_BYTECODE_C_CODE_FASTORE,
  SPerl_BYTECODE_C_CODE_DASTORE,
  SPerl_BYTECODE_C_CODE_POP,
  SPerl_BYTECODE_C_CODE_IADD,
  SPerl_BYTECODE_C_CODE_LADD,
  SPerl_BYTECODE_C_CODE_FADD,
  SPerl_BYTECODE_C_CODE_DADD,
  SPerl_BYTECODE_C_CODE_ISUB,
  SPerl_BYTECODE_C_CODE_LSUB,
  SPerl_BYTECODE_C_CODE_FSUB,
  SPerl_BYTECODE_C_CODE_DSUB,
  SPerl_BYTECODE_C_CODE_IMUL,
  SPerl_BYTECODE_C_CODE_LMUL,
  SPerl_BYTECODE_C_CODE_FMUL,
  SPerl_BYTECODE_C_CODE_DMUL,
  SPerl_BYTECODE_C_CODE_IDIV,
  SPerl_BYTECODE_C_CODE_LDIV,
  SPerl_BYTECODE_C_CODE_FDIV,
  SPerl_BYTECODE_C_CODE_DDIV,
  SPerl_BYTECODE_C_CODE_IREM,
  SPerl_BYTECODE_C_CODE_LREM,
  SPerl_BYTECODE_C_CODE_FREM,
  SPerl_BYTECODE_C_CODE_DREM,
  SPerl_BYTECODE_C_CODE_INEG,
  SPerl_BYTECODE_C_CODE_LNEG,
  SPerl_BYTECODE_C_CODE_FNEG,
  SPerl_BYTECODE_C_CODE_DNEG,
  SPerl_BYTECODE_C_CODE_ISHL,
  SPerl_BYTECODE_C_CODE_LSHL,
  SPerl_BYTECODE_C_CODE_ISHR,
  SPerl_BYTECODE_C_CODE_LSHR,
  SPerl_BYTECODE_C_CODE_IUSHR,
  SPerl_BYTECODE_C_CODE_LUSHR,
  SPerl_BYTECODE_C_CODE_IAND,
  SPerl_BYTECODE_C_CODE_LAND,
  SPerl_BYTECODE_C_CODE_IOR,
  SPerl_BYTECODE_C_CODE_LOR,
  SPerl_BYTECODE_C_CODE_IXOR,
  SPerl_BYTECODE_C_CODE_LXOR,
  SPerl_BYTECODE_C_CODE_IINC,
  SPerl_BYTECODE_C_CODE_I2L,
  SPerl_BYTECODE_C_CODE_I2F,
  SPerl_BYTECODE_C_CODE_I2D,
  SPerl_BYTECODE_C_CODE_L2I,
  SPerl_BYTECODE_C_CODE_L2F,
  SPerl_BYTECODE_C_CODE_L2D,
  SPerl_BYTECODE_C_CODE_F2I,
  SPerl_BYTECODE_C_CODE_F2L,
  SPerl_BYTECODE_C_CODE_F2D,
  SPerl_BYTECODE_C_CODE_D2I,
  SPerl_BYTECODE_C_CODE_D2L,
  SPerl_BYTECODE_C_CODE_D2F,
  SPerl_BYTECODE_C_CODE_I2B,
  SPerl_BYTECODE_C_CODE_I2S,
  SPerl_BYTECODE_C_CODE_LCMP,
  SPerl_BYTECODE_C_CODE_FCMPL,
  SPerl_BYTECODE_C_CODE_FCMPG,
  SPerl_BYTECODE_C_CODE_DCMPL,
  SPerl_BYTECODE_C_CODE_DCMPG,
  SPerl_BYTECODE_C_CODE_IFEQ,
  SPerl_BYTECODE_C_CODE_IFNE,
  SPerl_BYTECODE_C_CODE_IFLT,
  SPerl_BYTECODE_C_CODE_IFGE,
  SPerl_BYTECODE_C_CODE_IFGT,
  SPerl_BYTECODE_C_CODE_IFLE,
  SPerl_BYTECODE_C_CODE_IF_ICMPEQ,
  SPerl_BYTECODE_C_CODE_IF_ICMPNE,
  SPerl_BYTECODE_C_CODE_IF_ICMPLT,
  SPerl_BYTECODE_C_CODE_IF_ICMPGE,
  SPerl_BYTECODE_C_CODE_IF_ICMPGT,
  SPerl_BYTECODE_C_CODE_IF_ICMPLE,
  SPerl_BYTECODE_C_CODE_IF_ACMPEQ,
  SPerl_BYTECODE_C_CODE_IF_ACMPNE,
  SPerl_BYTECODE_C_CODE_GOTO,
  SPerl_BYTECODE_C_CODE_TABLESWITCH,
  SPerl_BYTECODE_C_CODE_LOOKUPSWITCH,
  SPerl_BYTECODE_C_CODE_ARRAYLENGTH,
  SPerl_BYTECODE_C_CODE_ATHROW,
  SPerl_BYTECODE_C_CODE_WIDE,
  SPerl_BYTECODE_C_CODE_IFNULL,
  SPerl_BYTECODE_C_CODE_IFNONNULL,
  SPerl_BYTECODE_C_CODE_INVOKESTATIC_WW,
  SPerl_BYTECODE_C_CODE_LDC_WW,
  SPerl_BYTECODE_C_CODE_LDC2_WW,
  SPerl_BYTECODE_C_CODE_BGETFIELD,
  SPerl_BYTECODE_C_CODE_SGETFIELD,
  SPerl_BYTECODE_C_CODE_IGETFIELD,
  SPerl_BYTECODE_C_CODE_LGETFIELD,
  SPerl_BYTECODE_C_CODE_FGETFIELD,
  SPerl_BYTECODE_C_CODE_DGETFIELD,
  SPerl_BYTECODE_C_CODE_BPUTFIELD,
  SPerl_BYTECODE_C_CODE_SPUTFIELD,
  SPerl_BYTECODE_C_CODE_IPUTFIELD,
  SPerl_BYTECODE_C_CODE_LPUTFIELD,
  SPerl_BYTECODE_C_CODE_FPUTFIELD,
  SPerl_BYTECODE_C_CODE_DPUTFIELD,
  SPerl_BYTECODE_C_CODE_LINC,
  SPerl_BYTECODE_C_CODE_B2I,
  SPerl_BYTECODE_C_CODE_S2I,
  SPerl_BYTECODE_C_CODE_B2L,
  SPerl_BYTECODE_C_CODE_B2F,
  SPerl_BYTECODE_C_CODE_B2D,
  SPerl_BYTECODE_C_CODE_S2B,
  SPerl_BYTECODE_C_CODE_S2L,
  SPerl_BYTECODE_C_CODE_S2F,
  SPerl_BYTECODE_C_CODE_S2D,
  SPerl_BYTECODE_C_CODE_L2B,
  SPerl_BYTECODE_C_CODE_L2S,
  SPerl_BYTECODE_C_CODE_F2B,
  SPerl_BYTECODE_C_CODE_F2S,
  SPerl_BYTECODE_C_CODE_D2B,
  SPerl_BYTECODE_C_CODE_D2S,
  SPerl_BYTECODE_C_CODE_B2S,
  SPerl_BYTECODE_C_CODE_MALLOC,
  SPerl_BYTECODE_C_CODE_MALLOCARRAY,
  SPerl_BYTECODE_C_CODE_RETURN_VOID,
  SPerl_BYTECODE_C_CODE_RETURN_VALUE,
  SPerl_BYTECODE_C_CODE_LOAD,
  SPerl_BYTECODE_C_CODE_LOAD_0,
  SPerl_BYTECODE_C_CODE_LOAD_1,
  SPerl_BYTECODE_C_CODE_LOAD_2,
  SPerl_BYTECODE_C_CODE_LOAD_3,
  SPerl_BYTECODE_C_CODE_STORE,
  SPerl_BYTECODE_C_CODE_STORE_0,
  SPerl_BYTECODE_C_CODE_STORE_1,
  SPerl_BYTECODE_C_CODE_STORE_2,
  SPerl_BYTECODE_C_CODE_STORE_3,
};

extern const char* const SPerl_BYTECODE_C_CODE_NAMES[];

#endif
