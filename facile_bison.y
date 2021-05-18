%{

#include "facile.h"

%}

%union {
	long number;
	char *string;
	GNode * node;
}

%left			TOK_ADD
%left			TOK_SUB
%left			TOK_MUL
%left			TOK_DIV
%left			TOK_MOD
%left			TOK_PARO
%left			TOK_PARF
%token			TOK_AND
%token			TOK_OR
%token			TOK_DIFF
%token			TOK_END
%token			TOK_EGAL
%token			TOK_INF
%token			TOK_NOT
%token			TOK_SUP
%token			TOK_TRUE
%token			TOK_FALSE
%token			TOK_ELSE
%token			TOK_ELSEIF
%token			TOK_IF
%token			TOK_THEN
%token<number>		TOK_NUMBER
%token			TOK_AFFECT
%token			TOK_SEMICOLON
%token			TOK_PRINT
%token			TOK_READ
%token<string>		TOK_IDENT
%token			TOK_DO
%token			TOK_WHILE
%token			TOK_FOREACH
%token			TOK_IN

%type<node> code
%type<node> instruction
%type<node> affectation
%type<node> print
%type<node> read
%type<node> expr
%type<node> number
%type<node> ident
%type<node> addition
%type<node> soustraction
%type<node> multiplication
%type<node> division
%type<node> modulo
%type<node> boolean_expr
%type<node> and
%type<node> or
%type<node> not
%type<node> egal
%type<node> inferieur_egal
%type<node> superieur_egal
%type<node> superieur
%type<node> inferieur
%type<node> if
%type<node> elseif
%type<node> else
%type<node> different
%type<node> while
%type<node> foreach
%type<node> tab_expr

%%

input:
		code {
			begin_code();
			produce_code($1);
			end_code();
			g_node_destroy($1);
		}
	;

code:
		{
			$$ = g_node_new((gpointer)TYPE_NULL);
		}
	|	code instruction {
			$$ = g_node_new((gpointer)TYPE_SEQUENCE);
			g_node_append($$, $1);
			g_node_append_data($$, (gpointer)(gulong)yylineno);
			g_node_append($$, $2);
		}
	;

instruction:
		affectation {
			$$ = $1;
		}
	|
		print {
			$$ = $1;
		}
	|
		read {
			$$ = $1;
		}
	|	if {
			$$ = $1;
		}
	|	while {
			$$ = $1;
		}
	|	foreach {
			$$ = $1;
		}
	;

if:		TOK_IF boolean_expr TOK_THEN code elseif else TOK_END{
			$$ = g_node_new((gpointer)TYPE_IF);
			g_node_append($$, $2);
			g_node_append($$, $4);
			g_node_append($$, $5);
			g_node_append($$, $6);
		}
	;
	
elseif:		{
			$$=g_node_new((gpointer)TYPE_NULL);
		}
	|	TOK_ELSEIF boolean_expr TOK_THEN code elseif {
			$$ = g_node_new((gpointer)TYPE_ELSEIF);
			g_node_append($$, $2);
			g_node_append($$, $4);
			g_node_append($$, $5);
		}
	;

else:		{
			$$=g_node_new((gpointer)TYPE_NULL);
		}
	|	TOK_ELSE code {
			$$ = g_node_new((gpointer)TYPE_ELSE);
			g_node_append($$, $2);
		}
	;
	
foreach :	TOK_FOREACH ident TOK_IN tab_expr TOK_DO code TOK_END{
			$$ = g_node_new((gpointer)TYPE_FOREACH);
			g_node_append($$, $2);
			g_node_append($$, $4);
			g_node_append($$, $6);
		}
	;
	
tab_expr : 	{
			$$=g_node_new((gpointer)TYPE_NULL);
		}
	|	TOK_PARO tab_expr TOK_PARF {
			$$ = $2;
		}
	|	expr tab_expr{
			$$ = g_node_new((gpointer)TYPE_TAB);
			g_node_append($$, $1);
			g_node_append($$, $2);
		}
	;
	
while :		TOK_WHILE boolean_expr TOK_DO code TOK_END {
			$$ = g_node_new((gpointer)TYPE_WHILE);
			g_node_append($$, $2);
			g_node_append($$, $4);
		}
	;
	
affectation:
		ident TOK_AFFECT expr TOK_SEMICOLON {
			$$ = g_node_new((gpointer)TYPE_AFFECTATION);
			g_node_append($$, $1);
			g_node_append($$, $3);
		}
	;

print:
		TOK_PRINT expr TOK_SEMICOLON {
			$$ = g_node_new((gpointer)TYPE_PRINT);
			g_node_append($$, $2);			
		}
	;
	
