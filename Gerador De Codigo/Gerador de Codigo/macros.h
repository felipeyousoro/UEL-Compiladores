#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>

#define SAVE_STACK(reg) printf("\taddi $sp, $sp, -4\n"); \
                        printf("\tsw $%i , 0($sp)\n", reg);

#define LOAD_STACK(reg) printf("\tlw $%i, 0($sp)\n", reg); \
                        printf("\taddi $sp, $sp, 4\n");

#define JUMP_LINK(label) printf("\tjal %s\n", label);
#define RETURN_RA() printf("\tjr $ra\n");

#define SAVE_CONST(reg, const_value) printf("\taddi $%i, $zero, %i\n", reg, const_value);

#define MIPS_ADD(reg1, reg2, reg3) printf("\tadd $%i, $%i, $%i\n", reg1, reg2, reg3);
#define MIPS_ADDI(reg1, reg2, const_value) printf("\taddi $%i, $%i, %i\n", reg1, reg2, const_value);
#define MIPS_SUB(reg1, reg2, reg3) printf("\tsub $%i, $%i, $%i\n", reg1, reg2, reg3);
#define MIPS_MUL(reg1, reg2, reg3) printf("\tmul $%i, $%i, $%i\n", reg1, reg2, reg3);
#define MIPS_DIV(reg1, reg2, reg3) printf("\tdiv $%i, $%i, $%i\n", reg1, reg2, reg3);
#define MIPS_REM(reg1, reg2, reg3) printf("\tdiv $%i, $%i, $%i\n", reg1, reg2, reg3); \
                                   printf("\tmfhi $%i\n", reg1);
#define MIPS_SLL(reg1, reg2, reg3) printf("\tsllv $%i, $%i, $%i\n", reg1, reg2, reg3);
#define MIPS_SRL(reg1, reg2, reg3) printf("\tsrlv $%i, $%i, $%i\n", reg1, reg2, reg3);

#define MIPS_MOVE(reg1, reg2) printf("\tadd $%i, $%i, $zero\n", reg1, reg2);

#define MIPS_SLT(reg1, reg2, reg3) printf("\tslt $%i, $%i, $%i\n", reg1, reg2, reg3);
#define MIPS_SGT(reg1, reg2, reg3) printf("\tslt $%i, $%i, $%i\n", reg1, reg3, reg2);

#define MIPS_SLE(reg1, reg2, reg3) SAVE_STACK(2); \
                                    printf("\tslt $%i, $%i, $%i\n", reg1, reg3, reg2); \
                                    printf("\tori $2, $0, 1\n");                      \
                                    printf("\tsubu $%i, $2, $%i\n", reg1, reg1);      \
                                    LOAD_STACK(2);

#define MIPS_SGE(reg1, reg2, reg3) SAVE_STACK(2); \
                                    printf("\tslt $%i, $%i, $%i\n", reg1, reg2, reg3); \
                                    printf("\tori $2, $0, 1\n");                      \
                                    printf("\tsubu $%i, $2, $%i\n", reg1, reg1);      \
                                    LOAD_STACK(2);

#define MIPS_SEQ(reg1, reg2, reg3) SAVE_STACK(2); \
                                    printf("\tsubu $%i, $%i, $%i\n", reg1, reg2, reg3); \
                                    printf("\tori $2, $0, 1\n");                       \
                                    printf("\tsltu $%i, $%i, $2\n", reg1, reg1);       \
                                    LOAD_STACK(2);

#define MIPS_BEQ(reg1, reg2, label) printf("\tbeq $%i, $%i, %s\n", reg1, reg2, label);
#define MIPS_BNE(reg1, reg2, label) printf("\tbne $%i, $%i, %s\n", reg1, reg2, label);

#define MIPS_LW_LABEL(reg, offset, label) printf("\tlw $%i, %s($%i)\n", reg, label, offset);
#define MIPS_SW_LABEL(reg, offset, label) printf("\tsw $%i, %s($%i)\n", reg, label, offset);
#define MIPS_LW(reg, offset, base) printf("\tlw $%i, %i($%i)\n", reg, offset, base);
#define MIPS_SW(reg, offset, base) printf("\tsw $%i, %i($%i)\n", reg, offset, base);
#define MIPS_LA(reg, label) printf("\tla $%i, %s\n", reg, label);

