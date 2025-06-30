#include <smake/lexer.h>

#define DEBUG_LEXER

#ifdef DEBUG_LEXER
#include <stdio.h>
#endif /*DEBUG_LEXER*/


struct lexer_state make_lexer(const char *input)
{
	struct lexer_state lexer;
	lexer.input = input;
	lexer.length = strlen(input);
	lexer.current = 0;
	lexer.line = 1;
	lexer.column = 0;
	return lexer;
}

static char next_c(struct lexer_state *lexer)
{
	if (!lexer)
		return 0;

	if (!lexer->input)
		return 0;

	if (lexer->current >= lexer->length)
		return 0;

	return lexer->input[lexer->current++];
}

static char peek(struct lexer_state *lexer, int offset)
{
	if (!lexer)
		return 0;

	size_t index = lexer->current + offset;

	if (index < 0 || index >= lexer->length)
		return 0;

	return lexer->input[index];
}

static int next_token(struct lexer_state *lexer, struct token *token)
{
	if (!lexer)
		return 1;

	if (!token)
		return 1;

	int line = lexer->line, column = lexer->column;

	char c = peek(lexer, 0);

	while (1) {
		if (isalnum(c)) {
			int cap = 10;
			char *str = malloc(cap);
			int size = 0;
			while (isalnum(c)) {
				if (size + 1 >= cap) {
					char *tmp = realloc(str, cap * 2);
					if (!tmp) {
						free(str);
#ifdef DEBUG_LEXER
						printf("realloc error\n");
#endif /*DEBUG_LEXER*/
						return 1;
					}
					continue;
				}
				str[size++] = c;
				c = next_c(lexer);
			}
			str[size] = '\0';
			token->type = TT_WORD;
			token->word = str;
			token->line = line;
			token->column = column;
			return 0;
		} else if (c == '$') {
		token->type = TT_DOLLAR;
		return 0;
		} else if (c == ':') {
			token->type = TT_COLON;
			return 0;
		} else if (c == '%') {
			token->type = TT_MODULO;
			return 0;
		} else if (!c) {
			token->type == TT_EOF;
			return 0;
		}
	}
}

struct token *tokenize(struct lexer_state *lexer)
{
	if (!lexer)
		return NULL;

	char c;

	struct token *head = NULL;
	struct token *current = head;

	if (!head)
		return NULL;

	while (c = peek(lexer, 0)) {
		struct token token;
		struct token *prev = current;
		current = malloc(sizeof(struct token));
		if (!current)
#ifdef DEBUG_LEXER
#endif /*DEBUG_LEXER*/
			return NULL;

		if (prev)
			prev->next = current;
		else
			head = current;

		if (next_token(lexer, &token))
			return NULL;

		*current = token;
	}

	return head;
}