read:
		TOK_READ expr TOK_SEMICOLON {
			$$ = g_node_new((gpointer)TYPE_READ);
			g_node_append($$, $2);
		}
	;

expr:
		number {
			$$ = $1;
		}
	|	ident {
			$$ = $1;
		}
	|	TOK_PARO expr TOK_PARF {
			$$ = $2;
		}
	|	soustraction {
			$$ = $1;
		}
	|	multiplication {
			$$ = $1;
		}
	|	addition {
			$$ = $1;
		}
	|	division {
			$$ = $1;
		}
	|	modulo {
			$$ = $1;
		}
	;

number:
		TOK_NUMBER {
			$$ = g_node_new((gpointer)TYPE_NUMBER);
			g_node_append_data($$, (gpointer)$1);
		}

	;

ident:
		TOK_IDENT {
			$$ = g_node_new((gpointer)TYPE_IDENT);
			gulong value = (gulong) g_hash_table_lookup(table, $1);
			if (!value) {
				value = g_hash_table_size(table) + 1;
				g_hash_table_insert(table, strdup($1), (gpointer) value);
			}
			g_node_append_data($$, (gpointer)value);
		}
	;

addition: 	expr TOK_ADD expr {
			$$ = g_node_new((gpointer)TYPE_ADD);
			g_node_append($$, $1);
			g_node_append($$, $3);
		}
	;
	
soustraction: 	expr TOK_SUB expr {
			$$ = g_node_new((gpointer)TYPE_SUB);
			g_node_append($$, $1);
			g_node_append($$, $3);
		}
	;
	
multiplication:	expr TOK_MUL expr {
			$$ = g_node_new((gpointer)TYPE_MUL);
			g_node_append($$, $1);
			g_node_append($$, $3);
		}
	;
	
division: 	expr TOK_DIV expr {
			$$ = g_node_new((gpointer)TYPE_DIV);
			g_node_append($$, $1);
			g_node_append($$, $3);
		}
	;
	
modulo: 	expr TOK_MOD expr {
			$$ = g_node_new((gpointer)TYPE_MOD);
			g_node_append($$, $1);
			g_node_append($$, $3);
		}
	;
boolean_expr:	TOK_TRUE {
			$$ = g_node_new((gpointer)TYPE_TRUE);
		}
	|	TOK_FALSE {
			$$ = g_node_new((gpointer)TYPE_FALSE);
		}
	|	TOK_PARO boolean_expr TOK_PARF {
			$$ = $2;
		}
	|	superieur {
			$$ = $1;
		}
	|	inferieur {
			$$ = $1;
		}
	|	superieur_egal {
			$$ = $1;
		}
	|	inferieur_egal {
			$$ = $1;
		}
	|	egal {
			$$ = $1;
		}
	|	not {
			$$ = $1;
		}
	|	or {
			$$ = $1;
		}
	|	and {
			$$ = $1;
		}
	|	different {
			$$ = $1;
		}
	;
	
different:	expr TOK_DIFF expr {
			$$ = g_node_new((gpointer)TYPE_DIFF);
			g_node_append($$, $1);
			g_node_append($$, $3);
		}
	;
	
superieur:	expr TOK_SUP expr {
			$$ = g_node_new((gpointer)TYPE_SUP);
			g_node_append($$, $1);
			g_node_append($$, $3);
		}
	;
	
inferieur:	expr TOK_INF expr {
			$$ = g_node_new((gpointer)TYPE_INF);
			g_node_append($$, $1);
			g_node_append($$, $3);
		}
	;

superieur_egal:	expr TOK_SUP TOK_EGAL expr {
			$$ = g_node_new((gpointer)TYPE_SUP_EGAL);
			g_node_append($$, $1);
			g_node_append($$, $4);
		}
	;

inferieur_egal:	expr TOK_INF TOK_EGAL expr {
			$$ = g_node_new((gpointer)TYPE_INF_EGAL);
			g_node_append($$, $1);
			g_node_append($$, $4);
		}
	;

egal:		expr TOK_EGAL expr {
			$$ = g_node_new((gpointer)TYPE_EGAL);
			g_node_append($$, $1);
			g_node_append($$, $3);
		}
	;

not:		TOK_NOT boolean_expr {
			$$ = g_node_new((gpointer)TYPE_NOT);
			g_node_append($$, $2);
		}
	;

or:		boolean_expr TOK_OR boolean_expr {
			$$ = g_node_new((gpointer)TYPE_OR);
			g_node_append($$, $1);
			g_node_append($$, $3);
		}
	;
and:		boolean_expr TOK_AND boolean_expr {
			$$ = g_node_new((gpointer)TYPE_AND);
			g_node_append($$, $1);
			g_node_append($$, $3);
		}
	;
%%


