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
        std::shared_ptr<std::vector<size_t>> data_;
        shared_vector(size_t size = 0, size_t element = 0);
        shared_vector(shared_vector const&);
        void detach();
    };
    union
    {
        size_t s_storage;
        shared_vector* d_storage;
    };
    int8_t size_;
    void safe_delete();
public:
    storage(size_t size = 0, size_t element = 0);
    storage(storage const&);
    ~storage();
    storage& operator=(storage const&);
    void resize(size_t, size_t element = 0);
    size_t size() const;
    size_t const& operator[](size_t) const;
    size_t& operator[](size_t);
    size_t const& back() const;
    size_t& back();
    void push_back(size_t);
    void pop_back();
    bool empty() const;
    void erase(size_t const, size_t const);
    void insert(size_t const, size_t const, size_t const);
    friend bool operator==(storage const&, storage const&);
    friend bool operator!=(storage const&, storage const&);
};

#endif //STORAGE_H