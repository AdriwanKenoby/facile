/* definition */
%{

#include "facile.h"

%}

%option yylineno


%%

0|[1-9][[:digit:]]* {
			sscanf(yytext, "%ld", &yylval.number);
			return TOK_NUMBER;
}

"in"			return TOK_IN;

"foreach"		return TOK_FOREACH;

"do"			return TOK_DO;

"while"			return TOK_WHILE;

"!"			return TOK_NOT;

"end"|"endif"|"endwhile"|"endforeach"	return TOK_END;

"then"			return TOK_THEN;

"else"			return TOK_ELSE;

"elseif"		return TOK_ELSEIF;

"if"			return TOK_IF;

"and"			return TOK_AND;

"or"			return TOK_OR;

"#"			return TOK_DIFF;

"false"			return TOK_FALSE;

"true"			return TOK_TRUE;

"<"			return TOK_INF;

">"			return TOK_SUP;

"("			return TOK_PARO;

")"			return TOK_PARF;

":="			return TOK_AFFECT;

"="			return TOK_EGAL;

"+"			return TOK_ADD;

"-"			return TOK_SUB;

"*"			return TOK_MUL;

"/"			return TOK_DIV;

"%"			return TOK_MOD;

";"			return TOK_SEMICOLON;

"print"			return TOK_PRINT;

"read"			return TOK_READ;

[[:alpha:]][[:alnum:]]* {
			yylval.string = yytext;
			return TOK_IDENT;
}

" "|"\t"|"\n" {
}

.			return yytext[0];

%%
int yyerror(char *msg)
{
	fprintf(stderr,"%d: %s at %s\n",yylineno,msg,yytext);
}
