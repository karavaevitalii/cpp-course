#include "includes/storage.h"
storage::shared_vector::shared_vector(size_t const size, size_t const element)
    : data_(size, element)
    , links(1)
{}
bool storage::is_small() const
{
    return small < 2;
}
uint8_t storage::init_small(size_t const size)
{
    if (size == 0)
        return 0;
    else if (size == 1)
        return 1;
    else
        return 2;
}
void storage::detach()
{
    if (d_storage->links > 1)
    {
        shared_vector* s_vector = new shared_vector(*d_storage);
        --d_storage->links;
        std::swap(s_vector, d_storage);
    }
}
void storage::safe_delete()
{
    if (!is_small())
    {
        if (d_storage->links > 1)
            --d_storage->links;
        else
            delete d_storage;
    }
}
storage::storage(size_t const size, size_t const element)
    : small(init_small(size))
{
    if (is_small())
        s_storage = element;
    else
        d_storage = new shared_vector(size, element);
}
storage::storage(storage const& that)
    : small(that.small)
{
    if (is_small())
        s_storage = that.s_storage;
    else
        d_storage = that.d_storage;
}
storage::~storage()
{
    safe_delete();
}
storage& storage::operator=(storage const& that)
{
    if (!is_small())
        safe_delete();
    this->small = that.small;
    if (is_small())
        this->s_storage = that.s_storage;
    else
    {
        ++that.d_storage->links;
        this->d_storage = that.d_storage;
    }
    return *this;

}
void storage::resize(size_t const size, size_t const element)
{
    if (is_small())
        if (size < 2)
        {
            s_storage = element;
            small = 1;
        }
        else
        {
            d_storage = new shared_vector(size, element);
            small = 2;
        }
    else
    {
        detach();
        d_storage->data_.resize(size, element);
    }
}
size_t storage::size() const
{
    if (is_small())
        return static_cast<size_t>(small);
    return d_storage->data_.size();
}
size_t const& storage::operator[](size_t const index) const
{
    if (is_small())
        return s_storage;
    return d_storage->data_.at(index);
}
size_t& storage::operator[](size_t const index)
{
    if (is_small())
        return s_storage;
    detach();
    return d_storage->data_.at(index);
}
size_t const& storage::back() const
{
    if (is_small())
        return s_storage;
    return d_storage->data_.back();
}
size_t& storage::back()
{
    if (is_small())
        return s_storage;
    detach();
    return d_storage->data_.back();
}
void storage::push_back(size_t const element)
{
    if (is_small())
    {
        if (small == 0)
        {
            s_storage = element;
            small = 1;
        }
        else
        {
            size_t old = s_storage;
            d_storage = new shared_vector(1, old);
            d_storage->data_.push_back(element);
            small = 2;
        }
    }
    else
    {
        detach();
        d_storage->data_.push_back(element);
    }
}
void storage::pop_back()
{
    if (!is_small())
    {
        detach();
        d_storage->data_.pop_back();
    }
    else
        small = 0;
}
bool storage::empty() const
{
    return small == 0;
}
void storage::erase(size_t const first, size_t const last)
{
    if (small == 1)
        small = 0;
    else if (!is_small())
    {
        detach();
        d_storage->data_.erase(d_storage->data_.begin() + first, d_storage->data_.begin() + first + last);
    }
}
void storage::insert(size_t const first, size_t const amount, size_t const element)
{
    if (small == 0 && amount == 1)
    {
        small = 1;
        s_storage = element;
    }
    else if (small == 1)
    {
        size_t old = s_storage;
        d_storage = new shared_vector(1, old);
        d_storage->data_.insert(d_storage->data_.begin() + first, amount, element);
        small = 2;
    }
    else
    {
        detach();
        d_storage->data_.insert(d_storage->data_.begin() + first, amount, element);
    }
}