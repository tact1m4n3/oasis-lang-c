#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* --------------- utils --------------- */

#define FATAL(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__), exit(1)
#define ASSERT(x, fmt, ...) if (!(x)) { FATAL(fmt, ##__VA_ARGS__); }

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
