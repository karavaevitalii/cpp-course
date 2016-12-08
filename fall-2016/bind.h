#ifndef BIND_H
#define BIND_H

#include <tuple>

template<typename F, typename ... Args>
struct binder
{
    binder(F func, Args ... args)
        : func(std::forward<F>(func))
        , args(std::forward<Args>(args) ...)
    {}

    template<typename ... Args1, typename ... Args2>
    auto operator()(Args2 ... args2)
    {
        return call(func, std::make_tuple<Args2 ...>(std::forward<Args2>(args2) ...));
    }

private:
    F func;
    std::tuple<Args ...> args;

    template<typename FF, typename T, bool Pred, size_t A, size_t ... N>
    struct caller
    {
        static auto call(FF f, T&& t)
        {
            return caller<FF, T
                    , A == 1 + sizeof ... (N)
                    , A, N ..., sizeof ... (N)>
                    ::call(f, std::forward<T>(t));
        }
    };

    template<typename FF, typename T, size_t A, size_t... N>
    struct caller<FF, T, true, A, N ...>
    {
        static auto call(FF f, T&& t)
        {
            return f(std::get<N>(std::forward<T>(t)) ...);
        }
    };

    template<typename FF, typename T>
    auto call(FF f, T&& t)
    {
        typedef typename std::decay<T>::type type_t;

        return caller<FF, T
                , 0 == std::tuple_size<type_t>::value
                , std::tuple_size<type_t>::value>
                ::call(f, std::forward<T>(t));
    }
};

template<typename F, typename ... Args>
auto bind(F f, Args ... args)
{
    return(binder<F, Args ...>(f, args ...));
}

#endif
