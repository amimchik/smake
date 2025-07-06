#include <smake/lexer.h>

/*#define DEBUG_LEXER*/

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

	return lexer->input[++lexer->current];
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

	token->next = NULL;

	int line = lexer->line, column = lexer->column;

	char c = peek(lexer, 0);

	while (1) {
#ifdef DEBUG_LEXER
		printf("cycle iteration\n");
#endif /*DEBUG_LEXER*/
		if (isalnum(c)) {
			int cap = 10;
			char *str = malloc(cap);
			if (!str) {
#ifdef DEBUG_LEXER
				printf("alloc_err\n");
#endif /*DEBUG_LEXER*/
				return 1;
			}
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
#ifdef DEBUG_LEXER
			printf("word:%s\n", str);
#endif /*DEBUG_LEXER*/
			return 0;
		} else if (c == '$') {
			token->type = TT_DOLLAR;
			c = next_c(lexer);
			return 0;
		} else if (c == ':') {
			token->type = TT_COLON;
			c = next_c(lexer);
			return 0;
		} else if (c == '%') {
			c = next_c(lexer);
			int cap = 10;
			char *str = malloc(cap);
			int size = 0;
			int covered = 0;
			while (covered || c != '%') {
#ifdef DEBUG_LEXER
				printf("str;parsing:%c\n", c);
#endif /*DEBUG_LEXER*/
				if (!c) {
#ifdef DEBUG_LEXER
					printf("unexpected EOF\n");
#endif /*DEBUG_LEXER*/
					return 1;
				}
				covered = c == '\\';
				if (size + 1 >= cap) {
#ifdef DEBUG_LEXER
					printf("NOT ENOUGHT MEM:REALLOC\n");
#endif /*DEBUG_LEXER*/
					char *tmp = realloc(str, cap * 2);
					if (!tmp) {
						free(str);
#ifdef DEBUG_LEXER
						printf("realloc error\n");
#endif /*DEBUG_LEXER*/
						return 1;
					}
					cap *= 2;
				}
				str[size++] = c;
				c = next_c(lexer);
			}
			str[size] = '\0';
			token->type = TT_STR_LIT;
			token->str_lit = str;
			token->line = line;
			token->column = column;
#ifdef DEBUG_LEXER
			printf("str_lit:%s\n", str);
#endif /*DEBUG_LEXER*/
			c = next_c(lexer);
			return 0;
		} else if (c == '$') {
			c = next_c(lexer);
			return 0;
		} else if (!c) {
			token->type = TT_EOF;
			c = next_c(lexer);
			return 0;
		} else {
			c = next_c(lexer);
		}
	}
}

struct token *tokenize(struct lexer_state *lexer)
{
	if (!lexer)
		return NULL;

	struct token *head = NULL;
	struct token *current = head;

	while (1) {
		struct token token;
		struct token *prev = current;
		current = malloc(sizeof(struct token));
		if (!current) {
#ifdef DEBUG_LEXER
			printf("token alloc err\n");
#endif /*DEBUG_LEXER*/
			return NULL;
		}

		if (prev)
			prev->next = current;
		else
			head = current;

		if (next_token(lexer, &token)) {
			return NULL;
#ifdef DEBUG_LEXER
			printf("token parsing error\n");
#endif /*DEBUG_LEXER*/
		}

		*current = token;

		if (token.type == TT_EOF) {
#ifdef DEBUG_LEXER
			printf("EOF detected\n");
#endif /*DEBUG_LEXER*/
			break;
		}
	}

	return head;
}
