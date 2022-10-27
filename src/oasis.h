#pragma once

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* --------------- utils --------------- */

#define FATAL(fmt, ...) { fprintf(stderr, fmt, ##__VA_ARGS__); exit(1); }
#define ASSERT(x, fmt, ...) if (!(x)) { FATAL(fmt, ##__VA_ARGS__); }

/* --------------- list ---------------- */

typedef struct {
	int len;
	int cap;
	void** data;
} List;

List* new_list();
void extend(List* list, int delta);
void append(List* list, void* val);

/* --------------- token --------------- */

enum {
	T_EOF, T_ILLEGAL,
	T_IDENT, T_INT,
	T_ASSIGN, T_ADD, T_SUB, T_MUL, T_DIV,
	T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE,
	T_COMMA, T_SEMI,
	T_FUNC, T_LET, T_RETURN,
};

extern char* token_name[];

int lookup_ident(char* ident);

/* ---------------- sym ---------------- */

struct Symbol {
	char* name;
	int global;
	int id;
};

struct Scope {
	struct Scope* outer;
	List* syms;
	int next_id;
};

struct Symbol* new_sym(char* name);
struct Scope* new_scope(struct Scope* outer);
void define(struct Scope* scope, struct Symbol* sym);
struct Symbol* resolve(struct Scope* scope, char* name);

/* ---------------- ast ---------------- */

enum {
	A_IDENT, A_INT,
	A_UNARYOP, A_BINOP,
	A_BLOCK, A_FUNC,
	A_CALL,
	A_LET, A_RETURN,
	A_FILE,
};

struct Node {
	int kind;
	struct Symbol* sym;
	union {
		// IDENT
		char* ident;

		// INT
		int intval;

		// UNARYOP, BINOP
		struct {
			struct Node* left;
			int op;
			struct Node* right;
		} expr;

		// RETURN
		struct Node* value;

		// LET
		struct {
			struct Node* name;
			struct Node* value;
		} let;

		// FUNC, CALL
		struct {
			struct Node* name;
			List* args;
			struct Node* body;
		} func;

		// BLOCK, FILE
		List* list;
	};
};

extern char* node_name[];

struct Node* new_node(int kind);
void debug_node(struct Node* node);

/* -------------- scanner -------------- */

extern char literal[];

void scanner_init(char* fname);
int scan_token();

/* -------------- parser --------------- */

void parser_init();
struct Node* parse_file();

/* --------------- gen ----------------- */

void gen_init();
void gen_node(struct Node* node);
