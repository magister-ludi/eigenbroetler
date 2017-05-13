#include <calculator.h>
#include <complex_array.h>

Complex const Constants::E = exp(1);
Complex const Constants::I(0, 1);
Complex const Constants::PI(4 * atan2(1, 1));

double const Expression_base::TOLERANCE = 1e-5;

Complex NSComplex::asin(Complex const& z)
{
    Complex c = sqrt(Complex(1, 0) - z * z) + Constants::I * z;
    return Complex(arg(c), -::log(abs(c)));
}

Complex NSComplex::acos(Complex const& z)
{
    Complex c = sqrt(Complex(1, 0) - z * z) + Constants::I * z;

    return Complex((0.5 * Constants::PI - arg(c)).real(), ::log(abs(c)));
}

Complex NSComplex::atan(Complex const& z)
{
    double x, y, yp1, ym1;

    x = z.real();
    y = z.imag();
    if (y == 0)
        return(::atan(x));
    yp1 = y + 1;
    ym1 = y - 1;
    return Complex((::atan2(x, 1 - y) - ::atan2(-x, 1 + y)) / double(2),
                   (::log((yp1 * yp1 + x * x) / (ym1 * ym1 + x * x))) / double(4)
                   );
}

Complex NSComplex::atan2(Complex const& x, Complex const& y)
{
    if (fabs(x.imag()) < Expression_base::TOLERANCE &&
        fabs(y.imag()) < Expression_base::TOLERANCE)
        return ::atan2(x.real(), y.real());
    return atan(x / y);
}

using namespace NSComplex;

template<int N>
class Expression_class: public Expression_base {
public:
    Expression_class();
};

template<int N>
Expression_class<N>::Expression_class():
    Expression_base(N)
{
}

class sqrt_class: public Expression_class<1> {
public: sqrt_class(Expression a);
    Complex eval() const;
private: sqrt_class();
    sqrt_class(sqrt_class const &);
    sqrt_class & operator=(sqrt_class const &);
};

sqrt_class::sqrt_class(Expression a)
{
    expr[0] = a;
}

Complex sqrt_class::eval() const
{
    return sqrt(expr[0]->eval());
}

Expression Calculator::sqrt_func(Expression a) const
{
    return new sqrt_class(a);
};

class pow_class: public Expression_class<2> {
public: pow_class(Expression a1, Expression a2);
    Complex eval() const;
private: pow_class();
    pow_class(pow_class const &);
    pow_class & operator=(pow_class const &);
};

pow_class::pow_class(Expression a1, Expression a2)
{
    expr[0] = a1;
    expr[1] = a2;
}

Complex pow_class::eval() const
{
    return pow(expr[0]->eval(), expr[1]->eval());
}

Expression Calculator::pow_func(Expression a1, Expression a2) const
{
    return new pow_class(a1, a2);
};

class rand_class: public Expression_class<0> {
public: rand_class();
    Complex eval() const;
private: rand_class(rand_class const &);
    rand_class & operator=(rand_class const &);
};

rand_class::rand_class()
{
}

Complex rand_class::eval() const
{
    return double(rand()) / RAND_MAX;
}

Expression Calculator::rand_func() const
{
    return new rand_class();
};

class exp_class: public Expression_class<1> {
public: exp_class(Expression a);
    Complex eval() const;
private: exp_class();
    exp_class(exp_class const &);
    exp_class & operator=(exp_class const &);
};

exp_class::exp_class(Expression a)
{
    expr[0] = a;
}

Complex exp_class::eval() const
{
    return exp(expr[0]->eval());
}

Expression Calculator::exp_func(Expression a) const
{
    return new exp_class(a);
};

class abs_class: public Expression_class<1> {
public: abs_class(Expression a);
    Complex eval() const;
private: abs_class();
    abs_class(abs_class const &);
    abs_class & operator=(abs_class const &);
};

abs_class::abs_class(Expression a)
{
    expr[0] = a;
}

Complex abs_class::eval() const
{
    return abs(expr[0]->eval());
}

Expression Calculator::abs_func(Expression a) const
{
    return new abs_class(a);
};

class arg_class: public Expression_class<1> {
public: arg_class(Expression a);
    Complex eval() const;
private: arg_class();
    arg_class(arg_class const &);
    arg_class & operator=(arg_class const &);
};

arg_class::arg_class(Expression a)
{
    expr[0] = a;
}

