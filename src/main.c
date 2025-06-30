#include <stdio.h>
#include <smake/lexer.h>

int main()
{
	char *input = "test $ ";
	struct lexer_state lexer = make_lexer(input);
	struct token *head = tokenize(&lexer);
	if (!head) {
		printf("NULL RETURNED\n");
		return 1;
	}
	struct token *tmp = head;
	while (tmp) {
		printf("%d\n", tmp->type);
		tmp = tmp->next;
	}
	return 0;
}
