// -*- c++ -*-
#ifndef CALCULATOR_INCLUDE
#define CALCULATOR_INCLUDE

#include <global_defs.h>

class Expression_base;
typedef Expression_base *Expression;

class Expression_base {
 public:
    static double const TOLERANCE;
    Expression_base(int len);
    virtual ~Expression_base();
    virtual Complex eval() const = 0;
 protected:
    Expression *expr;
    int n;
 private:
    Expression_base();
    Expression_base(Expression_base const&);
    Expression_base& operator=(Expression_base const&);
};

inline Expression_base::Expression_base(int len):
    expr(new Expression[len]),
    n(len)
{
}

inline Expression_base::~Expression_base()
{
    if (expr) {
        for (int i = 0; i < n; ++i)
            delete expr[i];
        delete [] expr;
    }
}

extern int yyparse();

union YYSTYPE
{
    Expression e;
    double d;
};

class Calculator {
 public:
    Calculator();
    ~Calculator();
    bool setFormula(QString const& formula);
    QString const& getFormula() const;
    Complex eval(double x, double y, int n);
    Expression cmp(Expression tst, Expression iftrue, Expression iffalse) const;
    Expression eq(Expression a1, Expression a2) const;
    Expression lt(Expression a1, Expression a2) const;
    Expression gt(Expression a1, Expression a2) const;
    Expression le(Expression a1, Expression a2) const;
    Expression ge(Expression a1, Expression a2) const;
    Expression add(Expression a1, Expression a2) const;
    Expression subtract(Expression a1, Expression a2) const;
    Expression multiply(Expression a1, Expression a2) const;
    Expression divide(Expression a1, Expression a2) const;
    Expression neg(Expression a) const;
    Expression num(double d) const;
    Expression var_i() const;
    Expression var_pi() const;
    Expression var_e() const;
    Expression var_x() const;
    Expression var_y() const;
    Expression var_r() const;
    Expression var_theta() const;
    Expression var_n() const;
    Expression sqrt_func(Expression a) const;
    Expression pow_func(Expression a1, Expression a2) const;
    Expression rand_func() const;
    Expression exp_func(Expression a) const;
    Expression arg_func(Expression a) const;
    Expression real_func(Expression a) const;
    Expression imag_func(Expression a) const;
    Expression sinh_func(Expression a) const;
    Expression cosh_func(Expression a) const;
    Expression tanh_func(Expression a) const;
    Expression sin_func(Expression a) const;
    Expression cos_func(Expression a) const;
    Expression tan_func(Expression a) const;
    Expression atan2_func(Expression a1, Expression a2) const;
    Expression asinh_func(Expression a) const;
    Expression acosh_func(Expression a) const;
    Expression atanh_func(Expression a) const;
    Expression asin_func(Expression a) const;
    Expression acos_func(Expression a) const;
    Expression atan_func(Expression a) const;
    Expression ln_func(Expression a) const;
    Expression log_func(Expression a) const;
 private:
    Calculator(Calculator const&);
    Calculator& operator=(Calculator const&);
    friend int yyparse();
    void setExpression(Expression formula);
    Expression expr;

    Complex x;
    Complex y;
    Complex r;
    Complex theta;
    Complex n;
    QString frm;
};

inline QString const& Calculator::getFormula() const
{
    return frm;
}

#endif /* CALCULATOR_INCLUDE */
