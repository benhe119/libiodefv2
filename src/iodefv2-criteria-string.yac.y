/*****
*
* Copyright (C) 2003-2016 CS-SI. All Rights Reserved.
* Author: Krzysztof Zaraska <kzaraska@student.uci.agh.edu.pl>
* Author: Nicolas Delon <nicolas.delon@libiodefv2-ids.com>
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

%{
#include "libmissing.h"
%}

%{
#define LIBIODEFV2_ERROR_SOURCE_DEFAULT LIBIODEFV2_ERROR_SOURCE_IODEFV2_CRITERIA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdarg.h>
#include <ctype.h>

#include "glthread/lock.h"

#include "libiodefv2-log.h"
#include "libiodefv2-error.h"
#include "libiodefv2-inttypes.h"

#include "iodefv2.h"
#include "iodefv2-criteria.h"
#include "common.h"

static int path_count = 0;
static int real_ret = 0;
static iodefv2_path_t *cur_path;
static iodefv2_criteria_t *processed_criteria;
static iodefv2_criterion_operator_t cur_operator;

gl_lock_t _criteria_parse_mutex = gl_lock_initializer;


#define operator_or 1
#define operator_and 2

extern int yylex(void);
extern void yylex_init(void);
extern void yylex_destroy(void);
static void yyerror(char *s);
extern void *yy_scan_string(const char *);
extern void yy_delete_buffer(void *);
void _iodefv2_criteria_string_init_lexer(void);

#define YYERROR_VERBOSE


static int create_criteria(iodefv2_criteria_t **criteria, iodefv2_path_t *path,
                           iodefv2_criterion_value_t *value, iodefv2_criterion_operator_t operator)
{
        iodefv2_criterion_t *criterion;

        real_ret = iodefv2_criteria_new(criteria);
        if ( real_ret < 0 )
                goto err;

        if ( path_count++ > 0 )
                iodefv2_path_ref(path);

        real_ret = iodefv2_criterion_new(&criterion, path, value, operator);
        if ( real_ret < 0 ) {
                iodefv2_criteria_destroy(*criteria);
                goto err;
        }

        iodefv2_criteria_set_criterion(*criteria, criterion);
        return 0;

err:
        iodefv2_path_destroy(path);
        cur_path = NULL;

        return real_ret;
}


%}

%union {
        char *str;
        int operator;
        iodefv2_path_t *path;
        iodefv2_criteria_t *criteria;
        iodefv2_criterion_operator_t relation;
}

/* BISON Declarations */

%token <str> TOK_IODEFV2_VALUE "<IODEFV2-Value>"
%token <str> TOK_IODEFV2_PATH "<IODEFV2-Path>"

%destructor { free($$); } TOK_IODEFV2_VALUE TOK_IODEFV2_PATH
%destructor { iodefv2_criteria_destroy($$); } criteria

%token TOK_RELATION_SUBSTRING "<>"
%token TOK_RELATION_SUBSTRING_NOCASE "<>*"
%token TOK_RELATION_NOT_SUBSTRING "!<>"
%token TOK_RELATION_NOT_SUBSTRING_NOCASE "!<>*"

%token TOK_RELATION_REGEXP "~"
%token TOK_RELATION_REGEXP_NOCASE "~*"
%token TOK_RELATION_NOT_REGEXP "!~"
%token TOK_RELATION_NOT_REGEXP_NOCASE "!~*"

%token TOK_RELATION_GREATER ">"
%token TOK_RELATION_GREATER_OR_EQUAL ">="
%token TOK_RELATION_LESS "<"
%token TOK_RELATION_LESS_OR_EQUAL "<="
%token TOK_RELATION_EQUAL "="
%token TOK_RELATION_EQUAL_NOCASE "=*"
%token TOK_RELATION_NOT_EQUAL "!="
%token TOK_RELATION_NOT_EQUAL_NOCASE "!=*"

%token TOK_NOT "!"
%token TOK_OPERATOR_AND "&&"
%token TOK_OPERATOR_OR "||"

%token TOK_ERROR

%type <criteria> criteria
%type <criteria> criteria_base
%type <criteria> value
%type <criteria> multiple_value
%type <criteria> criterion
%type <path> path
%type <relation> relation
%type <operator> operator


/* Grammar follows */
%%

input:
        criteria {
                processed_criteria = $1;
        }
;


criteria:
        criteria_base {
                $$ = $1;
        }

        | criteria operator criteria_base {
                if ( $2 == operator_or )
                        iodefv2_criteria_or_criteria($1, $3);
                else
                        iodefv2_criteria_and_criteria($1, $3);

                $$ = $1;
        }
;

criteria_base:
        criterion {
                $$ = $1;
        }

        | '(' criteria ')' {
                $$ = $2;
        }

        | TOK_NOT '(' criteria ')' {
                iodefv2_criteria_set_negation($3, TRUE);
                $$ = $3;
        }
;


criterion:
        path relation '(' multiple_value ')' {
                $$ = $4;
        }

        | path relation value {
                $$ = $3;
        }

        | path {
                iodefv2_criteria_t *criteria;

                real_ret = create_criteria(&criteria, $1, NULL, IODEFV2_CRITERION_OPERATOR_NOT|IODEFV2_CRITERION_OPERATOR_NULL);
                if ( real_ret < 0 )
                        YYABORT;

                $$ = criteria;
        }

        | TOK_NOT path {
                iodefv2_criteria_t *criteria;

                real_ret = create_criteria(&criteria, $2, NULL, IODEFV2_CRITERION_OPERATOR_NULL);
                if ( real_ret < 0 )
                        YYABORT;

                $$ = criteria;
        }
