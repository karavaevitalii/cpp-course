#include "persistent_set.h"
#include <limits>

typedef persistent_set ps;
typedef ps::node psn;
typedef ps::iterator psi;

psn::node(ps::value_type const& data)
    : data_(data)
    , left_(nullptr)
    , right_(nullptr)
{}

psn::node(std::shared_ptr<psn> const& that)
    : data_(that->data_)
    , left_(that->left_)
    , right_(that->right_)
{}

psn& psn::operator=(psn const& that)
{
    data_ = that.data_;
    left_ = that.left_;
    right_ = that.right_;
    return *this;
}

std::shared_ptr<psn> psn::find(value_type const& target)
{
    if (this == nullptr)
        return nullptr;

    if (data_ > target)
        return left_->find(target);
    else if (data_ == target)
        return shared_from_this();
    else
        return right_->find(target);
}

std::shared_ptr<psn> psn::insert(value_type const& target)
{
    if (data_ > target)
    {
        if (!left_)
        {
            left_ = std::make_shared<psn>(target);
            return left_;
        }
        else
        {
            left_ = std::make_shared<node>(left_);
            return left_->insert(target);
        }
    }
    else if (data_ == target)
        return shared_from_this();
    else
    {
        if (!right_)
        {
            right_ = std::make_shared<psn>(target);
            return right_;
        }
        else
        {
            right_ = std::make_shared<psn>(right_);
            return right_->insert(target);
        }
    }
}

void psn::erase(std::shared_ptr<node> const& target, std::shared_ptr<node> const& parent)
{
    if (data_ > target->data_)
    {
        left_ = std::make_shared<psn>(left_);
        left_->erase(target, shared_from_this());
    }
    else if (data_ == target->data_)
    {
        if (left_)
        {
            left_ = std::make_shared<psn>(left_);
            std::shared_ptr<psn> p = left_;
            while (p->right_)
            {
                p->right_ = std::make_shared<psn>(p->right_);
                p = p->right_;
            }
            p->right_ = right_;
            right_ = nullptr;
        }
        if (parent->right_ != shared_from_this())
        {
            if (!left_)
                parent->left_ = right_;
            else
                parent->left_ = left_;
        }
        else
        {
            if (!right_)
                parent->right_ = left_;
            else
                parent->right_ = right_;
        }
    }
    else
    {
        right_ = std::make_shared<psn>(right_);
        right_->erase(target, shared_from_this());
    }
}

std::shared_ptr<psn> psn::next(std::shared_ptr<node> const& target)
{
    if (data_ >= target->data_)
    {
        std::shared_ptr<psn> ret;
        if (!left_)
            ret = shared_from_this();
        else
            ret = left_->next(target);

        if (ret->data_ <= target->data_)
            ret = shared_from_this();

        return ret;
    }

    if (!right_)
        return shared_from_this();

    return right_->next(target);
}

std::shared_ptr<psn> psn::prev(std::shared_ptr<node> const& target)
{
    if (data_ > target->data_)
    {
        if (!left_)
            return shared_from_this();
        return left_->prev(target);
    }

    std::shared_ptr<psn> ret;
    if (!right_)
        ret = shared_from_this();
    else
        ret = right_->prev(target);

    if (ret->data_ >= target->data_)
        ret = shared_from_this();

    return ret;
}

psi::iterator(std::shared_ptr<psn> const& el, std::shared_ptr<psn> const& root)
    : it_(el)
    , root_(root)
{}

psi::iterator(psi const& that)
    : it_(that.it_)
    , root_(that.root_)
{}

psi& psi::operator=(psi const& that)
{
    it_ = that.it_;
    root_ = that.root_;
    return *this;
}

ps::value_type const& psi::operator*() const
{
    return it_->data_;
}

psi& psi::operator++()
{
    if (it_->right_)
    {
        it_ = it_->right_;
        std::shared_ptr<psn> p;
        while (it_->left_)
        {
            p = it_->left_;
            it_ = p;
        }
    }
    else
        it_ = root_->next(it_);

    return *this;
}

psi& psi::operator--()
{
    if (it_->left_)
    {
        it_ = it_->left_;
        std::shared_ptr<psn> p;
        while (it_->right_)
        {
            p = it_->right_;
            it_ = p;
        }
    }
    else
        it_ = root_->prev(it_);

    return *this;
}

psi psi::operator++(int)
{
    psi ret = *this;
    ++*this;
    return ret;
}

psi psi::operator--(int)
{
    psi ret = *this;
    --*this;
    return ret;
}

bool operator==(psi const& left, psi const& right)
{
    return left.it_->data_ == right.it_->data_;
}

bool operator!=(psi const& left, psi const& right)
{
    return !(left == right);
}

psi ps::begin() const
{
    std::shared_ptr<psn> p = fake_root_;
    while (p->left_)
        p = p->left_;

    return iterator(p, fake_root_);
}

psi ps::end() const
{
    std::shared_ptr<psn> p = fake_root_;
    while (p->right_)
        p = p->right_;

    return iterator(p, fake_root_);
}

ps::persistent_set()
    : fake_root_(std::make_shared<psn>(std::numeric_limits<int>::max()))
{}

ps::persistent_set(persistent_set const& that)
    : fake_root_(std::make_shared<psn>(that.fake_root_))
{}

ps::~persistent_set()
{
    fake_root_.reset();
}

persistent_set& ps::operator=(persistent_set const& that)
{
    fake_root_ = that.fake_root_;
    return *this;
}

psi ps::find(value_type const& target)
{
    std::shared_ptr<psn> found = fake_root_->find(target);

    if (!found)
        return end();

    return iterator(found, fake_root_);
}

std::pair<psi, bool> ps::insert(ps::value_type const& target)
{
    psi found = find(target);

    if (found != end())
        return {found, false};

    found = iterator(fake_root_->insert(target), fake_root_);
    return {found, true};
}

void ps::erase(psi const& target)
{
    fake_root_->erase(target.it_, fake_root_);
}
