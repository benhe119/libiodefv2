/*****
*
* Copyright (C) 2003-2016 CS-SI. All Rights Reserved.
* Author: Krzysztof Zaraska
* Author: Nicolas Delon
*
* This file is part of the LibIodef library.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*****/

%top{
#include "libmissing.h"
}

%{
#include <stdlib.h>
#include <string.h>

#include "iodefv2.h"
#include "common.h"
#include "iodefv2-criteria-string.yac.h"

#define YY_NO_INPUT
#define YY_NO_TOP_STATE

#ifndef MIN
# define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

static void do_pop(void);

static unsigned int inp = 0;


static char *escape_str(char *str)
{
        size_t w = 0, i = 0;
        int escape_next = FALSE;

        for ( i = 0; str[i]; i++ ) {

                if ( ! escape_next && str[i] == '\\' ) {
                        escape_next = TRUE;
                        continue;
                }

                str[w++] = str[i];
                escape_next = FALSE;
        }

        str[w] = 0;
        return str;
}


%}

IODEFV2_PATH      ([a-zA-Z0-9_\-]+(\(((\-?[0-9\*]+)|(\"[^"]+\")|(\'[^']+\'))\))?\.?)+
SQSTRING        \'([^\\\']|\\.)*\'
DQSTRING        \"([^\\\"]|\\.)*\"
BLANK           [ \t\n]+

%option nounput
%option noyywrap
%option stack

%x IODEFV2_VALUE

%%

substr                  { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_SUBSTRING; }
\!substr                { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_NOT_SUBSTRING; }
substr\*                { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_SUBSTRING_NOCASE; }
\!substr\*              { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_NOT_SUBSTRING_NOCASE; }

{IODEFV2_PATH}            {
                                yylval.str = strdup(yytext);
                                return TOK_IODEFV2_PATH;
                        }

\<\>                    { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_SUBSTRING; }
\<\>\*                  { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_SUBSTRING_NOCASE; }
\!\<\>                  { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_NOT_SUBSTRING; }
\!\<\>\*                { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_NOT_SUBSTRING_NOCASE; }

\>                      { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_GREATER; }
\>\=                    { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_GREATER_OR_EQUAL; }
\<                      { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_LESS; }
\<\=                    { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_LESS_OR_EQUAL; }

\~                      { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_REGEXP; }
\~\*                    { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_REGEXP_NOCASE; }
\!\~                    { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_NOT_REGEXP; }
\!\~\*                  { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_NOT_REGEXP_NOCASE; }

\=\=                    { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_EQUAL; }
\=                      { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_EQUAL; }
\=\*                    { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_EQUAL_NOCASE; }
\!\=                    { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_NOT_EQUAL; }
\!\=\*                  { yy_push_state(IODEFV2_VALUE); return TOK_RELATION_NOT_EQUAL_NOCASE; }
\!                      { return TOK_NOT; }

\&\&                    { return TOK_OPERATOR_AND; }
\&                      { return TOK_OPERATOR_AND; }
\|\|                    { return TOK_OPERATOR_OR; }
\|                      { return TOK_OPERATOR_OR; }

\(                      { return '('; }
\)                      { return ')'; }

<IODEFV2_VALUE>{SQSTRING} {
                                yylval.str = escape_str(strndup(yytext + 1, yyleng - 2));

                                if ( ! inp )
                                        yy_pop_state();

                                return TOK_IODEFV2_VALUE;
                        }

<IODEFV2_VALUE>{DQSTRING} {
                                yylval.str = escape_str(strndup(yytext + 1, yyleng - 2));

                                if ( ! inp )
                                        yy_pop_state();

                                return TOK_IODEFV2_VALUE;
                        }

<IODEFV2_VALUE>[^ \t\(\)\&\|]+ {
                                yylval.str = escape_str(strdup(yytext));

                                if ( ! inp )
                                        yy_pop_state();

                                return TOK_IODEFV2_VALUE;
                        }

<IODEFV2_VALUE>{BLANK}    {
                                /* nop */;
                        }

<IODEFV2_VALUE>\(         {
                                inp++;
                                return '(';
                        }

<IODEFV2_VALUE>\)         {
                                do_pop();
                                return ')';
                        }

<IODEFV2_VALUE>\&\&       { return TOK_OPERATOR_AND; }
<IODEFV2_VALUE>\&         { return TOK_OPERATOR_AND; }
<IODEFV2_VALUE>\|\|       { return TOK_OPERATOR_OR; }
<IODEFV2_VALUE>\|         { return TOK_OPERATOR_OR; }

<IODEFV2_VALUE>.          {
                                /* invalid token */
                                return TOK_ERROR;
                        }

{BLANK}                 {
                                /* nop */;
                        }

.                       {
                                /* invalid token */
                                yylval.str = strdup(yytext);
                                return TOK_ERROR;
                        }



%%

static void do_pop(void)
{
        if ( --inp == 0 )
                yy_pop_state();
}

void _iodefv2_criteria_string_init_lexer(void);

void _iodefv2_criteria_string_init_lexer(void)
{
        inp = 0;
        BEGIN INITIAL;
}
