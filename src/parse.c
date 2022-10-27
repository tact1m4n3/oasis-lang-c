#include "oasis.h"

static int s_precedences[] = {
	0, 0, // EOF, ILLEGAL
	0, 0, // IDENT, INT
	10, 20, 20, 30, 30, // ASSIGN, ADD, SUB, MUL, DIV
	0, 0, 0, 0, // LPAREN, RPAREN, LBRACE, RBRACE
	0, 0, // COMMA, SEMI
	0, 0, 0, // FUNC, LET, RETURN
};

static int s_tok;
static struct Scope* s_scope;
static List* s_unresolved;

static void next() {
	s_tok = scan_token();
}

static void expect(int tok) {
	ASSERT(tok == s_tok, "expected %s, got %s\n", token_name[tok], token_name[s_tok]);
	next();
}

static struct Node* parse_ident() {
	if (s_tok != T_IDENT) {
		expect(T_IDENT);
		return NULL;
	} else {
		struct Node* node = new_node(A_IDENT);
		node->ident = strdup(literal);
		next();
		return node;
	}
}

static List* parse_ident_list(int end) {
	List* list = new_list();
	if (s_tok == end) return list;

	append(list, parse_ident());
	while (s_tok == T_COMMA) {
		next();
		if (s_tok == end) break;
		append(list, parse_ident());
	}

	return list;
}

static List* parse_expr_list(int end);

static struct Node* parse_primary() {
	int op;
	struct Node* node, *right;
	struct Symbol* sym;
	switch (s_tok) {
	case T_IDENT:
		node = parse_ident();
		sym = resolve(s_scope, node->ident);
		if (sym)
			node->sym = sym;
		else
			append(s_unresolved, node);
		break;
	case T_INT:
		node = new_node(A_INT);
		node->intval = atoi(literal);
		next();
		break;
	case T_SUB:
		op = s_tok;
		next();

		right = parse_primary();

		node = new_node(A_UNARYOP);
		node->expr.op = op;
		node->expr.right = right;

		break;
	default:
		FATAL("expected expression, got %s\n", token_name[s_tok]);
	};

	if (node->kind == A_IDENT && s_tok == T_LPAREN) {
		next();
		List* args = parse_expr_list(T_RPAREN);
		expect(T_RPAREN);

		struct Node* call = new_node(A_CALL);
		call->func.name = node;
		call->func.args = args;
		return call;
	} else {
		return node;
	}
}

static struct Node* parse_expr(int prec) {
	struct Node* left = parse_primary();

	while (prec < s_precedences[s_tok]) {
		int op = s_tok;
		next();

		struct Node* node = new_node(A_BINOP);
		node->expr.left = left;
		node->expr.op = op;
		node->expr.right = parse_expr(s_precedences[op]);

		left = node;
	}

	return left;
}

static List* parse_expr_list(int end) {
	List* list = new_list();
	if (s_tok == end) return list;

	append(list, parse_expr(0));
	while (s_tok == T_COMMA) {
		next();
		if (s_tok == end) break;
		append(list, parse_expr(0));
	}

	return list;
}

static struct Node* parse_return() {
	expect(T_RETURN);
	struct Node* node = new_node(A_RETURN);
	node->value = NULL;
	if (s_tok != T_SEMI)
		node->value = parse_expr(0);
	expect(T_SEMI);
	return node;
}

static struct Node* parse_let() {
	expect(T_LET);

	struct Node* name = parse_ident();

	struct Node* value = NULL;
	if (s_tok != T_SEMI) {
		expect(T_ASSIGN);
		value = parse_expr(0);
	}
	expect(T_SEMI);

	struct Symbol* sym = new_sym(strdup(name->ident));
	define(s_scope, sym);

	struct Node* node = new_node(A_LET);
	node->sym = sym;
	node->let.name = name;
	node->let.value = value;
	return node;
}

static struct Node* parse_func();

static struct Node* parse_local_stmt() {
	struct Node* node;
	switch (s_tok) {
	case T_IDENT:
	case T_INT:
	case T_SUB:
		node = parse_expr(0);
		expect(T_SEMI);
		return node;
	case T_RETURN:
		return parse_return();
	case T_LET:
		return parse_let();
	case T_FUNC:
		return parse_func();
	default:
		FATAL("expected statement, got %s\n", token_name[s_tok]);
	};
}

static struct Node* parse_block() {
	expect(T_LBRACE);
	struct Node* node = new_node(A_BLOCK);

	node->list = new_list();
	while (s_tok != T_RBRACE && s_tok != T_EOF)
		append(node->list, parse_local_stmt());

	expect(T_RBRACE);
	return node;
}

static struct Node* parse_func() {
	expect(T_FUNC);

	struct Node* name = parse_ident();

	expect(T_LPAREN);
	List* args = parse_ident_list(T_RPAREN);
	expect(T_RPAREN);

	struct Node* body = NULL;
	if (s_tok != T_SEMI) {
		s_scope = new_scope(s_scope);
		for (int i = 0; i < args->len; i++) {
			struct Node* arg = args->data[i];
			struct Symbol* sym = new_sym(strdup(arg->ident));
			define(s_scope, sym);
		}

		body = parse_block();

		s_scope = s_scope->outer;
	} else {
		expect(T_SEMI);
	}

	struct Symbol* sym = new_sym(strdup(name->ident));
	define(s_scope, sym);

	struct Node* node = new_node(A_FUNC);
	node->sym = sym;
	node->func.name = name;
	node->func.args = args;
	node->func.body = body;
	return node;
}

static struct Node* parse_global_stmt() {
	struct Node* node;
	switch (s_tok) {
	case T_LET:
		return parse_let();
	case T_FUNC:
		return parse_func();
	default:
		FATAL("expected statement, got %s\n", token_name[s_tok]);
	};
}

void parser_init() {
	next();
	s_scope = new_scope(NULL);
	s_unresolved = new_list();
}

struct Node* parse_file() {
	struct Node* node = new_node(A_FILE);

	node->list = new_list();
	while (s_tok != T_EOF)
		append(node->list, parse_global_stmt());

	for (int i = 0; i < s_unresolved->len; i++) {
		struct Node* idnt = s_unresolved->data[i];
		struct Symbol* sym = resolve(s_scope, idnt->ident);
		ASSERT(sym, "name %s not defined\n", idnt->ident);
		idnt->sym = sym;
	}

	return node;
}
