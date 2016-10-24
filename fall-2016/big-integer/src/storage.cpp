#include "includes/storage.h"
void detach(std::shared_ptr<std::vector<size_t>>& pv)
{
    if (!pv.unique())
        pv = std::make_shared<std::vector<size_t>>(*pv);
}
bool storage::is_small() const
{
    return small < 2;
}
storage::storage(const size_t size, const size_t el)
{
    if (size == 0)
        small = 0;
    else if (size == 1)
    {
        small = 1;
        s_storage = el;
    }
    else
    {
        small = 2;
        d_storage = std::make_shared<std::vector<size_t>>(size, el);
    }
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
{}
storage& storage::operator=(storage const& that)
{
    if (this == &that)
        return *this;
    this->small = that.small;
    if (is_small())
        s_storage = that.s_storage;
    else
        d_storage = that.d_storage;
    return *this;
}
size_t& storage::operator[](size_t const index)
{
    if (is_small())
        return s_storage;
    detach(d_storage);
    return d_storage->at(index);
}
size_t const& storage::operator[](size_t const index) const
{
    if (is_small())
        return s_storage;
    return d_storage->at(index);
}
size_t& storage::back()
{
    if (is_small())
        return s_storage;
    detach(d_storage);
    return d_storage->back();
}
size_t const& storage::back() const
{
    if (is_small())
        return s_storage;
    return d_storage->back();
}
void storage::pop_back()
{
    if (is_small())
        small = 0;
    else
    {
        detach(d_storage);
        d_storage->pop_back();
        if (d_storage->size() == 1)
        {
            small = 1;
            s_storage = d_storage->back();
        }
    }
}
void storage::push_back(size_t const el)
{
    if (small == 0)
    {
        small = 1;
        s_storage = el;
    }
    else if (small == 1)
    {
        small = 2;
        d_storage = std::make_shared<std::vector<size_t>>();
        d_storage->push_back(s_storage);
        d_storage->push_back(el);
    }
    else
    {
        detach(d_storage);
        d_storage->push_back(el);
    }
}
void storage::resize(size_t const size, size_t const el)
{
    if (size == 0)
        small = 0;
    else if (size == 1)
    {
        small = 1;
        s_storage = el;
    }
    else if (is_small())
    {
        d_storage = std::make_shared<std::vector<size_t>>(size, el);
        small = 2;
    }
    else
    {
        detach(d_storage);
        d_storage->resize(size, el);
    }
}
void storage::insert_to_begin(size_t const amount, size_t const el)
{
    if (amount == 0)
        return;
    if (small == 0)
    {
        if (amount == 1)
        {
            small = 1;
            s_storage = el;
        }
        else
        {
            d_storage = std::make_shared<std::vector<size_t>>(amount, el);
            small = 2;
        }
    }
    else if (small == 1)
    {
        d_storage = std::make_shared<std::vector<size_t>>();
        d_storage->push_back(s_storage);
        d_storage->insert(d_storage->begin(), amount, el);
        small = 2;
    }
    else
    {
        detach(d_storage);
        d_storage->insert(d_storage->begin(), amount, el);
    }
}
void storage::erase_from_begin(size_t const del){
    if (!is_small())
    {
        detach(d_storage);
        d_storage->erase(d_storage->begin(), d_storage->begin() + del);
    }
    else if (small == 1 && del != 0)
    {
        small = 0;
    }
}
size_t storage::size() const
{
    if (is_small())
        return small;
    return d_storage->size();
}
bool storage::empty() const
{
    return small == 0;
}