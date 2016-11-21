#ifndef PERSISTENT_SET_H
#define PERSISTENT_SET_H

#include <memory>

struct persistent_set
{
    typedef int value_type;
    struct iterator;
    struct node;
private:
       std::shared_ptr<node> root;
public:
    persistent_set();
    persistent_set(persistent_set const&);
    ~persistent_set();

    persistent_set& operator=(persistent_set const&);

    iterator find(value_type);
    std::pair<iterator, bool> insert(value_type);
    void erase(iterator);

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

    std::shared_ptr<node> find(value_type);
    std::shared_ptr<node> insert(value_type);
    void erase(std::shared_ptr<node>, std::shared_ptr<node>);

    std::shared_ptr<node> next(std::shared_ptr<node>);
    std::shared_ptr<node> prev(std::shared_ptr<node>);
};

struct persistent_set::iterator
{
    friend struct persistent_set;
private:
    std::shared_ptr<node> el_, root_;

public:
    iterator() = delete;
    iterator(std::shared_ptr<node>, std::shared_ptr<node>);
    iterator(iterator const&);

    iterator& operator=(iterator const&);

    value_type const& operator*() const;

    iterator& operator++();
    iterator operator++(int);

    iterator& operator--();
    iterator operator--(int);

    friend bool operator==(iterator, iterator);
    friend bool operator!=(iterator, iterator);
};

bool operator==(persistent_set::iterator, persistent_set::iterator);
bool operator!=(persistent_set::iterator, persistent_set::iterator);

#endif //PERSISTENT_SET_H
