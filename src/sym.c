#include "oasis.h"

struct Symbol* new_sym(char* name) {
	struct Symbol* sym = malloc(sizeof(struct Symbol));
	sym->name = name;
	return sym;
}

struct Scope* new_scope(struct Scope* outer) {
	struct Scope* scope = malloc(sizeof(struct Scope));
	scope->outer = outer;
	scope->syms = new_list();
	scope->next_id = 0;
	return scope;
}

void define(struct Scope* scope, struct Symbol* sym) {
	sym->global = (scope->outer) ? 0 : 1;
	sym->id = scope->next_id++;
	append(scope->syms, sym);
}

struct Symbol* resolve(struct Scope* scope, char* name) {
	for (struct Scope* cur = scope; cur != NULL; cur = cur->outer) {
		for (int i = 0; i < scope->syms->len; i++) {
			struct Symbol* sym = scope->syms->data[i];
			if (!strcmp(name, sym->name))
				return sym;
		}
	}
	return NULL;
}
