#ifndef PERSISTENT_SET_H
#define PERSISTENT_SET_H

#include <memory>

struct persistent_set
{
    typedef int value_type;

    struct iterator;
    struct node;

private:
    std::shared_ptr<node> fake_root_;

public:
    persistent_set();
    persistent_set(persistent_set const&);
    ~persistent_set();

    persistent_set& operator=(persistent_set const&);

    iterator find(value_type const&);
    std::pair<iterator, bool> insert(value_type const&);
    void erase(iterator const&);

    iterator begin() const;
    iterator end() const;
};

struct persistent_set::node : std::enable_shared_from_this<node>
{
    value_type data_;
    std::shared_ptr<node> left_, right_;

    node(value_type const&);
    node(std::shared_ptr<node> const&);

    node& operator=(node const&);

    std::shared_ptr<node> find(value_type const&);
    std::shared_ptr<node> insert(value_type const&);
    void erase(std::shared_ptr<node> const&, std::shared_ptr<node> const&);

    std::shared_ptr<node> next(std::shared_ptr<node> const&);
    std::shared_ptr<node> prev(std::shared_ptr<node> const&);
};

struct persistent_set::iterator
{
    friend struct persistent_set;

private:
    std::shared_ptr<node> it_, root_;

public:
    iterator() = delete;
    iterator(std::shared_ptr<node> const&, std::shared_ptr<node> const&);
    iterator(iterator const&);

    iterator& operator=(iterator const&);

    value_type const& operator*() const;

    iterator& operator++();
    iterator operator++(int);

    iterator& operator--();
    iterator operator--(int);

    friend bool operator==(iterator const&, iterator const&);
    friend bool operator!=(iterator const&, iterator const&);
};

bool operator==(persistent_set::iterator const&, persistent_set::iterator const&);
bool operator!=(persistent_set::iterator const&, persistent_set::iterator const&);

#endif
