#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "token.h"
#define div divide

void buildLabelMap(op *op_stack[OPSTACK_SIZE], label_ptr labels[LABELS_SIZE]) {
  int lp = 0;
  for (int i = 0; i < OPSTACK_SIZE; i++) {
    if (op_stack[i] == NULL) {
      break;
    }

    if (op_stack[i]->code == lbl) {
      labels[lp].op = op_stack[i];
      labels[lp].instruction = i;
      lp++;
    }
  }
}

#define op_is_reg(c) (op_stack[stackptr]->children[(c)].type == reg)
#define op_is_num(c) (op_stack[stackptr]->children[(c)].type == num)
#define op_is_str(c) (op_stack[stackptr]->children[(c)].type == str)

#define op_reg(c) op_stack[stackptr]->children[(c)].v.reg
#define op_num(c) op_stack[stackptr]->children[(c)].v.num
#define op_str(c) op_stack[stackptr]->children[(c)].v.str

void printRegs(int regs[8]) {
  printf("Regs: ");
  for (int i = 0; i < 8; i++) {
    printf("[%d]: %d, ", i, regs[i]);
  }
  printf("\n");
}

#define require_reg(r, m) { \
  if (!op_is_reg((r))) {    \
    printf(m "\n");         \
    exit(0);                \
  }                         \
}

#define get_label_instruction() _get_label_instruction(labels, op_stack[stackptr])
int _get_label_instruction(label_ptr labels[LABELS_SIZE], op *o) {
  for (int i = 0; i < LABELS_SIZE && labels[i].op != NULL; i++) {
    if (strcmp(labels[i].op->children[0].v.str, o->children[0].v.str) == 0) {
      return labels[i].instruction;
    }
  }

  return -1;
}

char *do_interpret(op *op_stack[OPSTACK_SIZE], label_ptr labels[LABELS_SIZE]) {
  int regs[26] = {0};
  int cmp_val;

  int call_stack[256] = {0};
  int call_stackptr = 0;

  char outbuf[1024] = {0};

  int stackptr = 0;
  while (stackptr < OPSTACK_SIZE) {
    if (stackptr == -1) {
      printf("Unknown jump\n");
      exit(1);
    }
    if (op_stack[stackptr] == NULL) {
      return (char*)-1;
    }
    switch (op_stack[stackptr]->code) {
      case mov:
        require_reg(0, "Unable to store value in a non register")
        regs[op_reg(0)] = op_is_reg(1) ? regs[op_reg(1)] : op_num(1);
        break;
      case inc:
        require_reg(0, "Unable to increment value in a non register")
        regs[op_reg(0)]++;
        break;
      case dec:
        require_reg(0, "Unable to decrement value in a non register")
        regs[op_reg(0)]--;
        break;
      case add:
        require_reg(0, "Unable to add value in a non register")
        regs[op_reg(0)] += op_is_reg(1) ? regs[op_reg(1)] : op_num(1);
        break;
      case sub:
        require_reg(0, "Unable to subtract value in a non register")
        regs[op_reg(0)] -= op_is_reg(1) ? regs[op_reg(1)] : op_num(1);
        break;
      case mul:
        require_reg(0, "Unable to multiply value in a non register")
        regs[op_reg(0)] *= op_is_reg(1) ? regs[op_reg(1)] : op_num(1);
        break;
      case div:
        require_reg(0, "Unable to divide value in a non register")
        regs[op_reg(0)] /= op_is_reg(1) ? regs[op_reg(1)] : op_num(1);
        break;
      case cmp:
        cmp_val = (op_is_reg(0) ? regs[op_reg(0)] : op_num(0)) - (op_is_reg(1) ? regs[op_reg(1)] : op_num(1));
        break;
      case jmp:
        stackptr = get_label_instruction();
        continue;
      case jne:
        if (cmp_val != 0) {
          stackptr = get_label_instruction();
          continue;
        }
        break;
      case je:
        if (cmp_val == 0) {
          stackptr = get_label_instruction();
          continue;
        }
        break;
      case jge:
        if (cmp_val >= 0) {
          stackptr = get_label_instruction();
          continue;
        }
        break;
      case jg:
        if (cmp_val > 0) {
          stackptr = get_label_instruction();
          continue;
        }
        break;
      case jle:
        if (cmp_val <= 0) {
          stackptr = get_label_instruction();
          continue;
        }
        break;
      case jl:
        if (cmp_val < 0) {
          stackptr = get_label_instruction();
          continue;
        }
        break;
      case call:
        call_stack[call_stackptr++] = stackptr + 1;
        stackptr = get_label_instruction();
        continue;
      case ret:
        if (call_stackptr == 0) {
          printf("Can not return from non subroutine\n");
        }
        stackptr = call_stack[--call_stackptr];
        continue;
      case msg:
        for (int i = 0; i < op_stack[stackptr]->num; i++) {
          if (op_is_reg(i))
            sprintf(outbuf, "%s%d", outbuf, regs[op_reg(i)]);
          else if (op_is_num(i))
            sprintf(outbuf, "%s%d", outbuf, op_num(i));
          else if (op_is_str(i))
            sprintf(outbuf, "%s%s", outbuf, op_str(i));
        }
        break;
      case end:
        return strdup(outbuf);

      default:
        break;
    }
    printRegs(regs);
    stackptr++;
  }

  return (char*)-1;
}