Complex arg_class::eval() const
{
    return arg(expr[0]->eval());
}

Expression Calculator::arg_func(Expression a) const
{
    return new arg_class(a);
};

class real_class: public Expression_class<1> {
public: real_class(Expression a);
    Complex eval() const;
private: real_class();
    real_class(real_class const &);
    real_class & operator=(real_class const &);
};

real_class::real_class(Expression a)
{
    expr[0] = a;
}

Complex real_class::eval() const
{
    return real(expr[0]->eval());
}

Expression Calculator::real_func(Expression a) const
{
    return new real_class(a);
};

class imag_class: public Expression_class<1> {
public: imag_class(Expression a);
    Complex eval() const;
private: imag_class();
    imag_class(imag_class const &);
    imag_class & operator=(imag_class const &);
};

imag_class::imag_class(Expression a)
{
    expr[0] = a;
}

Complex imag_class::eval() const
{
    return imag(expr[0]->eval());
}

Expression Calculator::imag_func(Expression a) const
{
    return new imag_class(a);
};

class sinh_class: public Expression_class<1> {
public: sinh_class(Expression a);
    Complex eval() const;
private: sinh_class();
    sinh_class(sinh_class const &);
    sinh_class & operator=(sinh_class const &);
};

sinh_class::sinh_class(Expression a)
{
    expr[0] = a;
}

Complex sinh_class::eval() const
{
    return sinh(expr[0]->eval());
}

Expression Calculator::sinh_func(Expression a) const
{
    return new sinh_class(a);
};

class cosh_class: public Expression_class<1> {
public: cosh_class(Expression a);
    Complex eval() const;
private: cosh_class();
    cosh_class(cosh_class const &);
    cosh_class & operator=(cosh_class const &);
};

cosh_class::cosh_class(Expression a)
{
    expr[0] = a;
}

Complex cosh_class::eval() const
{
    return cosh(expr[0]->eval());
}

Expression Calculator::cosh_func(Expression a) const
{
    return new cosh_class(a);
};

class tanh_class: public Expression_class<1> {
public: tanh_class(Expression a);
    Complex eval() const;
private: tanh_class();
    tanh_class(tanh_class const &);
    tanh_class & operator=(tanh_class const &);
};

tanh_class::tanh_class(Expression a)
{
    expr[0] = a;
}

Complex tanh_class::eval() const
{
    return tanh(expr[0]->eval());
}

Expression Calculator::tanh_func(Expression a) const
{
    return new tanh_class(a);
};

class sin_class: public Expression_class<1> {
public: sin_class(Expression a);
    Complex eval() const;
private: sin_class();
    sin_class(sin_class const &);
    sin_class & operator=(sin_class const &);
};

sin_class::sin_class(Expression a)
{
    expr[0] = a;
}

Complex sin_class::eval() const
{
    return sin(expr[0]->eval());
}

Expression Calculator::sin_func(Expression a) const
{
    return new sin_class(a);
};

class cos_class: public Expression_class<1> {
public: cos_class(Expression a);
    Complex eval() const;
private: cos_class();
    cos_class(cos_class const &);
    cos_class & operator=(cos_class const &);
};

cos_class::cos_class(Expression a)
{
    expr[0] = a;
}

Complex cos_class::eval() const
{
    return cos(expr[0]->eval());
}

Expression Calculator::cos_func(Expression a) const
{
    return new cos_class(a);
};

class tan_class: public Expression_class<1> {
public: tan_class(Expression a);
    Complex eval() const;
private: tan_class();
    tan_class(tan_class const &);
    tan_class & operator=(tan_class const &);
};

tan_class::tan_class(Expression a)
{
    expr[0] = a;
}

Complex tan_class::eval() const
{
    return tan(expr[0]->eval());
}

Expression Calculator::tan_func(Expression a) const
{
    return new tan_class(a);
};

class asinh_class: public Expression_class<1> {
public: asinh_class(Expression a);
    Complex eval() const;
private: asinh_class();
    asinh_class(asinh_class const &);
    asinh_class & operator=(asinh_class const &);
};

asinh_class::asinh_class(Expression a)
{
    expr[0] = a;
}

Complex asinh_class::eval() const
{
    return -Constants::I * asin(Constants::I * expr[0]->eval());
}

Expression Calculator::asinh_func(Expression a) const
{
    return new asinh_class(a);
};