#define MIPS_LOGICAL_AND(reg1, reg2, reg3) SAVE_STACK(2); \
                                            printf("\tsltu $%i, $zero, $%i\n", reg2, reg2); \
                                            printf("\tsltu $%i, $zero, $%i\n", reg3, reg3); \
                                            printf("\tand $%i, $%i, $%i\n", reg1, reg2, reg3); \
                                            LOAD_STACK(2);

#define MIPS_LOGICAL_OR(reg1, reg2, reg3) SAVE_STACK(2); \
                                             printf("\tsltu $%i, $zero, $%i\n", reg2, reg2); \
                                             printf("\tsltu $%i, $zero, $%i\n", reg3, reg3); \
                                             printf("\tor $%i, $%i, $%i\n", reg1, reg2, reg3); \
                                             LOAD_STACK(2);

#define MIPS_LOGICAL_NOT(reg1, reg2) SAVE_STACK(2); \
                                        printf("\tsltu $%i, $zero, $%i\n", reg2, reg2); \
                                        printf("\tori $2, $0, 1\n");                    \
                                        printf("\tsubu $%i, $2, $%i\n", reg1, reg2);   \
                                        LOAD_STACK(2);

#define MIPS_BITWISE_AND(reg1, reg2, reg3) printf("\tand $%i, $%i, $%i\n", reg1, reg2, reg3);
#define MIPS_BITWISE_OR(reg1, reg2, reg3) printf("\tor $%i, $%i, $%i\n", reg1, reg2, reg3);
#define MIPS_BITWISE_XOR(reg1, reg2, reg3) printf("\txor $%i, $%i, $%i\n", reg1, reg2, reg3);
#define MIPS_BITWISE_NOT(reg1, reg2) printf("\tnor $%i, $%i, $zero\n", reg1, reg2);

#define CALL_PRINT_INT(reg) SAVE_STACK(4); \
                                printf("\tadd $a0, $%i, $0\n", reg); \
                                printf("\taddi $v0, $zero, 1\n");    \
                                printf("\tsyscall\n");\
                                LOAD_STACK(4)

#define CALL_PRINT_STRING(reg) SAVE_STACK(4); \
                               printf("\tadd $a0, $%i, $0\n", reg); \
                               printf("\taddi $v0, $zero, 4\n");    \
                               printf("\tsyscall\n");\
                                LOAD_STACK(4)

#define CALL_PRINT_CHAR(reg) SAVE_STACK(4); \
                             printf("\tadd $a0, $%i, $0\n", reg); \
                             printf("\taddi $v0, $zero, 11\n");    \
                             printf("\tsyscall\n");\
                                LOAD_STACK(4)

#define CALL_READ_INT(reg) printf("\taddi $v0, $zero, 5\n");    \
                           printf("\tsyscall\n"); \
                            printf("\tadd $%i, $v0, $0\n", reg);

#define CALL_READ_STRING(reg, size) SAVE_STACK(4); \
                                    SAVE_STACK(5); \
                                    printf("\taddi $v0, $zero, 8\n");    \
                                    printf("\taddi $a0, $%i, 0\n", reg); \
                                    printf("\taddi $a1, $zero, %i\n", size); \
                                    printf("\tsyscall\n");               \
                                    LOAD_STACK(5); \
                                    LOAD_STACK(4);

#define CALL_READ_CHAR(reg) printf("\taddi $v0, $zero, 12\n");    \
                            printf("\tsyscall\n");                \
                            printf("\tadd $%i, $v0, $0\n", reg);

#define CALL_EXIT() printf("\taddi $v0, $zero, 10\n"); \
                    printf("\tsyscall\n");

#define CALL_EXIT_VALUE(reg) printf("\taddi $v0, $zero, 17\n"); \
                             printf("\tadd $a0, $%i, $0\n", reg); \
                             printf("\tsyscall\n");

void macro_print_int();

void macro_print_string();

void macro_print_char();

void macro_read_int();

void macro_read_string();

void macro_read_char();

void macro_rem();

void macro_exit();

void macro_sle();

void macro_sge();

void macro_seq();


#endif