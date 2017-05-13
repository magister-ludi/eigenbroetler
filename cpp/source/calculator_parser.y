/* calculator */

%{
#include <calculator.h>

void yyerror(char const *)
{
}

%}

/* declare tokens */
%token TOK_EOL
%token <d> TOK_NUMBER
%token TOK_IF
%token TOK_RANDOM TOK_PI TOK_EXP TOK_SQRT TOK_ABS TOK_ARG
%token TOK_MOD TOK_REAL TOK_IMAG TOK_SINH TOK_COSH TOK_TANH
%token TOK_SIN TOK_COS TOK_TAN TOK_ASINH TOK_ACOSH TOK_ATAN2
%token TOK_ATANH TOK_ASIN TOK_ACOS TOK_ATAN TOK_LN TOK_LOG
%token TOK_ROUND TOK_TRUNC TOK_FRAC
%token TOK_PXL
%token  TOK_E TOK_I TOK_X TOK_Y TOK_R TOK_THETA TOK_S TOK_N

%nonassoc  TOK_EQ TOK_GT TOK_LE TOK_LT TOK_GE TOK_NE

%left '+' '-'
%left '*' '/'
%left TOK_UMINUS TOK_UPLUS
%left TOK_POW
%type <e> expr
%start calclist

%%

expr:
     TOK_IF '(' expr ',' expr ',' expr ')' { $$ = calculator.cmp($3, $5, $7); }
   | expr TOK_EQ expr                      { $$ = calculator.eq($1, $3); }
   | expr TOK_LT expr                      { $$ = calculator.lt($1, $3); }
   | expr TOK_GT expr                      { $$ = calculator.gt($1, $3); }
   | expr TOK_LE expr                      { $$ = calculator.le($1, $3); }
   | expr TOK_GE expr                      { $$ = calculator.ge($1, $3); }
   | expr '+' expr                         { $$ = calculator.add($1, $3); }
   | expr '-' expr                         { $$ = calculator.subtract($1, $3); }
   | expr '*' expr                         { $$ = calculator.multiply($1, $3); }
   | expr '/' expr                         { $$ = calculator.divide($1, $3); }
   | expr TOK_POW expr                     { $$ = calculator.pow_func($1, $3); }
   | TOK_PXL TOK_NUMBER '(' expr ',' expr ')'
                                           { $$ = calculator.pixel(int($2), 0, $4, $6); }
   | TOK_PXL TOK_NUMBER '(' expr ',' expr ',' expr ')'
                                           { $$ = calculator.pixel(int($2), $4, $6, $8); }
   | '(' expr ')'                          { $$ = $2; }
   | '-' expr %prec TOK_UMINUS             { $$ = calculator.neg($2); }
   | '+' expr %prec TOK_UPLUS              { $$ = $2; }
   | TOK_NUMBER                            { $$ = calculator.num($1); }
   | TOK_I                                 { $$ = calculator.var_i(); }
   | TOK_PI                                { $$ = calculator.var_pi(); }
   | TOK_E                                 { $$ = calculator.var_e(); }
   | TOK_X                                 { $$ = calculator.var_x(); }
   | TOK_Y                                 { $$ = calculator.var_y(); }
   | TOK_R                                 { $$ = calculator.var_r(); }
   | TOK_THETA                             { $$ = calculator.var_theta(); }
   | TOK_N                                 { $$ = calculator.var_n(); }
   | TOK_SQRT         '(' expr ')'         { $$ = calculator.sqrt_func($3); }
   | TOK_POW '(' expr ',' expr ')'         { $$ = calculator.pow_func($3, $5); }
   | TOK_RANDOM                            { $$ = calculator.rand_func(); }
   | TOK_EXP          '(' expr ')'         { $$ = calculator.exp_func($3); }
   | TOK_ABS          '(' expr ')'         { $$ = calculator.abs_func($3); }
   | TOK_ARG          '(' expr ')'         { $$ = calculator.arg_func($3); }
   | TOK_REAL         '(' expr ')'         { $$ = calculator.real_func($3); }
   | TOK_IMAG         '(' expr ')'         { $$ = calculator.imag_func($3); }
   | TOK_SINH         '(' expr ')'         { $$ = calculator.sinh_func($3); }
   | TOK_COSH         '(' expr ')'         { $$ = calculator.cosh_func($3); }
   | TOK_TANH         '(' expr ')'         { $$ = calculator.tanh_func($3); }
   | TOK_SIN          '(' expr ')'         { $$ = calculator.sin_func($3); }
   | TOK_COS          '(' expr ')'         { $$ = calculator.cos_func($3); }
   | TOK_TAN          '(' expr ')'         { $$ = calculator.tan_func($3); }
   | TOK_ASINH        '(' expr ')'         { $$ = calculator.asinh_func($3); }
   | TOK_ACOSH        '(' expr ')'         { $$ = calculator.acosh_func($3); }
   | TOK_ATAN2 '(' expr ',' expr ')'       { $$ = calculator.atan2_func($3, $5); }
   | TOK_ATANH        '(' expr ')'         { $$ = calculator.atanh_func($3); }
   | TOK_ASIN         '(' expr ')'         { $$ = calculator.asin_func($3); }
   | TOK_ACOS         '(' expr ')'         { $$ = calculator.acos_func($3); }
   | TOK_ATAN         '(' expr ')'         { $$ = calculator.atan_func($3); }
   | TOK_LN           '(' expr ')'         { $$ = calculator.ln_func($3); }
   | TOK_LOG          '(' expr ')'         { $$ = calculator.log_func($3); }
   | TOK_TRUNC        '(' expr ')'         { $$ = calculator.trunc_func($3); }
   | TOK_ROUND        '(' expr ')'         { $$ = calculator.round_func($3); }
   ;

calclist: /* nothing */
        | expr TOK_EOL  { calculator.setExpression($1); }
        ;