;


path:
        TOK_IODEFV2_PATH {
                real_ret = iodefv2_path_new_fast(&cur_path, $1);
                free($1);

                if ( real_ret < 0 )
                        YYABORT;

                path_count = 0;
                $$ = cur_path;
        }
;


value:
        TOK_IODEFV2_VALUE {
                iodefv2_criteria_t *criteria;
                iodefv2_criterion_value_t *value = NULL;

                real_ret = iodefv2_criterion_value_new_from_string(&value, cur_path, $1, cur_operator);
                free($1);

                if ( real_ret < 0 )
                        YYABORT;

                real_ret = create_criteria(&criteria, cur_path, value, cur_operator);
                if ( real_ret < 0 )
                        YYABORT;

                $$ = criteria;
        }
;



multiple_value:
        multiple_value operator multiple_value {
                if ( $2 == operator_or )
                        iodefv2_criteria_or_criteria($1, $3);
                else
                        iodefv2_criteria_and_criteria($1, $3);

                $$ = $1;
        }

        | '(' multiple_value ')' {
                $$ = $2;
        }

        | value {
                $$ = $1;
        }
;


relation:
  TOK_RELATION_SUBSTRING            { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_SUBSTR; }
| TOK_RELATION_SUBSTRING_NOCASE     { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_SUBSTR|IODEFV2_CRITERION_OPERATOR_NOCASE; }
| TOK_RELATION_NOT_SUBSTRING        { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_SUBSTR|IODEFV2_CRITERION_OPERATOR_NOT; }
| TOK_RELATION_NOT_SUBSTRING_NOCASE { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_SUBSTR|IODEFV2_CRITERION_OPERATOR_NOT|IODEFV2_CRITERION_OPERATOR_NOCASE; }
| TOK_RELATION_REGEXP               { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_REGEX; }
| TOK_RELATION_REGEXP_NOCASE        { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_REGEX|IODEFV2_CRITERION_OPERATOR_NOCASE; }
| TOK_RELATION_NOT_REGEXP           { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_REGEX|IODEFV2_CRITERION_OPERATOR_NOT; }
| TOK_RELATION_NOT_REGEXP_NOCASE    { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_REGEX|IODEFV2_CRITERION_OPERATOR_NOT|IODEFV2_CRITERION_OPERATOR_NOCASE; }
| TOK_RELATION_GREATER              { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_GREATER; }
| TOK_RELATION_GREATER_OR_EQUAL     { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_GREATER|IODEFV2_CRITERION_OPERATOR_EQUAL; }
| TOK_RELATION_LESS                 { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_LESSER; }
| TOK_RELATION_LESS_OR_EQUAL        { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_LESSER|IODEFV2_CRITERION_OPERATOR_EQUAL; }
| TOK_RELATION_EQUAL                { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_EQUAL; }
| TOK_RELATION_EQUAL_NOCASE         { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_EQUAL|IODEFV2_CRITERION_OPERATOR_NOCASE; }
| TOK_RELATION_NOT_EQUAL            { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_EQUAL|IODEFV2_CRITERION_OPERATOR_NOT; }
| TOK_RELATION_NOT_EQUAL_NOCASE     { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_EQUAL|IODEFV2_CRITERION_OPERATOR_NOCASE|IODEFV2_CRITERION_OPERATOR_NOT; }
| TOK_NOT                           { cur_operator = $$ = IODEFV2_CRITERION_OPERATOR_NULL; }
| TOK_ERROR                         { real_ret = libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_CRITERIA_PARSE,
                                                                       "Criteria parser reported: Invalid operator found"); YYERROR; }
;

operator:       TOK_OPERATOR_AND        { $$ = operator_and; }
                | TOK_OPERATOR_OR       { $$ = operator_or; }
;

%%

static void yyerror(char *s)  /* Called by yyparse on error */
{
        real_ret = libiodefv2_error_verbose_make(LIBIODEFV2_ERROR_SOURCE_IODEFV2_CRITERIA,
                                              LIBIODEFV2_ERROR_IODEFV2_CRITERIA_PARSE,
                                              "IODEFV2-Criteria parser: %s", s);
}


int iodefv2_criteria_new_from_string(iodefv2_criteria_t **new_criteria, const char *str)
{
        int ret;
        void *state;

        libiodefv2_return_val_if_fail(str, -1);

        gl_lock_lock(_criteria_parse_mutex);

        real_ret = 0;
        processed_criteria = NULL;

        state = yy_scan_string(str);
        ret = yyparse();
        yy_delete_buffer(state);

        if ( ret != 0 ) {
                _iodefv2_criteria_string_init_lexer();

                if ( real_ret )
                        ret = real_ret;
                else
                        ret = libiodefv2_error_make(LIBIODEFV2_ERROR_SOURCE_IODEFV2_CRITERIA, LIBIODEFV2_ERROR_IODEFV2_CRITERIA_PARSE);

                if ( processed_criteria )
                        iodefv2_criteria_destroy(processed_criteria);
        }

        else *new_criteria = processed_criteria;

        gl_lock_unlock(_criteria_parse_mutex);

        return ret;
}
