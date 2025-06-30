#ifndef _LEXER_H
#define _LEXER_H

#include <ctype.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

struct lexer_state {
	const char *input;
	int length;
	int current;
	int line;
	int column;
};

enum token_type {
	TT_WORD,
	TT_DOLLAR,
	TT_MODULO,
	TT_COLON,
	TT_UNKNOWN,
	TT_EOF,
};

struct token {
	int type;
	union {
		char *word;
	};
	int line;
	int column;
	struct token *next;
};


struct lexer_state make_lexer(const char *input);


struct token *tokenize(struct lexer_state *lexer);


#endif /*!_LEXER_H*/
