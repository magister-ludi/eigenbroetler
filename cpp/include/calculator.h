// -*- c++ -*-
#ifndef CALCULATOR_INCLUDE
#define CALCULATOR_INCLUDE

#include <global_defs.h>
#include <QMap>

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

union YYSTYPE
{
    Expression e;
    double d;
};
typedef size_t yy_size_t;
#include <parser.hpp>

extern int yyparse();

class ComplexArray;

class Calculator {
public:
    class ImageData {
    public:
        ImageData();
        ~ImageData();
        QList<ComplexArray const *> c;
        int curr;
        bool owned;
        enum adjust {
            TRUNCATE,
            WRAP
        };
        adjust ax;
        adjust ay;
    private:
        ImageData(ImageData const&);
        ImageData& operator=(ImageData const&);
    };
    Calculator();
    ~Calculator();
    bool setFormula(QString const& formula);
    typedef QMap<int, ImageData*> ImageMap;
    ImageMap& getImageData();
    QString const& getFormula() const;
    Complex eval(double x, double y, int n);
    int errorPos() const;
    bool counterUsed() const;
    Expression cmp(Expression tst, Expression iftrue, Expression iffalse) const;
    Expression pixel(int img, Expression xpos, Expression ypos) const;
    Expression pixel(int img, Expression layer, Expression xpos, Expression ypos) const;
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
    Expression trunc_func(Expression a) const;
    Expression round_func(Expression a) const;
private:
    Calculator(Calculator const&);
    Calculator& operator=(Calculator const&);
    friend int yylex();
    friend int yyparse();
    void setExpression(Expression formula);
    void update();
    Expression expr;

    Complex x;
    Complex y;
    Complex r;
    Complex theta;
    Complex n;
    QString frm;
    int readpos;
    int lastlen;
    bool counter_used;
    ImageMap image_data;
};

inline QString const& Calculator::getFormula() const
{
    return frm;
}

inline int Calculator::errorPos() const
{
    if (expr)
        return -1; // no error
    else
        return readpos;
}

inline bool Calculator::counterUsed() const
{
    return counter_used;
}

inline Calculator::ImageMap& Calculator::getImageData()
{
    return image_data;
}

extern Calculator calculator;
extern int yylex();

#endif /* CALCULATOR_INCLUDE */
