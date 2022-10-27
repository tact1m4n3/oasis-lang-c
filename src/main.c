#include "oasis.h"

int main(int argc, char** argv) {
	ASSERT(argc >= 3, "usage: oasisc <INPUT> <OUTPUT>\n");
	char* input_fname = argv[1];
	char* output_fname = argv[2];

	scanner_init(input_fname);
	parser_init();
	gen_init(output_fname);

	struct Node* node = parse_file();
	debug_node(node);

	gen_node(node);

	return 0;
}
