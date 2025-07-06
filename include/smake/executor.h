#ifndef _EXECUTOR_H
#define _EXECUTOR_H

#include <smake/parser.h>
#include <smake/dict.h>

char *expand(struct dict *vars, const char *str);

int execute(struct node *nodes);

#endif /*!_EXECUTOR_H*/
