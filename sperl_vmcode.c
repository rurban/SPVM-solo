#include "sperl_vmcode.h"







SPerl_uchar* const SPerl_VMCODE_C_CODE_NAMES[] = {
  "POP",
  "POP2",
  "IADD",
  "LADD",
  "FADD",
  "DADD",
  "ISUBTRACT",
  "LSUBTRACT",
  "FSUBTRACT",
  "DSUBTRACT",
  "IMULTIPLY",
  "LMULTIPLY",
  "FMULTIPLY",
  "DMULTIPLY",
  "IDIVIDE",
  "LDIVIDE",
  "FDIVIDE",
  "DDIVIDE",
  "D2F",
  "D2I",
  "D2L",
  "F2D",
  "F2I",
  "F2L",
  "I2B",
  "I2C",
  "I2D",
  "I2F",
  "I2L",
  "I2S",
  "L2D",
  "L2F",
  "L2I",
  "DCONST_0",
  "DCONST_1",
  "FCONST_0",
  "FCONST_1",
  "FCONST_2",
  "ICONST_M1",
  "ICONST_0",
  "ICONST_1",
  "ICONST_2",
  "ICONST_3",
  "ICONST_4",
  "ICONST_5",
  "LCONST_0",
  "LCONST_1",
  "LDC",
  "LDC_W",
  "LDC2_W",
  "INEGATE",
  "LNEGATE",
  "FNEGATE",
  "DNEGATE",
  "IREMAINDER",
  "LREMAINDER",
  "FREMAINDER",
  "DREMAINDER",
  "IRETURN",
  "LRETURN",
  "FRETURN",
  "DRETURN",
  "ARETURN",
  "RETURN",
  "AASTORE",
  "ACONST_NULL",
  "ALOAD",
  "ALOAD_0",
  "ALOAD_1",
  "ALOAD_2",
  "ALOAD_3",
  "ANEWARRAY",
  "ARRAYLENGTH",
  "ASTORE",
  "ASTORE_0",
  "ASTORE_1",
  "ASTORE_2",
  "ASTORE_3",
  "ATHROW",
  "BALOAD",
  "BASTORE",
  "BIPUSH",
  "CALOAD",
  "CASTORE",
  "CHECKCAST",
  "DALOAD",
  "DASTORE",
  "DCMPG",
  "DCMPL",
  "DLOAD",
  "DLOAD_0",
  "DLOAD_1",
  "DLOAD_2",
  "DLOAD_3",
  "DSTORE",
  "DSTORE_0",
  "DSTORE_1",
  "DSTORE_2",
  "DSTORE_3",
  "DUP",
  "DUP_X1",
  "DUP_X2",
  "DUP2",
  "DUP2_X1",
  "DUP2_X2",
  "FALOAD",
  "FASTORE",
  "FCMPG",
  "FCMPL",
  "FLOAD",
  "FLOAD_0",
  "FLOAD_1",
  "FLOAD_2",
  "FLOAD_3",
  "FSTORE",
  "FSTORE_0",
  "FSTORE_1",
  "FSTORE_2",
  "FSTORE_3",
  "GETFIELD",
  "GETSTATIC",
  "GOTO",
  "GOTO_W",
  "IALOAD",
  "IAND",
  "IASTORE",
  "IF_ACMPEQ",
  "IF_ACMPNE",
  "IF_ICMPEQ",
  "IF_ICMPNE",
  "IF_ICMPLT",
  "IF_ICMPGE",
  "IF_ICMPGT",
  "IF_ICMPLE",
  "IFEQ",
  "IFNE",
  "IFLT",
  "IFGE",
  "IFGT",
  "IFLE",
  "IFNONNULL",
  "IFNULL",
  "IINC",
  "ILOAD",
  "ILOAD_0",
  "ILOAD_1",
  "ILOAD_2",
  "ILOAD_3",
  "INSTANCEOF",
  "INVOKEDYNAMIC",
  "INVOKEINTERFACE",
  "INVOKESPECIAL",
  "INVOKESTATIC",
  "INVOKEVIRTUAL",
  "IOR",
  "ISHL",
  "ISHR",
  "ISTORE",
  "ISTORE_0",
  "ISTORE_1",
  "ISTORE_2",
  "ISTORE_3",
  "IUSHR",
  "IXOR",
  "JSR",
  "JSR_W",
  "LALOAD",
  "LAND",
  "LASTORE",
  "LCMP",
  "LLOAD",
  "LLOAD_0",
  "LLOAD_1",
  "LLOAD_2",
  "LLOAD_3",
  "LOOKUPSWITCH",
  "LOR",
  "LSHL",
  "LSHR",
  "LSTORE",
  "LSTORE_0",
  "LSTORE_1",
  "LSTORE_2",
  "LSTORE_3",
  "LUSHR",
  "LXOR",
  "MONITORENTER",
  "MONITOREXIT",
  "MULTIANEWARRAY",
  "NEW",
  "NEWARRAY",
  "NOP",
  "PUTFIELD",
  "PUTSTATIC",
  "RET",
  "SALOAD",
  "SASTORE",
  "SIPUSH",
  "SWAP",
  "TABLESWITCH",
  "WIDE"
};
