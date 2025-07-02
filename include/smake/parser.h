#ifndef _PARSER_H
#define _PARSER_H

#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>

struct parser_state {};

struct dict_node {
	char *key;
	char *value;
};

struct dict {
	int cap;
	int len;
	struct dict_node *nodes;
};

struct dict make_dict(int initial_cap);

int dict_set(struct dict *dict, const char *key, const char *value);
char *dict_get(struct dict *dict, const char *key);


char *expand(struct dict *vars, const char *str);


enum node_type {
	NT_VAR_DECL,
	NT_TARGET_DECL,
};

struct node {
	int type;
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
};



#endif /*_PARSER_H*/
