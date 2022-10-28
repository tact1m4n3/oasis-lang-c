#include "oasislib.h"

extern struct Object* user_main();

int main(int argc, char** argv) {
	struct Object* ret = user_main();
	if (!ret) FATAL("something went wrong\n");
	if (ret->kind == O_INT)
		return ret->intval;
	else
		return 0;
}
