#include <smake/parser.h>

/*#define DEBUG_PARSER*/

#include <stdio.h>

#ifdef DEBUG_PARSER
#include <unistd.h>
#endif /*DEBUG_PARSER*/



struct dict make_dict(int initial_cap)
{
	struct dict dict;
	dict.cap = 0;
	dict.len = 0;
	if (initial_cap <= 0) {
		dict.nodes = NULL;
		return dict;
	}
	dict.nodes = malloc(sizeof(struct node) * initial_cap);
	if (!dict.nodes) {
		return dict;
	} else {
		dict.cap = initial_cap;
	}
	return dict;
}

char *dict_get(struct dict *dict, const char *key)
{
	if (!dict)
		return NULL;

	for (int i = 0; i < dict->len; i++) {
		if (!strcmp(key, dict->nodes[i].key)) {
			char *ret = malloc(strlen(dict->nodes[i].value));
			if (!ret)
				return NULL;
			strcpy(ret, dict->nodes[i].value);
			return ret;
		}
	}
	
	return NULL;
}

int dict_set(struct dict *dict, const char *key, const char *value)
{
	if (!dict)
		return 1;

	for (int i = 0; i < dict->len; i++) {
		if (!strcmp(key, dict->nodes[i].key)) {
			free(dict->nodes[i].value);
			dict->nodes[i].value = malloc(strlen(value));
			if (!dict->nodes[i].value)
				return 1;
			strcpy(dict->nodes[i].value, value);
			return 0;
		}
	}

#ifdef DEBUG_PARSER
	printf("DICT\nCAP=%d;\nLEN=%d;\nPTR=%p\n", dict->cap, dict->len, dict->nodes);
#endif /*DEBUG_PARSER*/
	if (dict->len >= dict->cap) {
#ifdef DEBUG_PARSER
		printf("DICT\nCAP=%d;\nLEN=%d;\nPTR=%p\n", dict->cap, dict->len, dict->nodes);
		printf("EXPECTED_REALLOC=%d;\n", dict->cap > 0 ? (dict->cap * 2) : 1);
#endif /*DEBUG_PARSER*/
		struct dict_node *tmp = realloc(dict->nodes, sizeof(struct dict_node) * (dict->cap > 0 ? (dict->cap * 2) : 1));
#ifdef DEBUG_PARSER
		printf("DICT\nCAP=%d;\nLEN=%d;\nPTR=%p\n", dict->cap, dict->len, dict->nodes);
#endif /*DEBUG_PARSER*/
		if (!tmp)
			return 1;

		dict->nodes = tmp;

		if (dict->cap != 0)
			dict->cap *= 2;
		else
			dict->cap = 1;
	}

	dict->nodes[dict->len].key = malloc(strlen(key));
	if (!dict->nodes[dict->len].key)
		return 1;

	strcpy(dict->nodes[dict->len].key, key);

	dict->nodes[dict->len].value = malloc(strlen(value));
	if (!dict->nodes[dict->len].value)
		return 1;

	strcpy(dict->nodes[dict->len].value, value);

	dict->len++;
	return 0;
}

static int expand_str_cap(char **str, int target_cap)
{
	if (!str)
		return 1;
	if (!*str)
		return 1;

	char *tmp = realloc(*str, target_cap);

	if (!tmp)
		return 1;

	*str = tmp;

	return 0;
}

static int expand_buf(struct dict *dict, char **expanded, int *cap, int *len, char *buf, int *buf_len)
{
#ifdef DEBUG_PARSER
	printf("GETTING:'%s'%ld\n", buf, strlen(buf));
#endif /*DEBUG_PARSER*/
	char *val = dict_get(dict, buf);
	if (!val)
		return 1;
	int val_len = strlen(val);
	for (int i = 0; i < val_len; i++) {
		if ((*len) + 1 >= (*cap)) {
			if (expand_str_cap(expanded, (*cap) * 2))
				return 1;
			(*cap) *= 2;
		}
		(*expanded)[(*len)++] = val[i];
	}
	*buf_len = 0;
	return 0;
}

char *expand(struct dict *dict, const char *str)
{
	int cap = 10;
	int len = 0;

	char *expanded = malloc(cap);

	if (!expanded)
		return NULL;


	int buf_cap = 10;
	int buf_len = 0;

	char *buf = malloc(buf_cap);

	if (!buf)
		return NULL;

	int covered = 0;
	int prevdollar = 0;

	while (*str) {
#ifdef DEBUG_PARSER
		printf("BUFF:%s\n", buf);
		printf("OUTPUT:%s\n", expanded);
#endif /*DEBUG_PARSER*/
		char c = *str;
		if (*str == '$' && !covered) {
			prevdollar = 1;
			covered = 1;
			str++;
		} else if (c == '$' && prevdollar) {
			covered = 0;
			prevdollar = 0;
			if (len + 1 >= cap) {
				if (!expand_str_cap(&expanded, cap  * 2))
					goto err;
				cap *= 2;
			}
			expanded[len++] = '$';
			str++;
		} else if (c == '$' && covered && !prevdollar) {
			prevdollar = 1;
			if (buf_len)
				if (expand_buf(dict, &expanded, &cap, &len, buf, &buf_len))
					goto err;
			str++;
		} else if (covered && isalnum(c)) {
			prevdollar = 0;
			if (buf_len + 1 >= buf_cap) {
				if (!expand_str_cap(&buf, buf_cap * 2))
					goto err;
				buf_cap *= 2;
			}
			buf[buf_len++] = c;
			str++;
		} else {
			covered = 0;
			prevdollar = 0;
#ifdef DEBUG_PARSER
			printf("ELSEDONE\n");
#endif /*DEBUG_PARSER*/
			if (buf_len)
				if (expand_buf(dict, &expanded, &cap, &len, buf, &buf_len))
					goto err;
			if (len + 1 >= cap) {
				if (expand_str_cap(&expanded, cap  * 2))
					goto err;
				cap *= 2;
			}
			expanded[len++] = c;
			str++;
		}
	}
	if (buf_len)
		if (expand_buf(dict, &expanded, &cap, &len, buf, &buf_len))
			goto err;

	return expanded;
err:
#ifdef DEBUG_PARSER
	printf("EROR\n");
#endif /*DEBUG_PARSER*/
	free(expanded);
	free(buf);
	fprintf(stderr, "error while expanding string\n");
	return NULL;
}
