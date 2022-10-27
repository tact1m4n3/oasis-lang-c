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

void print(struct Object* obj) {
	switch (obj->kind) {
	case O_NULL:
		printf("NULL\n");
	case O_INT:
		printf("%d\n", obj->intval);
	}
}
