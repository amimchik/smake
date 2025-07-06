#ifndef _DICT_H
#define _DICT_H

#include <stdlib.h>
#include <string.h>

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

void free_dict(struct dict *dict);

#endif /*!_DICT_H*/
