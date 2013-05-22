/* -*- c -*- */

%option noyywrap

%{

#include <string>
#include <calculator.h>
#include <parser.hpp>

#define YY_NEVER_INTERACTIVE 1

#define YY_SKIP_YYWRAP
#undef yywrap
#define yywrap(yyscanner) (1)
#ifdef YY_CHAR
#undef YY_CHAR
#define YY_CHAR char
#endif
%}

dreal    [0-9]*"."[0-9]+
ereal    [0-9]*("."[0-9]+)?[eE][+-]?[0-9]+
integer     [0-9]+
ws [ \t]+
nl \n
lt (\<)
gt >
le (\<=)
ge (>=)
ne (!=)|(\<>)
and (\&)|(and)
or  (\|)|(or)
not (!)|(not)
number {ereal}|{dreal}|{integer}

%%

{nl}        { calculator.update(); return TOK_EOL; }
{ws}        { calculator.update(); }
{number}    { yylval.d = atof(yytext); calculator.update(); return TOK_NUMBER; }

    /* functions */
rnd|rand      {calculator.update(); return TOK_RANDOM; }
pi            {calculator.update(); return TOK_PI; }
exp           {calculator.update(); return TOK_EXP; }
sqrt          {calculator.update(); return TOK_SQRT; }
abs           {calculator.update(); return TOK_ABS; }
arg           {calculator.update(); return TOK_ARG; }
mod           {calculator.update(); return TOK_MOD; }
real          {calculator.update(); return TOK_REAL; }
imag          {calculator.update(); return TOK_IMAG; }
sinh          {calculator.update(); return TOK_SINH; }
pow           {calculator.update(); return TOK_POW; }
\*\*          {calculator.update(); return TOK_POW; }
cosh          {calculator.update(); return TOK_COSH; }
tanh          {calculator.update(); return TOK_TANH; }
sin           {calculator.update(); return TOK_SIN; }
cos           {calculator.update(); return TOK_COS; }
tan           {calculator.update(); return TOK_TAN; }
asinh         {calculator.update(); return TOK_ASINH; }
acosh         {calculator.update(); return TOK_ACOSH; }
atan2         {calculator.update(); return TOK_ATAN2; }
atanh         {calculator.update(); return TOK_ATANH; }
asin          {calculator.update(); return TOK_ASIN; }
acos          {calculator.update(); return TOK_ACOS; }
atan          {calculator.update(); return TOK_ATAN; }
ln            {calculator.update(); return TOK_LN; }
log           {calculator.update(); return TOK_LOG; }
round         {calculator.update(); return TOK_ROUND; }
trunc|truncate   {calculator.update(); return TOK_TRUNC; }
frac|fraction    {calculator.update(); return TOK_FRAC; }
if            {calculator.update(); return TOK_IF; }
p             {calculator.update(); return TOK_PXL; }
         /* values */
e             {calculator.update(); return TOK_E; }
i             {calculator.update(); return TOK_I; }
x             {calculator.update(); return TOK_X; }
y             {calculator.update(); return TOK_Y; }
r             {calculator.update(); return TOK_R; }
theta|phi     {calculator.update(); return TOK_THETA; }
s             {calculator.update(); return TOK_S; }
n             {calculator.update(); return TOK_N; }
         /* comparisons and logic */
==            {calculator.update(); return TOK_EQ; }
{gt}          {calculator.update(); return TOK_GT; }
{le}          {calculator.update(); return TOK_LE; }
{lt}          {calculator.update(); return TOK_LT; }
{ge}          {calculator.update(); return TOK_GE; }
{ne}          {calculator.update(); return TOK_NE; }
         /*{and}         {calculator.update(); return TOK_AND; } */
         /*{or}          {calculator.update(); return TOK_OR; } */
         /*{not}         {calculator.update(); return TOK_NOT; } */
         /*p             {calculator.update(); return TOK_PIXEL; } */
         /*fx            {calculator.update(); return TOK_FOURIERX; } */
         /*fy            {calculator.update(); return TOK_FOURIERY; } */
         /*f             {calculator.update(); return TOK_FOURIER; } */
.             { calculator.update(); return yytext[0]; }

%%
// This just keeps the compiler  happy, because
// otherwise, yyunput() doesn't get called.
void never_called()
{
    char junk[2];
    yyunput(' ', junk);
}
