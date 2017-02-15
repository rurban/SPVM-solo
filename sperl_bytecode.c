#include "sperl_bytecode.h"







const char* const SPerl_BYTECODE_C_CODE_NAMES[] = {
  "NOP",
  "ACONST_NULL",
  "ICONST_M1",
  "ICONST_0",
  "ICONST_1",
  "ICONST_2",
  "ICONST_3",
  "ICONST_4",
  "ICONST_5",
  "LCONST_M1",
  "LCONST_0",
  "LCONST_1",
  "LCONST_2",
  "LCONST_3",
  "LCONST_4",
  "LCONST_5",
  "FCONST_1",
  "FCONST_2",
  "DCONST_0",
  "DCONST_1",
  "BIPUSH",
  "SIPUSH",
  "BLPUSH",
  "SLPUSH",
  "LDC",
  "LDC_W",
  "IALOAD",
  "LALOAD",
  "FALOAD",
  "DALOAD",
  "BALOAD",
  "SALOAD",
  "BASTORE",
  "CASTORE",
  "SASTORE",
  "IASTORE",
  "LASTORE",
  "FASTORE",
  "DASTORE",
  "POP",
  "IADD",
  "LADD",
  "FADD",
  "DADD",
  "ISUB",
  "LSUB",
  "FSUB",
  "DSUB",
  "IMUL",
  "LMUL",
  "FMUL",
  "DMUL",
  "IDIV",
  "LDIV",
  "FDIV",
  "DDIV",
  "IREM",
  "LREM",
  "FREM",
  "DREM",
  "INEG",
  "LNEG",
  "FNEG",
  "DNEG",
  "ISHL",
  "LSHL",
  "ISHR",
  "LSHR",
  "IUSHR",
  "LUSHR",
  "IAND",
  "LAND",
  "IOR",
  "LOR",
  "IXOR",
  "LXOR",
  "IINC",
  "I2L",
  "I2F",
  "I2D",
  "L2I",
  "L2F",
  "L2D",
  "F2I",
  "F2L",
  "F2D",
  "D2I",
  "D2L",
  "D2F",
  "I2B",
  "I2S",
  "LCMP",
  "FCMPL",
  "FCMPG",
  "DCMPL",
  "DCMPG",
  "IFEQ",
  "IFNE",
  "IFLT",
  "IFGE",
  "IFGT",
  "IFLE",
  "IF_ICMPEQ",
  "IF_ICMPNE",
  "IF_ICMPLT",
  "IF_ICMPGE",
  "IF_ICMPGT",
  "IF_ICMPLE",
  "IF_ACMPEQ",
  "IF_ACMPNE",
  "GOTO",
  "TABLESWITCH",
  "LOOKUPSWITCH",
  "ARRAYLENGTH",
  "ATHROW",
  "WIDE",
  "IFNULL",
  "IFNONNULL",
  "INVOKESTATIC_WW",
  "LDC_WW",
  "LDC2_WW",
  "BGETFIELD",
  "SGETFIELD",
  "IGETFIELD",
  "LGETFIELD",
  "FGETFIELD",
  "DGETFIELD",
  "BPUTFIELD",
  "SPUTFIELD",
  "IPUTFIELD",
  "LPUTFIELD",
  "FPUTFIELD",
  "DPUTFIELD",
  "LINC",
  "B2I",
  "S2I",
  "B2L",
  "B2F",
  "B2D",
  "S2B",
  "S2L",
  "S2F",
  "S2D",
  "L2B",
  "L2S",
  "F2B",
  "F2S",
  "D2B",
  "D2S",
  "B2S",
  "MALLOC",
  "MALLOCARRAY",
  "RETURN_VOID",
  "RETURN_VALUE",
  "LOAD",
  "LOAD_0",
  "LOAD_1",
  "LOAD_2",
  "LOAD_3",
  "STORE",
  "STORE_0",
  "STORE_1",
  "STORE_2",
  "STORE_3",
};
