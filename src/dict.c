#include <smake/dict.h>

struct dict make_dict(int initial_cap)
{
        struct dict dict;
        dict.cap = 0;
        dict.len = 0;
        if (initial_cap <= 0) {
                dict.nodes = NULL;
                return dict;
        }
        dict.nodes = malloc(sizeof(struct dict_node) * initial_cap);
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
        if (dict->len >= dict->cap) {
                struct dict_node *tmp = realloc(dict->nodes, sizeof(struct dict_node) * (dict->cap > 0 ? (dict->cap * 2) : 1));
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

void free_dict(struct dict *dict)
{
	if (!dict)
		return;
	for (int i = 0; i < dict->len; i++) {
		free(dict->nodes[i].key);
		free(dict->nodes[i].value);
	}
	free(dict->nodes);
	dict->cap = 0;
	dict->len = 0;
	dict->nodes = NULL;
}
