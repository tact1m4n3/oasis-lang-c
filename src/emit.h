#pragma once

#include "oasis.h"

#define EMIT(fmt, ...) fprintf(s_file, fmt, ##__VA_ARGS__)

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
		"\tcallq\t_print\n" \
	)

#define EMIT_RETURN() \
	EMIT( \
		"\tmovq\t%%rbp, %%rsp\n" \
		"\tpopq\t%%rbp\n" \
		"\tretq\n" \
	)

#define EMIT_DEFINE_GLOBAL(name) EMIT("%s: .long 0\n", name)

#define EMIT_STORE_GLOBAL(name) \
	EMIT( \
		"\tpopq\t%%rax\n" \
		"\tmovq\t%%rax, %s(%%rip)\n", \
		name \
	)

#define EMIT_LOAD_GLOBAL(name) \
	EMIT( \
		"\tmovq\t%s(%%rip), %%rax\n" \
		"\tpushq\t%%rax\n", \
		name \
	)

#define EMIT_DEFINE_LOCAL() EMIT("\tsubq\t$8, %%rsp\n")

#define EMIT_STORE_LOCAL(off) \
	EMIT( \
		"\tpopq\t%%rax\n" \
		"\tmovq\t%%rax, -%d(%%rbp)\n", \
		off \
	)

#define EMIT_LOAD_LOCAL(off) \
	EMIT( \
		"\tmovq\t-%d(%%rbp), %%rax\n" \
		"\tpushq\t%%rax\n", \
		off \
	)

#define EMIT_FUNC_BEGIN(name) \
	EMIT( \
		".global _%s\n" \
		"_%s:\n" \
		"\tpushq\t%%rbp\n" \
		"\tmovq\t%%rsp, %%rbp\n", \
		name, \
		name \
	)

#define EMIT_FUNC_END() \
	EMIT( \
		"\tmovq\t%%rbp, %%rsp\n" \
		"\tpopq\t%%rbp\n" \
		"\tmovq\t$0, %%rax\n" \
		"\tretq\n" \
	)

#define EMIT_FILE_BEGIN() \
	EMIT( \
		".extern\t_new_null\n" \
		".extern\t_new_int\n" \
		".extern\t_print\n" \
	)
