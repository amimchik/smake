#ifndef _PARSER_H
#define _PARSER_H

#include <smake/lexer.h>

#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>


enum node_type {
	NT_VAR_DECL,
	NT_TARGET_DECL,
};

struct node {
	int type;
	int line;
	int column;
	union {
		struct {
			char *name;
			char *value;
		} var_decl;
		struct {
			char *name;
			int dependencies_c;
			char **dependencies_v;
			char *value;
		} target_decl;
	};
	struct node *next;
};

struct node *parse(const struct token *tokens);

#endif /*_PARSER_H*/
