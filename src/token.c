#include "oasis.h"

#define KEYWORD_START T_FUNC

char* token_name[] = {
	"EOF", "ILLEGAL",
	"IDENT", "INT",
	"=", "+", "-", "*", "/",
	"(", ")", "{", "}",
	",", ";",
	"func", "let", "return",
};

static char* keywords[] = {
	"func", "let", "return",
};

int lookup_ident(char* ident) {
	for (int i = 0; i < sizeof(keywords) / sizeof(char*); i++)
		if (!strcmp(ident, keywords[i]))
			return KEYWORD_START + i;
	return T_IDENT;
}
