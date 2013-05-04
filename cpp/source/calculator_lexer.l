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

QString dataString;

#undef YY_INPUT
#define YY_INPUT(buf, result, max_size) loadString(buf, result, max_size)

int readpos = 0, lastlen = 0;

void loadString(char *buf, yy_size_t& result, yy_size_t max_size)
{
    if ((readpos + lastlen) == 0) {
        strncpy((char *) buf, dataString.toAscii().data(), max_size);
        result = std::min(yy_size_t(dataString.length()), max_size);
    }
    else {
        result = 0;
    }
}

void update()
{
    readpos += lastlen;
    lastlen = strlen((const char *) yytext);
    //std::cout << "readpos = " << readpos << std::endl;
    //std::cout << "lastlen = " << lastlen << std::endl;
    //std::cout << "yytext = '" << (const char *) yytext << "'" << std::endl;
}

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

{nl}        { update(); return TOK_EOL; }
{ws}        { update(); }
{number}     { yylval.d = atof(yytext); update(); return TOK_NUMBER;}

    /* functions */
rnd|rand      {update(); return TOK_RANDOM;}
pi            {update(); return TOK_PI;}
exp           {update(); return TOK_EXP;}
sqrt          {update(); return TOK_SQRT; }
abs           {update(); return TOK_ABS; }
arg           {update(); return TOK_ARG; }
mod           {update(); return TOK_MOD; }
real          {update(); return TOK_REAL;}
imag          {update(); return TOK_IMAG;}
sinh          {update(); return TOK_SINH;}
pow           {update(); return TOK_POW;}
\*\*          {update(); return TOK_POW;}
cosh          {update(); return TOK_COSH;}
tanh          {update(); return TOK_TANH;}
sin           {update(); return TOK_SIN;}
cos           {update(); return TOK_COS;}
tan           {update(); return TOK_TAN;}
asinh         {update(); return TOK_ASINH;}
acosh         {update(); return TOK_ACOSH;}
atan2         {update(); return TOK_ATAN2;}
atanh         {update(); return TOK_ATANH;}
asin          {update(); return TOK_ASIN;}
acos          {update(); return TOK_ACOS;}
atan          {update(); return TOK_ATAN;}
ln            {update(); return TOK_LN;}
log           {update(); return TOK_LOG;}
round         {update(); return TOK_ROUND;}
trunc|truncate   {update(); return TOK_TRUNC;}
frac|fraction    {update(); return TOK_FRAC;}
if            {update(); return TOK_IF;}
         /* values */
e             {update(); return TOK_E;}
i             {update(); return TOK_I;}
x             {update(); return TOK_X;}
y             {update(); return TOK_Y;}
r             {update(); return TOK_R;}
theta|phi     {update(); return TOK_THETA;}
s             {update(); return TOK_S;}
n             {update(); return TOK_N;}
         /* comparisons and logic */
==            {update(); return TOK_EQ;}
{gt}          {update(); return TOK_GT;}
{le}          {update(); return TOK_LE;}
{lt}          {update(); return TOK_LT;}
{ge}          {update(); return TOK_GE;}
{ne}          {update(); return TOK_NE;}
         /*{and}         {update(); return TOK_AND;} */
         /*{or}          {update(); return TOK_OR;} */
         /*{not}         {update(); return TOK_NOT;} */
         /*p             {update(); return TOK_PIXEL;} */
         /*fx            {update(); return TOK_FOURIERX;} */
         /*fy            {update(); return TOK_FOURIERY;} */
         /*f             {update(); return TOK_FOURIER;} */
.             { update(); return yytext[0]; }

%%
// This just keeps the compiler  happy, because
// otherwise, yyunput() doesn't get called.
void never_called()
{
    char junk[2];
    yyunput(' ', junk);
}
