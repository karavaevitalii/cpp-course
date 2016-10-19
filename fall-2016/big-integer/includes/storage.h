#ifndef STORAGE_H
#define STORAGE_H

#include <cstddef>
#include <vector>
#include <memory>
struct storage
{
private:
    struct shared_vector
    {
        std::vector<size_t> data_;
        size_t links;
        shared_vector(size_t const, size_t const);
    };
    union
    {
        size_t s_storage;
        shared_vector* d_storage;
    };
    uint8_t small;
    bool is_small() const;
    uint8_t init_small(size_t const size);
    void detach();
    void safe_delete();
public:
    storage(size_t const size = 0, size_t const element = 0);
    storage(storage const&);
    ~storage();
    storage& operator=(storage const&);
    void resize(size_t const, size_t const element = 0);
    size_t size() const;
    size_t const& operator[](size_t const) const;
    size_t& operator[](size_t const);
    size_t const& back() const;
    size_t& back();
    void push_back(size_t const);
    void pop_back();
    bool empty() const;
    void erase(size_t const, size_t const);
    void insert(size_t const, size_t const, size_t const);
};

#endif //STORAGE_H
