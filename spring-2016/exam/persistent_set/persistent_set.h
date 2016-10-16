#ifndef PERSISTENT_SET_H
#define PERSISTENT_SET_H


#include <memory>
#include <vector>
struct persistent_set
{
    typedef int value_type;
private:
    struct node
    {
        value_type data;
        std::shared_ptr<node> left;
        std::shared_ptr<node> right;

        node();
        node(std::shared_ptr<node> const&);
        node(value_type const&);
        node(value_type const&, std::shared_ptr<node>, std::shared_ptr<node>);

        friend std::shared_ptr<node> find(std::shared_ptr<node>, value_type const &);
        friend std::shared_ptr<node> insert(std::shared_ptr<node>, std::shared_ptr<node>);
        friend std::shared_ptr<node> erase(std::shared_ptr<node>, value_type const&);

        std::shared_ptr<node> min();
        std::shared_ptr<node> max();
        std::shared_ptr<node> next(std::shared_ptr<node>);
        std::shared_ptr<node> prev(std::shared_ptr<node>);
    };

    std::vector<std::shared_ptr<node>> fake_roots;
public:
    struct iterator
    {
        std::shared_ptr<node> current;
        std::shared_ptr<node> root;

        iterator(std::shared_ptr<node>, std::shared_ptr<node>);
        iterator(iterator const&);

        value_type const& operator*() const;
        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);

        friend bool operator==(iterator const&, iterator const&);
        friend bool operator!=(iterator const&, iterator const&);
    };
    persistent_set();
    persistent_set(persistent_set const&);
    persistent_set& operator=(persistent_set const&);
    ~persistent_set();
    iterator find(value_type const&);
    std::pair<iterator, bool> insert(value_type const&);
    void erase(iterator);
    iterator begin() const;
    iterator end() const;
};


#endif //PERSISTENT_SET_H
