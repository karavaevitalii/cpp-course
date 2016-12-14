#ifndef BIND_H
#define BIND_H

#include <tuple>
#include <functional>

template<typename F, typename ... Args>
struct binder
{
    auto operator()()
    {
        return call(func, args);
    }

private:
    typename std::decay<F>::type func;
    std::tuple<Args ...> args;

    binder(F const& func, Args&& ... args)
        : func(func)
        , args(std::forward<Args>(args) ...)
    {}

    template<typename Fn, typename ... T>
    friend binder<Fn, T ...> bind(Fn const& f, T&& ... t);

    template<typename Arg>
    auto getarg(Arg const&& arg) const
    {
        return arg;
    }

    template<typename FF, typename ... Args1, typename ... Args2>
    auto getarg(binder<FF, Args1 ...> const&& b, Args2&& ... args) const
    {
        return b(std::forward<Args2>(args) ...);
    }

    template<typename FF, typename T, bool Pred, size_t A, size_t ... N>
    struct caller
    {
        static auto call(FF&& f, T&& t)
        {
            return caller<FF, T
                    , sizeof ... (N) + 1 == A
                    , A, N ..., sizeof ... (N)>
                    ::call(f, std::forward<T>(t));
        }
    };

    template<typename FF, typename T, size_t A, size_t... N>
    struct caller<FF, T, true, A, N ...>
    {
        static auto call(FF&& f, T&& t)
        {
            return f(getarg(std::get<N>(std::forward<T>(t)) ...));
        }
    };

    template<typename FF, typename T>
    auto call(FF&& f, T&& t) const
    {
        typedef typename std::decay<T>::type type_t;

        return caller<FF, T
                , std::tuple_size<type_t>::value == 0
                , std::tuple_size<type_t>::value>
                ::call(f, std::forward<T>(t));
    }
};

template<typename F, typename ... Args>
binder<F, Args ...> bind(F const& f, Args&& ... args)
{
    return binder<F, Args ...>(f, std::forward<Args>(args) ...);
}

#endif
