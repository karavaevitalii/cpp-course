#include "includes/storage.h"
storage::shared_vector::shared_vector(size_t size, size_t element)
    : data_(new std::vector<size_t>(size, element))
{}
storage::shared_vector::shared_vector(storage::shared_vector const& that)
    : data_(that.data_)
{}
void storage::shared_vector::detach()
{
    if (!data_.unique())
        this->data_ = std::make_shared<std::vector<size_t>>(*this->data_);
}
void storage::safe_delete()
{
    if (size_ > 1)
        delete d_storage;
}
storage::storage(size_t size, size_t element)
{
    if (size > 1)
        size_ = 2;
    else if (size == 1)
        size_ = 1;
    else
        size_ = 0;
    if (size_ < 2)
        s_storage = element;
    else
        d_storage = new shared_vector(size, element);
}
storage::storage(storage const& that)
    : size_(that.size_)
{
    if (this->size_ < 2)
        this->s_storage = that.s_storage;
    else
        this->d_storage = new shared_vector(*that.d_storage);
}
storage::~storage()
{
    safe_delete();
}
storage& storage::operator=(storage const& that)
{
    if (this == &that)
        return *this;
    safe_delete();
    this->size_ = that.size_;
    if (size_ < 2)
        s_storage = that.s_storage;
    else
        d_storage = new shared_vector(*that.d_storage);
    return *this;
}
void storage::resize(size_t size, size_t element)
{
    if (size == 0)
    {
        s_storage = 0;
        size_ = 0;
    }
    if (size == 1)
    {
        s_storage = element;
        size_ = 1;
    }
    else if (size_ < 2)
    {
        d_storage = new shared_vector(size, element);
        size_ = 2;
    }
    else
    {
        d_storage->detach();
        d_storage->data_.get()->resize(size, element);
    }

}
size_t storage::size() const
{
    if (size_ == 0)
        return 0;
    if (size_ == 1)
        return 1;
    return d_storage->data_.get()->size();
}
size_t const& storage::operator[](size_t index) const
{
    if (size_ < 2)
        return s_storage;
    return d_storage->data_.get()->at(index);
}
size_t& storage::operator[](size_t index)
{
    if (size_ < 2)
        return s_storage;
    d_storage->detach();
    return d_storage->data_.get()->at(index);
}
size_t const& storage::back() const
{
    if (size_ < 2)
        return s_storage;
    return d_storage->data_.get()->back();
}
size_t& storage::back()
{
    if (size_ < 2)
        return s_storage;
    d_storage->detach();
    return d_storage->data_.get()->back();
}
void storage::push_back(size_t element)
{
    if (size_ == 0)
    {
        s_storage = element;
        size_ = 1;
    }
    else if (size_ == 1)
    {
        size_t old = s_storage;
        d_storage = new shared_vector(1, old);
        d_storage->data_.get()->push_back(element);
        size_ = 2;
    }
    else
    {
        d_storage->detach();
        d_storage->data_.get()->push_back(element);
    }
}
void storage::pop_back()
{
    if (size_ == 0)
        return;
    if (size_ == 1)
    {
        s_storage = 0;
        size_ = 0;
    }
    else
    {
        d_storage->detach();
        d_storage->data_.get()->pop_back();
        if (d_storage->data_.get()->size() == 1)
        {
            size_t old = d_storage->data_.get()->back();
            s_storage = old;
            size_ = 1;
        }
    }
}
bool storage::empty() const
{
    return size_ == 0;
}
void storage::erase(size_t const begin, size_t const end)
{
    if (size_ > 1)
    {
        d_storage->detach();
        d_storage->data_.get()->erase(d_storage->data_.get()->begin() + begin, d_storage->data_.get()->begin() + begin + end);
        if (d_storage->data_.get()->size() == 1)
        {
            size_t old = d_storage->data_.get()->back();
            s_storage = old;
            size_ = 1;
        }
    }
    else if (size_ == 1 && end != 0)
    {
        s_storage = 0;
        size_ = 0;
    }
}
void storage::insert(size_t const begin, size_t const size, size_t const element)
{
    if (size == 0)
        return;
    if (size_ == 0)
    {
        if (size == 1)
        {
            s_storage = element;
            size_ = 1;
        }
        else
        {
            d_storage = new shared_vector(size, element);
            size_ = 2;
        }
    }
    else if (size_ == 1)
    {
        size_t old = s_storage;
        d_storage = new shared_vector(1, old);
        d_storage->data_.get()->insert(d_storage->data_.get()->begin() + begin, size, element);
        size_ = 2;
    }
    else
    {
        d_storage->detach();
        d_storage->data_.get()->insert(d_storage->data_.get()->begin() + begin, size, element);
    }
}
bool operator==(storage const& left, storage const& right)
{
    if (left.size() != right.size())
        return false;
    for (size_t i = 0; i != left.size(); ++i)
        if (left[i] != right[i])
            return false;
    return true;
}
bool operator!=(storage const& left, storage const& right)
{
    return !(left == right);
}