#include <smake/parser.h>

/*#define DEBUG_PARSER*/

#include <stdio.h>

#ifdef DEBUG_PARSER
#include <unistd.h>
#endif /*DEBUG_PARSER*/


static int match(const struct token **tokens, int type)
{
	if (!tokens)
		return 0;

	if (!*tokens)
		return 0;

	return (**tokens).type == type;
}

struct node parse_var(const struct token **tokens)
{
	printf("parsevar\n");
	struct node node;
	node.type = NT_VAR_DECL;
	*tokens = (*tokens)->next;
	printf("%d\n", (*tokens)->type);
	
	if (!match(tokens, TT_WORD)) {
		fprintf(stderr, "unexpected token\n");
		return node;
	}

	char *name = malloc(strlen((**tokens).word));

	if (!name) {
		fprintf(stderr, "alloc err");
		return node;
	}

	strcpy(name, (**tokens).word);

	*tokens = (*tokens)->next;
	
	if (!match(tokens, TT_COLON)) {
		fprintf(stderr, "unexpected token\n");
		return node;
	}

	*tokens = (*tokens)->next;

	if (!match(tokens, TT_STR_LIT)) {
		fprintf(stderr, "unexpected token\n");
		return node;
	}

	char *value = malloc(strlen((*tokens)->str_lit));

	if (!value) {
		fprintf(stderr, "alloc err\n");
		return node;
	}

	strcpy(value, (*tokens)->str_lit);

	*tokens = (*tokens)->next;

	node.var_decl.name = name;
	node.var_decl.value = value;

	return node;
}

struct node parse_target(const struct token **tokens)
{
	printf("target\n");
	struct node node;
	node.type = NT_TARGET_DECL;
	node.target_decl.name = NULL;
	node.target_decl.value = NULL;
	node.target_decl.dependencies_v = NULL;
	node.target_decl.dependencies_c = 0;

	if (!tokens || !*tokens) {
		return node;
	}

	if (!match(tokens, TT_WORD)) {
		fprintf(stderr, "unexpected token\n");
		return node;
	}
	char *name = malloc(strlen((*tokens)->word));
	if (!name) {
		fprintf(stderr, "alloc err\n");
		return node;
	}
	strcpy(name, (*tokens)->word);
	*tokens = (**tokens).next;
	if (!match(tokens, TT_COLON)) {
		fprintf(stderr, "unexpected token\n");
		return node;
	}
	*tokens = (**tokens).next;
	int deps_cap = 5;
	int deps_count = 0;
	char **dependencies = malloc(sizeof(char *) * deps_cap);
	if (!dependencies) {
		fprintf(stderr, "alloc err\n");
		return node;
	}
	while (match(tokens, TT_WORD)) {
		if (deps_count >= deps_cap) {
			char **tmp = realloc(dependencies, deps_cap * 2);
			if (!tmp) {
				for (int i = 0; i < deps_count; i++)
					free(dependencies[i]);
				free(dependencies);
				fprintf(stderr, "realloc err\n");
				return node;
			}
			dependencies = tmp;
		}
		dependencies[deps_count] = malloc(strlen((*tokens)->word));
		if (!dependencies[deps_count]) {
			fprintf(stderr, "alloc err\n");
			return node;
		}
		strcpy(dependencies[deps_count++], (*tokens)->word);
		*tokens = (**tokens).next;
	}
	if (!match(tokens, TT_STR_LIT)) {
		fprintf(stderr, "unexpected token\n");
		return node;
	}
	char *value = malloc(strlen((*tokens)->str_lit));
	if (!value) {
		fprintf(stderr, "alloc err\n");
		return node;
	}
	strcpy(value, (*tokens)->str_lit);
	*tokens = (**tokens).next;
	node.target_decl.name = name;
	node.target_decl.dependencies_c = deps_count;
	node.target_decl.dependencies_v = dependencies;
	node.target_decl.value = value;
	return node;
}

struct node parse_next(const struct token **tokens)
{
	if (match(tokens, TT_DOLLAR))
		return parse_var(tokens);
	else
		return parse_target(tokens);
}

struct node *parse(const struct token *tokens)
{
	struct node *head = NULL;
	struct node *current = NULL;
	while (tokens && tokens->type != TT_EOF) {
		printf("TYPE:%d\n", tokens->type);
		struct node parsed = parse_next(&tokens);
		if (!head) {
			head = malloc(sizeof(struct node));
			if (!head)
				return NULL;
			current = head;
			*current = parsed;
		} else {
			current->next = malloc(sizeof(struct node));
			if (!current->next)
				return NULL;
			*(current->next) = parsed;
			current = current->next;
		}
		current->next = NULL;
	}
	return head;
}
