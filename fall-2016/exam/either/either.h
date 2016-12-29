#ifndef EITER_H
#define EITER_H

#include <memory>
#include <type_traits>

struct emplace_left_t
{};
static constexpr emplace_left_t emplace_left;

struct emplace_right_t
{};
static constexpr emplace_right_t emplace_right;

template<typename Left, typename Right>
struct either
{
    either(Left value)
        : which(1)
    {
        new (&data) Left(std::move(value));
    }

    either(Right value)
        : which(2)
    {
        new (&data) Right(std::move(value));
    }

    either(either const& that)
        : which(that.which)
    {
        if (which == 1 || which == 3)
            new (&data) Left(*that.left_data_ptr());
        else
            new (&data) Right(*that.right_data_ptr());
    }

    either(either&& that)
        : which(that.which)
    {
        if (is_left())
            new (&data) Left(std::move(*that.left_data_ptr()));
        else
            new (&data) Right(std::move(*that.right_data_ptr()));
    }

    template<typename ... Args>
    either(emplace_left_t, Args&& ... args)
        : which(1)
    {
        new (&data) Left(std::forward<Args>(args) ...);
    }

    template<typename ... Args>
    either(emplace_right_t, Args&& ... args)
        : which(2)
    {
        new (&data) Right(std::forward<Args>(args) ...);
    }

    either& operator=(either that)
    {
        if (that.is_left())
            emplace(emplace_left, that.left());
        else
            emplace(emplace_right, that.right());

        return *this;
    }

    template<typename ... Args>
    void emplace(emplace_left_t, Args&& ... args)
    {
        if (is_left())
            emplace_impl<Left>(left(), 1, std::forward<Args>(args) ...);
        else
            emplace_impl<Left>(right(), 1, std::forward<Args>(args) ...);
    }

    template<typename ... Args>
    void emplace(emplace_right_t, Args&& ... args)
    {
        if (is_left())
            emplace_impl<Right>(left(), 2, std::forward<Args>(args) ...);
        else
            emplace_impl<Right>(right(), 2, std::forward<Args>(args) ...);
    }

    bool is_left() const
    {
        return (which == 1 || which == 3);
    }

    Left& left()
    {
        return *left_data_ptr();
    }

    Left const& left() const
    {
        return *left_data_ptr();
    }

    bool is_right() const
    {
        return !is_left();
    }

    Right& right()
    {
        return *right_data_ptr();
    }

    Right const& right() const
    {
        return *right_data_ptr();
    }

    ~either()
    {
        clear();
    }

    either() = delete;

private:
    void clear()
    {
        if (which == 1)
            reinterpret_cast<Left*>(&data)->Left::~Left();
        else if (which == 2)
            reinterpret_cast<Right*>(&data)->Right::~Right();
        else if (which == 3)
            reinterpret_cast<std::unique_ptr<Left>*>(&data)->~unique_ptr();
        else
            reinterpret_cast<std::unique_ptr<Right>*>(&data)->~unique_ptr();
    }

    template<typename T0, typename T1, typename ... Args>
    void emplace_impl(T1 old, int t2, Args&& ... args)
    {
        auto tmp = std::make_unique<T1>(old);
        try
        {
            clear();
            new (&data) T0(std::forward<Args>(args) ...);
            which = t2;
        } catch (...)
        {
            if (which == 1)
                which = 3;
            else
                which = 4;

            new (&data) std::unique_ptr<T1>(tmp.release());
            throw;
        }
    }

private:
    Left* left_data_ptr()
    {
        if (which == 1)
            return reinterpret_cast<Left*>(&data);
        return reinterpret_cast<std::unique_ptr<Left>*>(&data)->get();
    }

    Left const* left_data_ptr() const
    {
        if (which == 1)
            return reinterpret_cast<Left const*>(&data);
        return reinterpret_cast<std::unique_ptr<Left> const*>(&data)->get();
    }

    Right* right_data_ptr()
    {
        if (which == 2)
            return reinterpret_cast<Right*>(&data);
        return reinterpret_cast<std::unique_ptr<Right>*>(&data)->get();
    }

    Right const* right_data_ptr() const
    {
        if (which == 2)
            return reinterpret_cast<Right const*>(&data);
        return reinterpret_cast<std::unique_ptr<Right> const*>(&data)->get();
    }

    template<typename L, typename R, typename LL, typename RR>
    friend void swap_impl(either<L, R>& a, either<L, R>& b, std::unique_ptr<LL> l, std::unique_ptr<RR> r, int lt, int rt);

private:
    int which; //1 - Left, 2 - Right, 3 - Left*, 4 - Right*
    typename std::aligned_storage<std::max(std::max(sizeof(Left), sizeof(Right)), std::max(sizeof(std::unique_ptr<Left>), sizeof(std::unique_ptr<Right>)))
    , std::max(std::max(alignof(Left), alignof(Right)), std::max(sizeof(std::unique_ptr<Left>), sizeof(std::unique_ptr<Right>)))>::type data;
};

template<typename F, typename Left, typename Right>
auto apply(F const& func, either<Left, Right> const& e)
{
    if (e.is_left())
        return func(e.left());
    else
        return func(e.right());
}

template<typename F, typename Left, typename Right>
auto apply(F const& func, either<Left, Right>& e)
{
    if (e.is_left())
        return func(e.left());
    else
        return func(e.right());
}

template<typename Left, typename Right, typename L, typename R>
void swap_impl(either<Left, Right>& a, either<Left, Right>& b, std::unique_ptr<L> l, std::unique_ptr<R> r, int lt, int rt)
{
    try
    {
        a.clear();
        a.which = rt;
        new (&a.data) R(*r);
        b.clear();
        b.which = lt;
        new (&b.data) L(*l);
    }
    catch (...)
    {
        if (a.which == 1)
            a.which = 3;
        else
            a.which = 4;

        if (b.which == 1)
            b.which = 3;
        else
            b.which = 4;

        new (&a.data) std::unique_ptr<L>(l.release());
        new (&b.data) std::unique_ptr<R>(r.release());
        throw;
    }
}

template<typename Left, typename Right>
void swap(either<Left, Right>& a, either<Left, Right>& b)
{
    if (a.is_left() && b.is_left())
        swap_impl(a, b, std::make_unique<Left>(a.left()), std::make_unique<Left>(b.left()), 1, 1);
    else if (a.is_left() && b.is_right())
        swap_impl(a, b, std::make_unique<Left>(a.left()), std::make_unique<Right>(b.right()), 1, 2);
    else if (a.is_right() && b.is_left())
        swap_impl(a, b, std::make_unique<Right>(a.right()), std::make_unique<Left>(b.left()), 2, 1);
    else
        swap_impl(a, b, std::make_unique<Right>(a.right()), std::make_unique<Right>(b.right()), 2, 2);
}

#endif //EITER_H
