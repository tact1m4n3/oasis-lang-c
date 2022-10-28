#include "oasis.h"

struct Node* new_node(int kind) {
	struct Node* node = malloc(sizeof(struct Node));
	node->kind = kind;
	return node;
}

void debug_node(struct Node* node) {
	switch (node->kind) {
	case A_IDENT:
		printf("%s", node->ident);
		break;
	case A_INT:
		printf("%d", node->intval);
		break;
	case A_UNARYOP:
		printf("%s", token_name[node->expr.op]);
		debug_node(node->expr.right);
		break;
	case A_BINOP:
		debug_node(node->expr.left);
		printf("%s", token_name[node->expr.op]);
		debug_node(node->expr.right);
		break;
	case A_CALL:
		debug_node(node->func.name);
		printf("(");
		for (int i = 0; i < node->func.args->len; i++) {
			debug_node(node->func.args->data[i]);
			printf(",");
		}
		printf(")");
		break;
	case A_RETURN:
		printf("return");
		if (node->value) {
			printf(" ");
			debug_node(node->value);
		}
		break;
	case A_LET:
		printf("let ");
		debug_node(node->let.name);
		if (node->let.value) {
			printf(" = ");
			debug_node(node->let.value);
		}
		break;
	case A_BLOCK:
		printf("{");
		for (int i = 0; i < node->list->len; i++) {
			debug_node(node->list->data[i]);
			printf(";");
		}
		printf("}");
		break;
	case A_FUNC:
		printf("func ");
		debug_node(node->func.name);
		printf("(");
		for (int i = 0; i < node->func.args->len; i++) {
			debug_node(node->func.args->data[i]);
			printf(",");
		}
		printf(")");
		if (node->func.body)
			debug_node(node->func.body);
		break;
	case A_FILE:
		for (int i = 0; i < node->list->len; i++) {
			debug_node(node->list->data[i]);
			printf(";");
		}
		printf("\n");
		break;
	};
}