char* assembler_interpreter (const char* program) {
  token tokens[2048] = {0};
  tokenize(program, tokens);

  op *op_stack[OPSTACK_SIZE] = {0};
  buildOpStack(tokens, op_stack);

  label_ptr labels[LABELS_SIZE] = {0};
  buildLabelMap(op_stack, labels);

  return do_interpret(op_stack, labels);
}

void testProgram1() {
  char *program =
  "call  func1\n"
  "call  print\n"
  "end\n"
  "\n"
  "func1:\n"
  "    call  func2\n"
  "    ret\n"
  "\n"
  "func2:\n"
  "     ret\n"
  "\n"
  "print:\n"
  "    msg 'This program should return -1'\n";
  char *programother =
    "mov   a, 11           ; value1\n"
    "mov   b, 3            ; value2\n"
    "call  mod_func\n"
    "msg   'mod(', a, ', ', b, ') = ', d        ; output\n"
    "end\n"
    "\n"
    "; Mod function\n"
    "mod_func:\n"
    "    mov   c, a        ; temp1\n"
    "    div   c, b\n"
    "    mul   c, b\n"
    "    mov   d, a        ; temp2\n"
    "    sub   d, c\n"
    "    ret\n";
  char *gcd =
  "mov   a, 81         ; value1\n"
  "mov   b, 153        ; value2\n"
  "call  init\n"
  "call  proc_gcd\n"
  "call  print\n"
  "end\n"
  "\n"
  "proc_gcd:\n"
  "    cmp   c, d\n"
  "    jne   loop\n"
  "    ret\n"
  "\n"
  "loop:\n"
  "    cmp   c, d\n"
  "    jg    a_bigger\n"
  "    jmp   b_bigger\n"
  "\n"
  "a_bigger:\n"
  "    sub   c, d\n"
  "    jmp   proc_gcd\n"
  "\n"
  "b_bigger:\n"
  "    sub   d, c\n"
  "    jmp   proc_gcd\n"
  "\n"
  "init:\n"
  "    cmp   a, 0\n"
  "    jl    a_abs\n"
  "    cmp   b, 0\n"
  "    jl    b_abs\n"
  "    mov   c, a            ; temp1\n"
  "    mov   d, b            ; temp2\n"
  "    ret\n"
  "\n"
  "a_abs:\n"
  "    mul   a, -1\n"
  "    jmp   init\n"
  "\n"
  "b_abs:\n"
  "    mul   b, -1\n"
  "    jmp   init\n"
  "\n"
  "print:\n"
  "    msg   'gcd(', a, ', ', b, ') = ', c\n"
  "    ret\n";

  char *fib =
  "mov   a, 8            ; value\n"
  "mov   b, 0            ; next\n"
  "mov   c, 0            ; counter\n"
  "mov   d, 0            ; first\n"
  "mov   e, 1            ; second\n"
  "call  proc_fib\n"
  "call  print\n"
  "end\n"
  "\n"
  "proc_fib:\n"
  "    cmp   c, 2\n"
  "    jl    func_0\n"
  "    mov   b, d\n"
  "    add   b, e\n"
  "    mov   d, e\n"
  "    mov   e, b\n"
  "    inc   c\n"
  "    cmp   c, a\n"
  "    jle   proc_fib\n"
  "    ret\n"
  "\n"
  "func_0:\n"
  "    mov   b, c\n"
  "    inc   c\n"
  "    jmp   proc_fib\n"
  "\n"
  "print:\n"
  "    msg   'Term ', a, ' of Fibonacci series is: ', b        ; output text\n"
  "    ret\n";

  char *err =
  "mov s, 9   ; instruction mov s, 9\n"
  "mov o, 11   ; instruction mov o, 11\n"
  "call func\n"
  "msg 'Random result: ', x\n"
  "end\n"
  "\n"
  "func:\n"
  "	cmp s, o\n"
  "	jg exit\n"
  "	mov x, s\n"
  "	div x, o\n"
  "	ret\n"
  "; Do nothing\n"
  "exit:\n"
  "	msg 'Do nothing'\n";
  char *ret = assembler_interpreter(err);
  if (ret == (char*)-1) {
    printf("Error Interpreting\n");
  } else {
    printf("Out: %s\n", ret);
    free(ret);
  }
}

int main() {
  testProgram1();
  return 0;
}
