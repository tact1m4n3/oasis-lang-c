#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* -------------- object --------------- */

enum {
	O_NULL,
	O_INT,
};

struct Object {
	int kind;
	union {
		int intval;
	};
};

struct Object* new_obj(int kind);
struct Object* new_int(int value);
void print(struct Object* obj);