class acosh_class: public Expression_class<1> {
public: acosh_class(Expression a);
    Complex eval() const;
private: acosh_class();
    acosh_class(acosh_class const &);
    acosh_class & operator=(acosh_class const &);
};

acosh_class::acosh_class(Expression a)
{
    expr[0] = a;
}

Complex acosh_class::eval() const
{
    return Constants::I * acos(expr[0]->eval());
}

Expression Calculator::acosh_func(Expression a) const
{
    return new acosh_class(a);
};

class atan2_class: public Expression_class<2> {
public: atan2_class(Expression a1, Expression a2);
    Complex eval() const;
private: atan2_class();
    atan2_class(atan2_class const &);
    atan2_class & operator=(atan2_class const &);
};

atan2_class::atan2_class(Expression a1, Expression a2)
{
    expr[0] = a1;
    expr[1] = a2;
}

Complex atan2_class::eval() const
{
    return atan2(expr[0]->eval(), expr[1]->eval());
}

Expression Calculator::atan2_func(Expression a1, Expression a2) const
{
    return new atan2_class(a1, a2);
};

class atanh_class: public Expression_class<1> {
public: atanh_class(Expression a);
    Complex eval() const;
private: atanh_class();
    atanh_class(atanh_class const &);
    atanh_class & operator=(atanh_class const &);
};

atanh_class::atanh_class(Expression a)
{
    expr[0] = a;
}

Complex atanh_class::eval() const
{
    return -Constants::I * atan(Constants::I * expr[0]->eval());
}

Expression Calculator::atanh_func(Expression a) const
{
    return new atanh_class(a);
};

class asin_class: public Expression_class<1> {
public: asin_class(Expression a);
    Complex eval() const;
private: asin_class();
    asin_class(asin_class const &);
    asin_class & operator=(asin_class const &);
};

asin_class::asin_class(Expression a)
{
    expr[0] = a;
}

Complex asin_class::eval() const
{
    return asin(expr[0]->eval());
}

Expression Calculator::asin_func(Expression a) const
{
    return new asin_class(a);
};

class acos_class: public Expression_class<1> {
public: acos_class(Expression a);
    Complex eval() const;
private: acos_class();
    acos_class(acos_class const &);
    acos_class & operator=(acos_class const &);
};

acos_class::acos_class(Expression a)
{
    expr[0] = a;
}

Complex acos_class::eval() const
{
    return acos(expr[0]->eval());
}

Expression Calculator::acos_func(Expression a) const
{
    return new acos_class(a);
};

class atan_class: public Expression_class<1> {
public: atan_class(Expression a);
    Complex eval() const;
private: atan_class();
    atan_class(atan_class const &);
    atan_class & operator=(atan_class const &);
};

atan_class::atan_class(Expression a)
{
    expr[0] = a;
}

Complex atan_class::eval() const
{
    return atan(expr[0]->eval());
}

Expression Calculator::atan_func(Expression a) const
{
    return new atan_class(a);
};

class ln_class: public Expression_class<1> {
public: ln_class(Expression a);
    Complex eval() const;
private: ln_class();
    ln_class(ln_class const &);
    ln_class & operator=(ln_class const &);
};

ln_class::ln_class(Expression a)
{
    expr[0] = a;
}

Complex ln_class::eval() const
{
    return log(expr[0]->eval());
}

Expression Calculator::ln_func(Expression a) const
{
    return new ln_class(a);
};

class log_class: public Expression_class<1> {
public: log_class(Expression a);
    Complex eval() const;
private: log_class();
    log_class(log_class const &);
    log_class & operator=(log_class const &);
};

log_class::log_class(Expression a)
{
    expr[0] = a;
}

Complex log_class::eval() const
{
    return log10(expr[0]->eval());
}

Expression Calculator::log_func(Expression a) const
{
    return new log_class(a);
};

class reference_class: public Expression_class<0> {
public:
    reference_class(Complex const& ref);
    Complex eval() const;
private:
    reference_class();
    reference_class(reference_class const &);
    reference_class & operator=(reference_class const &);
    Complex const& val;
};

reference_class::reference_class(Complex const& ref):
    val(ref)
{
}

Complex reference_class::eval() const
{
    return val;
}

