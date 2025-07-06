#include <smake/executor.h>

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
        free(expanded);
        free(buf);
        return NULL;
}


struct target {
	char *name;
	int deps_c;
	char **deps;
	char *value;
	struct target *next;
};

struct executor_state {
	struct dict vars;
	struct target *targets;
};

static int execute_node(struct node node)
{
}

int execute(struct node *nodes)
{
	struct executor_state executor;
	executor.vars = make_dict(5);
	
	free_dict(&vars);
}
