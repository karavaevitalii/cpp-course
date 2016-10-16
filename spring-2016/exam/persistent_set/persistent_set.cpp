#include "persistent_set.h"
persistent_set::node::node()
{}
persistent_set::node::node(std::shared_ptr<persistent_set::node> const& that)
    : data(that->data)
    , left(that->left)
    , right(that->right)
{}
persistent_set::node::node(persistent_set::value_type const& data)
    : data(data)
{}
persistent_set::node::node(persistent_set::value_type const& data,
                           std::shared_ptr<persistent_set::node> left,
                           std::shared_ptr<persistent_set::node> right)
    : data(data)
    , left(left)
    , right(right)
{}
std::shared_ptr<persistent_set::node>
persistent_set::node::find(std::shared_ptr<persistent_set::node> now,
                           persistent_set::value_type const &target)
{
    if (now == nullptr)
        return nullptr;
    if (target < now->data)
        return find(now->left, target);
    else if (target == now->data)
        return now;
    else
        return find(now->right, target);
}
std::shared_ptr<persistent_set::node>
persistent_set::node::insert(std::shared_ptr<persistent_set::node> now,
                             std::shared_ptr<persistent_set::node> target)
{
    if (now == nullptr)
        return std::make_shared<persistent_set::node>(target);
    if (target->data < now->data)
        return std::make_shared<persistent_set::node>(now->data, insert(now->left, target), now->right);
    else if (now->data == target->data)
        return std::make_shared<persistent_set::node>(target);
    else
        return std::make_shared<persistent_set::node>(now->data, now->left, insert(now->right, target));
}
std::shared_ptr<persistent_set::node>
persistent_set::node::erase(std::shared_ptr<persistent_set::node> now,
                            persistent_set::value_type const& target)
{
    if (now == nullptr)
        return nullptr;
    if (target < now->data)
        return std::make_shared<persistent_set::node>(now->data, erase(now->left, target), now->right);
    else if (target > now->data)
        return std::make_shared<persistent_set::node>(now->data, now->left, erase(now->right, target));
    else
    {
        if (now->left != nullptr && now->right != nullptr)
            return std::make_shared<persistent_set::node>(insert(now->left, now->right));
        else if (now->left != nullptr)
            return std::make_shared<persistent_set::node>(now->left->data, now->left->left, now->left->right);
        else if (now->right != nullptr)
            return std::make_shared<persistent_set::node>(now->right->data, now->right->left, now->right->right);
        else
            return nullptr;
    }
}
std::shared_ptr<persistent_set::node> persistent_set::node::min()
{
    std::shared_ptr<persistent_set::node> now = this;
    while (now->left)
        now = now->left;
    return now;
}
std::shared_ptr<persistent_set::node> persistent_set::node::max()
{
    std::shared_ptr<persistent_set::node> now = this;
    while (now->right)
        now = now->right;
    return now;
}
std::shared_ptr<persistent_set::node>
persistent_set::node::next(std::shared_ptr<persistent_set::node> now)
{
    if (data < now->data)
        return right ? right->next(now) : std::make_shared<persistent_set::node>(this);
    else
    {
        std::shared_ptr<persistent_set::node> found = left ? left->next(now) :
                                                      std::make_shared<persistent_set::node>(this);
        return found->data > now->data ? found : std::make_shared<persistent_set::node>(this);
    }
}
std::shared_ptr<persistent_set::node>
persistent_set::node::prev(std::shared_ptr<persistent_set::node> now)
{
    if (data > now->data)
        return right ? right->prev(now) : std::make_shared<persistent_set::node>(this);
    else
    {
        std::shared_ptr<persistent_set::node> found = right ? right->prev(now) :
                                                      std::make_shared<persistent_set::node>(this);
        return found->data < now->data ? found : std::make_shared<persistent_set::node>(this);
    }
}
persistent_set::iterator::iterator(std::shared_ptr<persistent_set::node> current,
                                   std::shared_ptr<persistent_set::node> root)
    : current(current)
    , root(root)
{}
persistent_set::iterator::iterator(persistent_set::iterator const& that)
    : current(that.current)
    , root(that.root)
{}
persistent_set::value_type const& persistent_set::iterator::operator*() const
{
    return current->data;
}
persistent_set::iterator& persistent_set::iterator::operator++()
{
    if (current->right)
        current = current->max();
    else
        current = root->next(current);
    return *this;
}
persistent_set::iterator persistent_set::iterator::operator++(int)
{
    iterator ret(*this);
    ++*this;
    return ret;
}
persistent_set::iterator& persistent_set::iterator::operator--()
{
    if (current->left)
        current = current->min();
    else
        current = root->prev(current);
    return *this;
}
persistent_set::iterator persistent_set::iterator::operator--(int)
{
    iterator ret(*this);
    --*this;
    return ret;
}
bool operator==(persistent_set::iterator const& left, persistent_set::iterator const& right)
{
    return left.current == right.current;
}
bool operator!=(persistent_set::iterator const& left, persistent_set::iterator const& right)
{
    return !(left == right);
}
persistent_set::persistent_set()
{
    fake_roots.push_back(nullptr);
}
persistent_set::persistent_set(persistent_set const& that)
    : fake_roots(that.fake_roots)
{}
persistent_set& persistent_set::operator=(persistent_set const& that)
{
    this->fake_roots = that.fake_roots;
    return *this;
}
persistent_set::~persistent_set()
{}
persistent_set::iterator persistent_set::find(persistent_set::value_type const& target)
{
    std::shared_ptr<persistent_set::node> found = find(fake_roots.back(), target);
    if (found == nullptr || found->data != target)
        return end();
    return iterator(found, fake_roots.back());
}
std::pair<persistent_set::iterator, bool> persistent_set::insert(persistent_set::value_type const& target)
{
    auto exists = find(target);
    if (exists != end())
        return {exists, false};
    std::shared_ptr<persistent_set::node> target_node = std::make_shared<persistent_set::node>(target);
    fake_roots.push_back(insert(fake_roots.back(), target_node));
    return {iterator(find(fake_roots.back(), target), fake_roots.back()), true};
}
void persistent_set::erase(persistent_set::iterator target)
{
    fake_roots.push_back(erase(fake_roots.back(), target.current->data));
}
persistent_set::iterator persistent_set::begin() const
{
    return iterator(fake_roots.back()->min(), fake_roots.back());
}
persistent_set::iterator persistent_set::end() const
{
    return iterator(fake_roots.back()->max(), fake_roots.back());
}


