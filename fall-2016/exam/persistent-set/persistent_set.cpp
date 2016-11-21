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

std::shared_ptr<psn> psn::find(value_type target)
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

std::shared_ptr<psn> psn::insert(value_type target)
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

void psn::erase(std::shared_ptr<node> target, std::shared_ptr<node> parent)
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

std::shared_ptr<psn> psn::next(std::shared_ptr<node> target)
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

std::shared_ptr<psn> psn::prev(std::shared_ptr<node> target)
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

psi::iterator(std::shared_ptr<psn> el, std::shared_ptr<psn> root)
    : el_(el)
    , root_(root)
{}

psi::iterator(psi const& that)
    : el_(that.el_)
    , root_(that.root_)
{}

psi& psi::operator=(psi const& that)
{
    el_ = that.el_;
    root_ = that.root_;
    return *this;
}

ps::value_type const& psi::operator*() const
{
    return el_->data_;
}

psi& psi::operator++()
{
    if (el_->right_)
    {
        el_ = el_->right_;
        std::shared_ptr<psn> p;
        while (el_->left_)
        {
            p = el_->left_;
            el_ = p;
        }
    }
    else
        el_ = root_->next(el_);

    return *this;
}

psi& psi::operator--()
{
    if (el_->left_)
    {
        el_ = el_->left_;
        std::shared_ptr<psn> p;
        while (el_->right_)
        {
            p = el_->right_;
            el_ = p;
        }
    }
    else
        el_ = root_->prev(el_);

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

bool operator==(psi left, psi right)
{
    return left.el_->data_ == right.el_->data_;
}

bool operator!=(psi left, psi right)
{
    return !(left == right);
}

psi ps::begin() const
{
    std::shared_ptr<psn> p = root;
    while (p->left_)
        p = p->left_;

    return iterator(p, root);
}

psi ps::end() const
{
    std::shared_ptr<psn> p = root;
    while (p->right_)
        p = p->right_;

    return iterator(p, root);
}

ps::persistent_set()
    : root(std::make_shared<psn>(std::numeric_limits<int>::max()))
{}

ps::persistent_set(persistent_set const& that)
    : root(std::make_shared<psn>(that.root))
{}

ps::~persistent_set()
{
    root.reset();
}

persistent_set& ps::operator=(persistent_set const& that)
{
    root = that.root;
    return *this;
}

psi ps::find(value_type target)
{
    std::shared_ptr<psn> found = root->find(target);

    if (!found)
        return end();

    return iterator(found, root);
}

std::pair<psi, bool> ps::insert(ps::value_type target)
{
    psi found = find(target);

    if (found != end())
        return {found, false};

    found = iterator(root->insert(target), root);
    return {found, true};
}

void ps::erase(psi target)
{
    root->erase(target.el_, root);
}







