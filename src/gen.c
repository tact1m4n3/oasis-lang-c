#include "oasis.h"
#include "emit.h"

static FILE* s_file;

static void gen_ident(struct Node* node) {
	struct Symbol* sym = node->sym;
	if (sym->global)
		EMIT_LOAD_GLOBAL(sym->name);
	else
		EMIT_LOAD_LOCAL(sym->id * 8);
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
	if (node->expr.left->kind == A_IDENT && node->expr.op == T_ASSIGN) {
		struct Symbol* sym = node->expr.left->sym;
		gen_node(node->expr.right);
		if (sym->global)
			EMIT_STORE_GLOBAL(sym->name);
		else
			EMIT_STORE_LOCAL(sym->id * 8);
		return;
	}

	switch (node->expr.op) {
	case T_ADD:

		break;
	case T_SUB:

		break;
	case T_MUL:

		break;
	case T_DIV:

		break;
	}

}

static void gen_call(struct Node* node) {
	// EMIT("%s(", node->func.name);
	// for (int i = 0; i < node->func.args->len; i++) {
	// 	gen_node(node->func.args->data[i]);
	// }
	// EMIT(")");
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
		EMIT_DEFINE_GLOBAL(node->let.name->ident);
	} else {
		EMIT_DEFINE_LOCAL();
		if (node->let.value) {
			gen_node(node->let.value);
			EMIT_STORE_LOCAL(sym->id * 8);
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
	EMIT_FUNC_BEGIN(node->func.name->ident);
	gen_node(node->func.body);
	EMIT_PRINT();
	EMIT_FUNC_END();
}

static void gen_file(struct Node* node) {
	EMIT_FILE_BEGIN();

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
