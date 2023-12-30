#include "macros.h"

 void macro_rem() {
     printf(".macro _rem(%%ri, %%rj, %%rk)\n");
     printf("\tdiv %%rj, %%rk\n");
     printf("\tmfhi %%ri\n");
     printf(".end_macro\n");
 }

void macro_exit() {
    printf(".macro _exit()\n");
    printf("\taddiu $v0, $zero, 10\n");
    printf("\tsyscall\n");
    printf(".end_macro\n");
}

void macro_print_int() {
    printf(".macro _print_int(%%ri)\n");
    SAVE_STACK(4);
    printf("\tadd $a0, $zero, %%ri\n");
    printf("\taddi $v0, $zero, 1\n");
    printf("\tsyscall\n");
    LOAD_STACK(4);
    printf(".end_macro\n");
}

void macro_print_string() {
    printf(".macro _print_string(%%ri)\n");
    SAVE_STACK(4);
    printf("\tadd $a0, $zero, %%ri\n");
    printf("\taddi $v0, $zero, 4\n");
    printf("\tsyscall\n");
    LOAD_STACK(4);
    printf(".end_macro\n");
}

void macro_print_char() {
    printf(".macro _print_char(%%ri)\n");
    SAVE_STACK(4);
    printf("\tadd $a0, $zero, %%ri\n");
    printf("\taddi $v0, $zero, 11\n");
    LOAD_STACK(4);
    printf("\tsyscall\n");
    printf(".end_macro\n");
}

void macro_read_int() {
    printf(".macro _read_int(%%ri)\n");
    SAVE_STACK(4);
    printf("\taddi $v0, $zero, 5\n");
    printf("\tsyscall\n");
    printf("\tadd %%ri, $zero, $v0\n");
    LOAD_STACK(4);
    printf(".end_macro\n");
}

void macro_read_string() {
    printf(".macro _read_string(%%ri, %%rj)\n");
    SAVE_STACK(4);
    SAVE_STACK(5);
    printf("\tadd $a0, $zero, %%ri\n");
    printf("\tadd $a1, $zero, %%rj\n");
    printf("\taddi $v0, $zero, 8\n");
    printf("\tsyscall\n");
    LOAD_STACK(5);
    LOAD_STACK(4);
    printf(".end_macro\n");
}

void macro_read_char() {
    printf(".macro _read_char(%%ri)\n");
    SAVE_STACK(4);
    printf("\taddi $v0, $zero, 12\n");
    printf("\tsyscall\n");
    printf("\tadd %%ri, $zero, $v0\n");
    LOAD_STACK(4);
    printf(".end_macro\n");
}

void macro_sle() {
    printf(".macro _sle(%%ri, %%rj, %%rk)\n");
    printf("\tslt %%ri, %%rk, %%rj\n");
    printf("\tori $1, $zero, 1\n");
    printf("\tsubu %%ri, $1, %%ri\n");
    printf(".end_macro\n");
}

void macro_seq(){
    printf(".macro _seq(%%ri, %%rj, %%rk)\n");
    printf("\tsubu %%ri, %%rj, %%rk\n");
    printf("\tori $1, $zero, 1\n");
    printf("\tsltu %%ri, %%ri, $1\n");
    printf(".end_macro\n");
}