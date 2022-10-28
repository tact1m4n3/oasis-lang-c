#include "oasislib.h"

struct Object* new_obj(int kind) {
	struct Object* obj = malloc(sizeof(struct Object));
	obj->kind = kind;
	return obj;
}

struct Object* new_null() {
	return new_obj(O_NULL);
}

struct Object* new_int(int value) {
	struct Object* obj = new_obj(O_INT);
	obj->intval = value;
	return obj;
}

void obj_print(struct Object* obj) {
	if (obj == NULL)
		FATAL("got a null ptr\n");

	switch (obj->kind) {
	case O_NULL:
		printf("NULL\n");
	case O_INT:
		printf("%d\n", obj->intval);
	}
}

struct Object* obj_add(struct Object* left, struct Object* right) {
	if (left == NULL || right == NULL)
		FATAL("got a null ptr\n");

	if (left->kind == O_INT && right->kind == O_INT)
		return new_int(left->intval + right->intval);
	else
		FATAL("invalid operation\n");
}

struct Object* obj_sub(struct Object* left, struct Object* right) {
	if (left == NULL || right == NULL)
		FATAL("got a null ptr\n");

	if (left->kind == O_INT && right->kind == O_INT)
		return new_int(left->intval - right->intval);
	else
		FATAL("invalid operation\n");
}

struct Object* obj_mul(struct Object* left, struct Object* right) {
	if (left == NULL || right == NULL)
		FATAL("got a null ptr\n");

	if (left->kind == O_INT && right->kind == O_INT)
		return new_int(left->intval * right->intval);
	else
		FATAL("invalid operation\n");
}

struct Object* obj_div(struct Object* left, struct Object* right) {
	if (left == NULL || right == NULL)
		FATAL("got a null ptr\n");

	if (left->kind == O_INT && right->kind == O_INT)
		if (right->intval == 0)
			FATAL("division by 0\n");
		else
			return new_int(left->intval / right->intval);
	else
		FATAL("invalid operation\n");
}
