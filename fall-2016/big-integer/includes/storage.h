#ifndef STORAGE_H
#define STORAGE_H

#include <cstddef>
#include <vector>
#include <memory>

struct storage
{
private:
    uint8_t small;
    size_t s_storage;
    std::shared_ptr<std::vector<size_t>> d_storage;
    bool is_small() const;
public:
    storage(size_t const size = 0, size_t const el = 0);
    storage(storage const&);
    ~storage();
    storage& operator=(storage const&);
    size_t& operator[](size_t const);
    size_t const & operator[](size_t const) const;
    size_t& back();
    size_t const& back() const;
    void pop_back();
    void push_back(size_t const);
    void resize(size_t const size = 0, size_t const el = 0);
    void insert_to_begin(size_t const, size_t const);
    void erase_from_begin(size_t const);
    size_t size() const;
    bool empty() const;
};

#endif //STORAGE_H