class number_class: public Expression_class<0> {
public:
    number_class(double const& d);
    Complex eval() const;
private:
    number_class();
    number_class(number_class const &);
    number_class & operator=(number_class const &);
    Complex val;
};

number_class::number_class(double const& d):
    val(d)
{
}

Complex number_class::eval() const
{
    return val;
}

class cmp_class: public Expression_class<3> {
public:
    cmp_class(Expression a1, Expression a2, Expression a3);
    Complex eval() const;
private:
    cmp_class();
    cmp_class(cmp_class const &);
    cmp_class & operator=(cmp_class const &);
};

cmp_class::cmp_class(Expression a1, Expression a2, Expression a3)
{
    expr[0] = a1;
    expr[1] = a2;
    expr[2] = a3;
}

Complex cmp_class::eval() const
{
    if (expr[0]->eval().real() == 0)
        return expr[1]->eval();
    else
        return expr[2]->eval();
}

class eq_class: public Expression_class<2> {
public:
    eq_class(Expression a1, Expression a2);
    Complex eval() const;
private:
    eq_class();
    eq_class(eq_class const &);
    eq_class & operator=(eq_class const &);
};

eq_class::eq_class(Expression a1, Expression a2)
{
    expr[0] = a1;
    expr[1] = a2;
}

Complex eq_class::eval() const
{
    if (std::abs(expr[0]->eval() - expr[1]->eval()) < TOLERANCE)
        return 0;
    else
        return 1;
}

class lt_class: public Expression_class<2> {
public:
    lt_class(Expression a1, Expression a2);
    Complex eval() const;
private:
    lt_class();
    lt_class(lt_class const &);
    lt_class & operator=(lt_class const &);
};

class gt_class: public Expression_class<2> {
public:
    gt_class(Expression a1, Expression a2);
    Complex eval() const;
private:
    gt_class();
    gt_class(gt_class const &);
    gt_class & operator=(gt_class const &);
};

class le_class: public Expression_class<2> {
public:
    le_class(Expression a1, Expression a2);
    Complex eval() const;
private:
    le_class();
    le_class(le_class const &);
    le_class & operator=(le_class const &);
};

class ge_class: public Expression_class<2> {
public:
    ge_class(Expression a1, Expression a2);
    Complex eval() const;
private:
    ge_class();
    ge_class(ge_class const &);
    ge_class & operator=(ge_class const &);
};

class add_class: public Expression_class<2> {
public:
    add_class(Expression a1, Expression a2);
    Complex eval() const;
private:
    add_class();
    add_class(add_class const &);
    add_class & operator=(add_class const &);
};

class subtract_class: public Expression_class<2> {
public:
    subtract_class(Expression a1, Expression a2);
    Complex eval() const;
private:
    subtract_class();
    subtract_class(subtract_class const &);
    subtract_class & operator=(subtract_class const &);
};

class multiply_class: public Expression_class<2> {
public:
    multiply_class(Expression a1, Expression a2);
    Complex eval() const;
private:
    multiply_class();
    multiply_class(multiply_class const &);
    multiply_class & operator=(multiply_class const &);
};

class divide_class: public Expression_class<2> {
public:
    divide_class(Expression a1, Expression a2);
    Complex eval() const;
private:
    divide_class();
    divide_class(divide_class const &);
    divide_class & operator=(divide_class const &);
};

class neg_class: public Expression_class<1> {
public:
    neg_class(Expression a);
    Complex eval() const;
private:
    neg_class();
    neg_class(neg_class const &);
    neg_class & operator=(neg_class const &);
};

le_class::le_class(Expression a1, Expression a2)
{
    expr[0] = a1;
    expr[1] = a2;
}

Complex le_class::eval() const
{
    if (expr[0]->eval().real() <= expr[1]->eval().real())
        return 0;
    else
        return 1;
}

lt_class::lt_class(Expression a1, Expression a2)
{
    expr[0] = a1;
    expr[1] = a2;
}

Complex lt_class::eval() const
{
    if (expr[0]->eval().real() < expr[1]->eval().real())
        return 0;
    else
        return 1;
}

ge_class::ge_class(Expression a1, Expression a2)
{
    expr[0] = a1;
    expr[1] = a2;
}

Complex ge_class::eval() const
{
    if (expr[0]->eval().real() >= expr[1]->eval().real())
        return 0;
    else
        return 1;
}

gt_class::gt_class(Expression a1, Expression a2)
{
    expr[0] = a1;
    expr[1] = a2;
}

