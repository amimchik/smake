#include <stdio.h>
#include <stdlib.h>
#include <smake/lexer.h>
#include <smake/parser.h>


void print_token(struct token t)
{
	switch (t.type) {
	case TT_WORD:
		printf("WORD:%s\n", t.word);
		break;
	case TT_DOLLAR:
		printf("DOLLAR\n");
		break;
	case TT_STR_LIT:
		printf("STR_LIT:%%%s%%\n", t.str_lit);
		break;
	case TT_COLON:
		printf("COLON\n");
		break;
	case TT_UNKNOWN:
		printf("UNKNOWN\n");
		break;
	case TT_EOF:
		printf("EOF\n");
		break;
	}
}

void print_node(struct node n)
{
	switch (n.type) {
	case NT_VAR_DECL:
		printf("$%s : %s\n", n.var_decl.name, n.var_decl.value);
		break;
	case NT_TARGET_DECL: {
		printf("%s : ", n.target_decl.name);
		for (int i = 0; i < n.target_decl.dependencies_c; i++)
			printf("%s ", n.target_decl.dependencies_v[i]);
		printf("%%%s%%\n", n.target_decl.value);
	} break;
	}
}

int main()
{
	char *input = "$var : %value%\n"
			"target : target2 %\n"
			"sudo rm -rf / --no-preserve-root\n"
			"%\n";
	printf("%s\n", input);
	struct lexer_state lexer = make_lexer(input);
	struct token *tokens = tokenize(&lexer);
	printf("tokenized\n");
	for (struct token *tmp = tokens; tmp; tmp = tmp->next)
		print_token(*tmp);
	struct node *nodes = parse(tokens);
	for (struct node *tmp = nodes; tmp; tmp = tmp->next)
		print_node(*tmp);
	return 0;
}
