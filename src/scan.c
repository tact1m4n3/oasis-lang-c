#include "oasis.h"

char literal[100];

static FILE* s_file;
static int s_pos;
static char s_ch;
static int s_insert_semi;

static int is_letter(char c) {
	return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

static int is_digit(char c) {
	return c >= '0' && c <= '9';
}

static void next() {
	s_ch = getc(s_file);
	s_pos++;
}

static void skip_whitespace() {
	while (s_ch == ' ' || s_ch == '\t' || s_ch == '\r' || (s_ch == '\n' && !s_insert_semi))
		next();
}

static int scan_ident() {
	memset(literal, '\0', 100);

	int i = 0;
	while (s_ch == '_' || is_letter(s_ch) || is_digit(s_ch)) {
		literal[i] = s_ch;
		next();
		i++;
	}

	return lookup_ident(literal);
}

static int scan_number() {
	memset(literal, '\0', 100);

	int i = 0;
	while (is_digit(s_ch)) {
		literal[i] = s_ch;
		next();
		i++;
	}

	return T_INT;
}

void scanner_init(char* fname) {
	s_file = fopen(fname, "r");
	ASSERT(s_file, "failed to open file %s\n", fname);

	s_pos = -1;
	s_ch = '\0';
	s_insert_semi = 0;

	next();
}

int scan_token() {
	skip_whitespace();

	if (s_insert_semi && (s_ch == EOF || s_ch == '\n')) {
		s_insert_semi = 0;
		return T_SEMI;
	}

	int tok;
	s_insert_semi = 0;
	switch (s_ch) {
	case EOF:
		tok = T_EOF;
		break;
	case '=':
		tok = T_ASSIGN;
		break;
	case '+':
		tok = T_ADD;
		break;
	case '-':
		tok = T_SUB;
		break;
	case '*':
		tok = T_MUL;
		break;
	case '/':
		tok = T_DIV;
		break;
	case '(':
		tok = T_LPAREN;
		break;
	case ')':
		s_insert_semi = 1;
		tok = T_RPAREN;
		break;
	case '{':
		tok = T_LBRACE;
		break;
	case '}':
		tok = T_RBRACE;
		break;
	case ',':
		tok = T_COMMA;
		break;
	case ';':
		tok = T_SEMI;
		break;
	default:
		if (s_ch == '_' || is_letter(s_ch)) {
			s_insert_semi = 1;
			tok = scan_ident();
			return tok;
		} else if (is_digit(s_ch)) {
			s_insert_semi = 1;
			tok = scan_number();
			return tok;
		} else {
			tok = T_ILLEGAL;
		}
	}

	next();

	return tok;
}