Complex gt_class::eval() const
{
    if (expr[0]->eval().real() > expr[1]->eval().real())
        return 0;
    else
        return 1;
}

neg_class::neg_class(Expression a)
{
    expr[0] = a;
}

Complex neg_class::eval() const
{
    return -expr[0]->eval();
}

add_class::add_class(Expression a1, Expression a2)
{
    expr[0] = a1;
    expr[1] = a2;
}

Complex add_class::eval() const
{
    return expr[0]->eval() + expr[1]->eval();
}

subtract_class::subtract_class(Expression a1, Expression a2)
{
    expr[0] = a1;
    expr[1] = a2;
}

Complex subtract_class::eval() const
{
    return expr[0]->eval() - expr[1]->eval();
}

multiply_class::multiply_class(Expression a1, Expression a2)
{
    expr[0] = a1;
    expr[1] = a2;
}

Complex multiply_class::eval() const
{
    return expr[0]->eval() * expr[1]->eval();
}

divide_class::divide_class(Expression a1, Expression a2)
{
    expr[0] = a1;
    expr[1] = a2;
}

Complex divide_class::eval() const
{
    return expr[0]->eval() / expr[1]->eval();
}

class trunc_class: public Expression_class<1> {
public:
    trunc_class(Expression a);
    Complex eval() const;
private: trunc_class();
    trunc_class(trunc_class const &);
    trunc_class & operator=(trunc_class const &);
};

trunc_class::trunc_class(Expression a)
{
    expr[0] = a;
}

Complex trunc_class::eval() const
{
    return int(expr[0]->eval().real());
}

Expression Calculator::trunc_func(Expression a) const
{
    return new trunc_class(a);
};

class round_class: public Expression_class<1> {
public:
    round_class(Expression a);
    Complex eval() const;
private: round_class();
    round_class(round_class const &);
    round_class & operator=(round_class const &);
};

round_class::round_class(Expression a)
{
    expr[0] = a;
}

Complex round_class::eval() const
{
    return round(expr[0]->eval().real());
}

Expression Calculator::round_func(Expression a) const
{
    return new round_class(a);
};

Calculator::Calculator():
    expr(NULL)
{
    //extern int yydebug;
    //yydebug = 1;
}

Calculator::~Calculator()
{
    setExpression(NULL);
}

Expression Calculator::cmp(Expression tst, Expression iftrue, Expression iffalse) const
{
    return new cmp_class(tst, iftrue, iffalse);
}

Expression Calculator::eq(Expression a1, Expression a2) const
{
    return new eq_class(a1, a2);
}

Expression Calculator::num(double d) const
{
    return new number_class(d);
}

Expression Calculator::lt(Expression a1, Expression a2) const
{
    return new lt_class(a1, a2);
}

Expression Calculator::gt(Expression a1, Expression a2) const
{
    return new gt_class(a1, a2);
}

Expression Calculator::le(Expression a1, Expression a2) const
{
    return new le_class(a1, a2);
}

Expression Calculator::ge(Expression a1, Expression a2) const
{
    return new ge_class(a1, a2);
}

Expression Calculator::add(Expression a1, Expression a2) const
{
    return new add_class(a1, a2);
}

Expression Calculator::subtract(Expression a1, Expression a2) const
{
    return new subtract_class(a1, a2);
}

Expression Calculator::multiply(Expression a1, Expression a2) const
{
    return new multiply_class(a1, a2);
}

Expression Calculator::divide(Expression a1, Expression a2) const
{
    return new divide_class(a1, a2);
}

Expression Calculator::neg(Expression a) const
{
    return new neg_class(a);
}

Expression Calculator::var_x() const
{
    return new reference_class(x);
}

Expression Calculator::var_y() const
{
    return new reference_class(y);
}

Expression Calculator::var_r() const
{
    return new reference_class(r);
}

Expression Calculator::var_theta() const
{
    return new reference_class(theta);
}

Expression Calculator::var_n() const
{
    // Is this good? I don't know.
    Calculator *non_const = const_cast<Calculator *>(this);
    non_const->counter_used = true;
    return new reference_class(n);
}

Expression Calculator::var_e() const
{
    return new reference_class(Constants::E);
}

Expression Calculator::var_i() const
{
    return new reference_class(Constants::I);
}

