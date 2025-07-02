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

int main()
{
	char *input = "$$ <- dollar $cat$sigma some teset $sigm2\n";
	struct dict dict = make_dict(0);
	dict_set(&dict, "sigma", "boy");
	dict_set(&dict, "sigm2", "boy2");
	dict_set(&dict, "cat", "boy3");
	printf("INPUT:%s\n", input);
	printf("dict_nodes:\n");
	for (int i = 0; i < dict.len; i++) {
		printf("\t%s:%s\n", dict.nodes[i].key, dict.nodes[i].value);
	}
	printf("\n");
	char *str = expand(&dict, input);
	if (!str)
		printf("OUTPUT: (null)\n");
	else
		printf("OUTPUT: %s\n", str);
	free(str);
	str = NULL;
	return 0;
}
