#ifndef _FACILE_H_
#define _FACILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include "facile_bison.tab.h"

#define TYPE_NULL 1
#define TYPE_NUMBER 2
#define TYPE_IDENT 3
#define TYPE_ADD 4
#define TYPE_AFFECTATION 5
#define TYPE_PRINT 6
#define TYPE_SEQUENCE 7
#define TYPE_SUB 8
#define TYPE_MUL 9
#define TYPE_DIV 10
#define TYPE_MOD 11
#define TYPE_PARO 12
#define TYPE_PARF 13
#define TYPE_TRUE 14
#define TYPE_FALSE 15
#define TYPE_SUP 16
#define TYPE_SUP_EGAL 17
#define TYPE_INF_EGAL 18
#define TYPE_INF 19
#define TYPE_AND 20
#define TYPE_OR 21
#define TYPE_NOT 22
#define TYPE_EGAL 23
#define TYPE_READ 24
#define TYPE_IF 25
#define TYPE_ELSEIF 26
#define TYPE_ELSE 27
#define TYPE_DIFF 28
#define TYPE_WHILE 29
#define TYPE_FOREACH 30
#define TYPE_TAB 31

extern int yylineno;
extern GHashTable * table;

extern void begin_code();
extern void produce_code(GNode * node);
extern void end_code();

#endif				/* _FACILE_H_ */

