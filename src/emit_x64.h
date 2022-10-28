#pragma once

#include "oasis.h"

#define EMIT_NULL() \
	EMIT( \
		"\tcallq\t_new_null\n" \
		"\tpushq\t%%rax\n" \
	)

#define EMIT_INT(val) \
	EMIT( \
		"\tmovl\t$%d, %%edi\n" \
		"\tcallq\t_new_int\n" \
		"\tpushq\t%%rax\n", \
		val \
	)

#define EMIT_PRINT() \
	EMIT( \
		"\tpopq\t%%rdi\n" \
		"\tcallq\t_obj_print\n" \
	)

#define EMIT_ADD() \
	EMIT( \
		"\tpopq\t%%rsi\n" \
		"\tpopq\t%%rdi\n" \
		"\tcallq\t_obj_add\n" \
		"\tpushq\t%%rax\n" \
	)

#define EMIT_SUB() \
	EMIT( \
		"\tpopq\t%%rsi\n" \
		"\tpopq\t%%rdi\n" \
		"\tcallq\t_obj_sub\n" \
		"\tpushq\t%%rax\n" \
	)

#define EMIT_MUL() \
	EMIT( \
		"\tpopq\t%%rsi\n" \
		"\tpopq\t%%rdi\n" \
		"\tcallq\t_obj_mul\n" \
		"\tpushq\t%%rax\n" \
	)

#define EMIT_DIV() \
	EMIT( \
		"\tpopq\t%%rsi\n" \
		"\tpopq\t%%rdi\n" \
		"\tcallq\t_obj_div\n" \
		"\tpushq\t%%rax\n" \
	)

#define EMIT_STORE_ARG(id) EMIT("\tpopq\t%%%s\n", s_arg_regs[id])

#define EMIT_LOAD_ARG(id) \
	EMIT( \
		"\tmovq\t%%%s, -%d(%%rbp)\n", \
		s_arg_regs[id], \
		STACK_OFFSET(id) \
	)

#define EMIT_CALL(name) \
	EMIT( \
		"\tcallq\t_user_%s\n" \
		"\tpushq\t%%rax\n", \
		name \
	)

#define EMIT_RETURN() \
	EMIT( \
		"\tpopq\t%%rax\n" \
		"\tmovq\t%%rbp, %%rsp\n" \
		"\tpopq\t%%rbp\n" \
		"\tretq\n" \
	)

#define EMIT_DEFINE_GLOBAL(name) \
	EMIT( \
		".data\n" \
		"_user_%s: .quad 0\n", \
		name \
	)

#define EMIT_STORE_GLOBAL(name) \
	EMIT( \
		"\tpopq\t%%rax\n" \
		"\tmovq\t%%rax, _%s(%%rip)\n", \
		name \
	)

#define EMIT_LOAD_GLOBAL(name) \
	EMIT( \
		"\tmovq\t_%s(%%rip), %%rax\n" \
		"\tpushq\t%%rax\n", \
		name \
	)

#define STACK_OFFSET(id) (id * 8 + 16)

#define EMIT_DEFINE_LOCAL(id) \
	if (STACK_OFFSET(id) > s_stack_space) { \
		EMIT("\tsubq\t$16, %%rsp\n"); \
		s_stack_space += (STACK_OFFSET(id) - s_stack_space) / 16; \
	}

#define EMIT_STORE_LOCAL(id) \
	EMIT( \
		"\tpopq\t%%rax\n" \
		"\tmovq\t%%rax, -%d(%%rbp)\n", \
		STACK_OFFSET(id) \
	)

#define EMIT_LOAD_LOCAL(id) \
	EMIT( \
		"\tmovq\t-%d(%%rbp), %%rax\n" \
		"\tpushq\t%%rax\n", \
		STACK_OFFSET(id) \
	)

#define EMIT_DEFINE_FUNC(name) \
	EMIT( \
		".text\n" \
		".global _user_%s\n" \
		"_user_%s:\n" \
		"\tpushq\t%%rbp\n" \
		"\tmovq\t%%rsp, %%rbp\n", \
		name, \
		name \
	), s_stack_space = 0

static int s_stack_space = 0;
static char* s_arg_regs[] = {"rdi", "rsi"};
