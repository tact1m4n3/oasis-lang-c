#include "oasis.h"

#define EMIT(fmt, ...) fprintf(s_file, fmt, ##__VA_ARGS__)

#ifdef OASIS_X64
#include "emit_x64.h"
#else
#error unknown architecture
#endif

static FILE* s_file;

static void gen_ident(struct Node* node) {
	struct Symbol* sym = node->sym;
	if (sym->global)
		EMIT_LOAD_GLOBAL(sym->name);
	else
		EMIT_LOAD_LOCAL(sym->id);
}

static void gen_int(struct Node* node) {
	EMIT_INT(node->intval);
}

static void gen_unaryop(struct Node* node) {
	gen_node(node->expr.right);

	switch (node->expr.op) {
	case T_SUB:
		break;
	}
}

static void gen_binop(struct Node* node) {
	if (node->expr.op == T_ASSIGN) {
		ASSERT(node->expr.left->kind == A_IDENT, "assignment only works on identifiers\n");

		struct Symbol* sym = node->expr.left->sym;
		gen_node(node->expr.right);
		if (sym->global)
			EMIT_STORE_GLOBAL(sym->name);
		else
			EMIT_STORE_LOCAL(sym->id);

		return;
	}

	gen_node(node->expr.left);
	gen_node(node->expr.right);

	switch (node->expr.op) {
	case T_ADD:
		EMIT_ADD();
		break;
	case T_SUB:
		EMIT_SUB();
		break;
	case T_MUL:
		EMIT_MUL();
		break;
	case T_DIV:
		EMIT_DIV();
		break;
	}
}

static void gen_call(struct Node* node) {
	for (int i = 0; i < node->func.args->len; i++) {
		struct Node* arg = node->func.args->data[i];
		gen_node(arg);
	}
	for (int i = node->func.args->len - 1; i >= 0; i--)
		EMIT_STORE_ARG(i);
	EMIT_CALL(node->func.name->ident);
}

static void gen_return(struct Node* node) {
	if (node->value)
		gen_node(node->value);
	else
		EMIT_NULL();
	EMIT_RETURN();
}

static void gen_let(struct Node* node) {
	struct Symbol* sym = node->sym;
	if (sym->global) {
		ASSERT(node->let.value, "global variables can't be initialized");
		EMIT_DEFINE_GLOBAL(sym->name);
	} else {
		EMIT_DEFINE_LOCAL(sym->id);
		if (node->let.value) {
			gen_node(node->let.value);
			EMIT_STORE_LOCAL(sym->id);
		}
	}
}

static void gen_block(struct Node* node) {
	for (int i = 0; i < node->list->len; i++) {
		struct Node* stmt = node->list->data[i];
		gen_node(stmt);
	}
}

static void gen_func(struct Node* node) {
	if (!node->func.body)
		return;

	EMIT_DEFINE_FUNC(node->sym->name);
	for (int i = 0; i < node->func.args->len; i++) {
		EMIT_DEFINE_LOCAL(i);
		EMIT_LOAD_ARG(i);
	}

	gen_node(node->func.body);

	EMIT_NULL();
	EMIT_RETURN();
}

static void gen_file(struct Node* node) {
	for (int i = 0; i < node->list->len; i++) {
		struct Node* stmt = node->list->data[i];
		gen_node(stmt);
	}
}

void gen_init(char* fname) {
	s_file = fopen(fname, "w+");
	ASSERT(s_file, "failed to open file %s\n", fname);
}

void gen_node(struct Node* node) {
	switch (node->kind) {
	case A_IDENT:
		gen_ident(node);
		break;
	case A_INT:
		gen_int(node);
		break;
	case A_UNARYOP:
		gen_unaryop(node);
		break;
	case A_BINOP:
		gen_binop(node);
		break;
	case A_CALL:
		gen_call(node);
		break;
	case A_RETURN:
		gen_return(node);
		break;
	case A_LET:
		gen_let(node);
		break;
	case A_BLOCK:
		gen_block(node);
		break;
	case A_FUNC:
		gen_func(node);
		break;
	case A_FILE:
		gen_file(node);
		break;
	}
}
