// encoding: utf-8

#include <ruby.h>
#include <complex_array.h>

extern "C" void Init_eigenbroetler_rb(void);
VALUE clsComplex;
VALUE clsComplexArray;

template<typename T>
class Wrapper {
public:
    Wrapper();
    ~Wrapper();

    T *m;
private:
    Wrapper(Wrapper const&); // not implemented
    Wrapper& operator=(Wrapper const&); // not implemented
};

template<typename T>
Wrapper<T>::Wrapper():
    m(NULL)
{
}

template<typename T>
Wrapper<T>::~Wrapper()
{
    delete m;
}

void rb_complex_array_free(void *p)
{
    delete (Wrapper<ComplexArray> *) p;
}

VALUE rb_complex_array_alloc(VALUE klass)
{
    Wrapper<ComplexArray> *w = new Wrapper<ComplexArray>;
    VALUE obj = Data_Wrap_Struct(klass, NULL, rb_complex_array_free, w);
    return obj;
}

VALUE rb_complex_array_initialize(int argc, VALUE *argv, VALUE self)
{
    Wrapper<ComplexArray> *w;
    Data_Get_Struct(self, Wrapper<ComplexArray>, w);
    if (argc == 0) {
        w->m = new ComplexArray;
    }
    else if (argc == 1) {
        char const *fname = RSTRING_PTR(argv[0]);
        w->m = new ComplexArray(fname);
    }
    else if (argc == 2) {
        w->m = new ComplexArray(NUM2UINT(argv[0]), NUM2UINT(argv[1]));
    }
    return Qnil;
}

VALUE rb_complex_array_width(VALUE self)
{
    Wrapper<ComplexArray> *w;
    Data_Get_Struct(self, Wrapper<ComplexArray>, w);
    return INT2NUM(w->m->width());
}

VALUE rb_complex_array_height(VALUE self)
{
    Wrapper<ComplexArray> *w;
    Data_Get_Struct(self, Wrapper<ComplexArray>, w);
    return INT2NUM(w->m->height());
}

VALUE rb_complex_array_get(VALUE self, VALUE x, VALUE y)
{
    Wrapper<const ComplexArray> *w;
    Data_Get_Struct(self, Wrapper<const ComplexArray>, w);
    Complex c = w->m->value(NUM2INT(x), NUM2INT(y));
    return rb_funcall(rb_cObject, rb_intern("Complex"), 2, rb_float_new(c.real()), rb_float_new(c.imag()));
}

VALUE rb_complex_array_valid(VALUE self)
{
    Wrapper<const ComplexArray> *w;
    Data_Get_Struct(self, Wrapper<const ComplexArray>, w);
    return w->m->isValid() ? Qtrue : Qfalse;
}

VALUE rb_complex_array_err(VALUE self)
{
    Wrapper<const ComplexArray> *w;
    Data_Get_Struct(self, Wrapper<const ComplexArray>, w);
    return rb_str_new2(w->m->errorString().toAscii());
}

void Init_eigenbroetler_rb(void)
{
    clsComplexArray = rb_define_class("ComplexArray", rb_cObject);
    clsComplex = rb_define_class("Complex", rb_cNumeric);
    clsComplex = rb_define_class("Complex", rb_cNumeric);

    rb_define_alloc_func(clsComplexArray, rb_complex_array_alloc);
    rb_define_method(clsComplexArray, "initialize", RUBY_METHOD_FUNC(rb_complex_array_initialize), -1);
    rb_define_method(clsComplexArray, "width", RUBY_METHOD_FUNC(rb_complex_array_width), 0);
    rb_define_method(clsComplexArray, "height", RUBY_METHOD_FUNC(rb_complex_array_height), 0);

    rb_define_method(clsComplexArray, "valid?", RUBY_METHOD_FUNC(rb_complex_array_valid), 0);
    rb_define_method(clsComplexArray, "err", RUBY_METHOD_FUNC(rb_complex_array_err), 0);

    rb_define_method(clsComplexArray, "[]", RUBY_METHOD_FUNC(rb_complex_array_get), 2);
}
