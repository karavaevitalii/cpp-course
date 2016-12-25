#ifndef BIND_H
#define BIND_H

#include <tuple>
#include <functional>

namespace bind
{
namespace placeholder
{

template<size_t N>
struct placeholder
{};

placeholder<0> _1;
placeholder<1> _2;
placeholder<2> _3;

}
}

namespace bind
{

template<typename F, typename ... Args>
struct bind_t
{
    template<typename ... Args1>
    constexpr auto operator()(Args1&& ... args1)
    {
        constexpr typename geter<sizeof ... (Args)>::next arguments;
        return call(arguments, std::forward<Args1>(args1) ...);
    }

private:
    typedef typename std::decay<F>::type func_t;
    typedef std::tuple<Args ...>         tuple_t;

    bind_t(F&& func, Args&& ... args)
        : func(std::forward<F>(func))
        , args(std::forward<Args>(args) ...)
    {}

    template<typename Arg, typename ... Args1>
    constexpr auto&& getarg(Arg& a, Args1& ...)
    {
        return a;
    }

    template<size_t N, typename ... Args1>
    constexpr auto&& getarg(placeholder::placeholder<N>&, Args1& ... args1)
    {
        return std::get<N>(std::forward_as_tuple(args1 ...));
    }

    template<typename B, typename ... Args1, typename ... Args2>
    constexpr auto getarg(bind_t<B, Args1 ...>& b, Args2& ... args2)
    {
        return b(args2 ...);
    }

    template<size_t ... N>
    struct arguments
    {};

    template<size_t N, size_t ... Seq>
    struct geter
    {
        typedef typename geter<N - 1, N - 1, Seq ...>::next next;
    };

    template<size_t ... Seq>
    struct geter<0, Seq ...>
    {
        typedef arguments<Seq ...> next;
    };

    template<size_t ... N, typename ... Args1>
    constexpr auto call(arguments<N ...> const&, Args1&& ... args1)
    {
        return func(getarg(std::get<N>(args), args1 ...) ...);
    }

    template<typename Fn, typename ... Args1>
    friend bind_t<Fn, Args1 ...> bind(Fn&& f, Args1&& ... args);

private:
    func_t func;
    tuple_t args;
};

template<typename F, typename ... Args>
bind_t<F, Args ...> bind(F&& f, Args&& ... args)
{
    return bind_t<F, Args ...>(std::forward<F>(f), std::forward<Args>(args) ...);
}

}

#endif