Expression Calculator::var_pi() const
{
    return new reference_class(Constants::PI);
}

Calculator::ImageData::ImageData():
    owned(false)
{
}

Calculator::ImageData::~ImageData()
{
    for (QList<ComplexArray const *>::iterator i = c.begin(); owned && i != c.end(); ++i)
        delete *i;
}

class pixel_class: public Expression_class<3> {
public:
    pixel_class(Calculator::ImageData *data, Expression layer, Expression a1, Expression a2);
    Complex eval() const;
private:
    pixel_class();
    pixel_class(pixel_class const &);
    pixel_class & operator=(pixel_class const &);
    Calculator::ImageData *d;
};

pixel_class::pixel_class(Calculator::ImageData *data, Expression layer, Expression a1, Expression a2):
d(data)
{
    expr[0] = layer;
    expr[1] = a1;
    expr[2] = a2;
}

Complex pixel_class::eval() const
{
    ComplexArray const *a = d->c[d->curr];
    if (expr[0]) {
        int layer = int(expr[0]->eval().real()) - 1;
        int nlayers = d->c.length();
        if (layer < 0)
            layer += nlayers * (layer / nlayers + 1);
        layer %= nlayers;
        a = d->c[layer];
    }
    int xx = int(expr[1]->eval().real()) + a->width() / 2;
    int yy = int(expr[2]->eval().real()) + a->height() / 2;
    if (xx < 0 || xx >= a->width()) {
        if (d->ax == Calculator::ImageData::TRUNCATE)
            return 0;
        else {
            if (xx < 0)
                xx += a->width() * (xx / a->width() + 1);
            xx %= a->width();
        }
    }
    if (yy < 0 || yy >= a->height()) {
        if (d->ay == Calculator::ImageData::TRUNCATE)
            return 0;
        else {
            if (yy < 0)
                yy += a->height() * (yy / a->height() + 1);
            yy %= a->height();
        }
    }
    return a->value(xx, yy);
}

Expression Calculator::pixel(int img, Expression xpos, Expression ypos) const
{
    // Is this good? I don't know.
    Calculator *non_const = const_cast<Calculator *>(this);
    ImageMap::iterator key = non_const->image_data.find(img);
    if (key == non_const->image_data.end())
        non_const->image_data[img] = new ImageData;
    return new pixel_class(non_const->image_data[img], NULL, xpos, ypos);
}

Expression Calculator::pixel(int img, Expression layer, Expression xpos, Expression ypos) const
{
    // Is this good? I don't know.
    Calculator *non_const = const_cast<Calculator *>(this);
    ImageMap::iterator key = non_const->image_data.find(img);
    if (key == non_const->image_data.end())
        non_const->image_data[img] = new ImageData;
    return new pixel_class(non_const->image_data[img], layer, xpos, ypos);
}

struct yy_buffer_state;
typedef yy_buffer_state *YY_BUFFER_STATE;
void yy_switch_to_buffer (YY_BUFFER_STATE new_buffer);
YY_BUFFER_STATE yy_scan_string(char const *);
void yy_delete_buffer(YY_BUFFER_STATE b);

bool Calculator::setFormula(QString const& formula)
{
    QString dataString = formula + "\n";
    YY_BUFFER_STATE b = yy_scan_string(dataString.toAscii().data());
    yy_switch_to_buffer(b);
    readpos = lastlen = 0;
    frm.clear();
    bool result;
    setExpression(NULL);
    counter_used = false;
    if (yyparse()) {
        setExpression(NULL);
        result = false;
    }
    else {
        frm = formula;
        result = true;
    }
    yy_delete_buffer(b);
    return result;
}

void Calculator::setExpression(Expression formula)
{
    if (expr) {
        delete expr;
        ImageMap::iterator id;
        for (id = image_data.begin(); id != image_data.end(); ++id)
            delete id.value();
        image_data.clear();
    }
    expr = formula;
}

Complex Calculator::eval(double xx, double yy, int nn)
{
    if (!expr) {
        std::cerr << "No formula has been set!" << std::endl;
        exit(EXIT_FAILURE);
    }
    x = xx;
    y = yy;
    n = nn;
    r = sqrt(x * x + y * y);
    theta = atan2(y, x);
    return expr->eval();
}

void Calculator::update()
{
    extern char *yytext;
    readpos += lastlen;
    lastlen = strlen((const char *) yytext);
}
