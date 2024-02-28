#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <memory>

namespace CrazyDave {
    template<typename T>
    class vector {
    private:
        T *data;
        std::allocator<T> alloc;
        size_t currentSize;
        size_t capacity;

        void double_space() {
            T *tmp = data;
            data = alloc.allocate(capacity * 2);
            for (int i = 0; i < currentSize; ++i) {
                std::construct_at(data + i, tmp[i]);
                (tmp + i)->~T();
            }
            alloc.deallocate(tmp, capacity);
            capacity <<= 1;
        }

    public:
        class const_iterator;


        class iterator {
            friend vector<T>;
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            T *ptr;
            vector<T> *vec;


        public:
            explicit iterator(T *ptr = nullptr, vector<T> *vec = nullptr) : ptr(ptr), vec(vec) {}

            iterator operator+(const int &n) const {
                //TODO
                iterator itr(ptr + n, this->vec);
                return itr;
            }

            iterator operator-(const int &n) const {
                //TODO
                iterator itr(ptr - n, this->vec);
                return itr;
            }

            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                if (vec != rhs.vec)throw invalid_iterator();
                return rhs.ptr - ptr;
            }

            iterator &operator+=(const int &n) {
                *this = *this + n;
                return *this;
            }

            iterator &operator-=(const int &n) {
                *this = *this - n;
                return *this;
            }

            iterator operator++(int) {
                iterator tmp = *this;
                ++ptr;
                return tmp;
            }

            iterator &operator++() {
                ++ptr;
                return *this;
            }

            iterator operator--(int) {
                iterator tmp = *this;
                --ptr;
                return tmp;
            }

            iterator &operator--() {
                --ptr;
                return *this;
            }

            T &operator*() const {
                return *ptr;
            }

            bool operator==(const iterator &rhs) const {
                return ptr == rhs.ptr;
            }

            bool operator==(const const_iterator &rhs) const {
                return ptr == rhs.ptr;
            }

            bool operator!=(const iterator &rhs) const {
                return !(rhs == *this);
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(rhs == *this);
            }
        };

        class const_iterator {
            friend vector<T>;
        private:
            T *ptr;
            const vector<T> *vec;
        public:
            const_iterator(T *ptr = nullptr, const vector<T> *vec = nullptr) : ptr(ptr), vec(vec) {}

            const_iterator operator+(const int &n) const {
                iterator itr(ptr + n, this->vec);
                return itr;
            }

            const_iterator operator-(const int &n) const {
                const_iterator itr(ptr - n, this->vec);
                return itr;
            }

            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                if (vec != rhs.vec)throw invalid_iterator();
                return rhs.ptr - ptr;
            }

            const_iterator &operator+=(const int &n) {
                *this = *this + n;
                return *this;
            }

            const_iterator &operator-=(const int &n) {
                *this = *this - n;
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator tmp = *this;
                ++ptr;
                return tmp;
            }

            const_iterator &operator++() {
                ++ptr;
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator tmp = *this;
                --ptr;
                return tmp;
            }

            const_iterator &operator--() {
                --ptr;
                return *this;
            }

            T &operator*() const {
                return *ptr;
            }

            bool operator==(const iterator &rhs) const {
                return ptr == rhs.ptr;
            }

            bool operator==(const const_iterator &rhs) const {
                return ptr == rhs.ptr;
            }

            bool operator!=(const iterator &rhs) const {
                return !(rhs == *this);
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(rhs == *this);
            }
        };


        vector() {
            data = alloc.allocate(10);
            capacity = 10;
            currentSize = 0;
        }

        vector(const vector &other) {
            currentSize = other.currentSize;
            capacity = other.capacity;
            data = alloc.allocate(capacity);
            for (int i = 0; i < currentSize; ++i) {
                std::construct_at(data + i, other.data[i]);
            }
        }

        ~vector() {
            for (int i = 0; i < currentSize; ++i) {
                (data + i)->~T();
            }
            alloc.deallocate(data, capacity);
        }

        vector &operator=(const vector &other) {
            if (&other == this)return *this;
            for (int i = 0; i < currentSize; ++i) {
                (data + i)->~T();
            }
            alloc.deallocate(data, capacity);
            currentSize = other.currentSize;
            capacity = other.capacity;
            data = alloc.allocate(capacity);
            for (int i = 0; i < currentSize; ++i) {
                std::construct_at(data + i, other.data[i]);
            }

            return *this;
        }

        T &at(const size_t &pos) {
            if (pos < 0 || pos >= currentSize)throw index_out_of_bound();
            return data[pos];
        }

        const T &at(const size_t &pos) const {
            if (pos < 0 || pos >= currentSize)throw index_out_of_bound();
            return data[pos];
        }

        T &operator[](const size_t &pos) {
            if (pos < 0 || pos >= capacity)throw index_out_of_bound();
            return data[pos];
        }

        const T &operator[](const size_t &pos) const {
            if (pos < 0 || pos >= capacity)throw index_out_of_bound();
            return data[pos];
        }

        const T &front() const {
            if (currentSize == 0)throw container_is_empty();
            return data[0];
        }

        const T &back() const {
            if (currentSize == 0)throw container_is_empty();
            return data[currentSize - 1];
        }

        iterator begin() {
            iterator itr(data, this);
            return itr;
        }

        const_iterator cbegin() const {
            const_iterator itr(data, this);
            return itr;
        }

        iterator end() {
            iterator itr(data + currentSize, this);
            return itr;
        }

        const_iterator cend() const {
            const_iterator itr(data + currentSize, this);
            return itr;
        }

        bool empty() const {
            return currentSize == 0;
        }

        size_t size() const {
            return currentSize;
        }

        void clear() {
            for (int i = 0; i < capacity; ++i) {
                (data + i)->~T();
            }
            alloc.deallocate(data, capacity);
            data = alloc.allocate(10);
            currentSize = 0;
            capacity = 10;
        }

        iterator insert(iterator pos, const T &value) {
            if (currentSize == capacity) {
                int ind = pos.ptr - data;
                double_space();
                pos.ptr = data + ind;
            }
            iterator tmp = end() + 1;
            for (; tmp != pos; --tmp) {
                *tmp = *(tmp - 1);
            }
            *tmp = value;
            ++currentSize;
            return tmp;
        }

        iterator insert(const size_t &ind, const T &value) {
            if (ind > currentSize)throw index_out_of_bound();
            iterator itr(data + ind, this);
            insert(itr, value);
        }

        iterator erase(iterator pos) {
            if (pos == end()) {
                pop_back();
                return end();
            }
            for (; pos != end(); ++pos) {
                *pos = *(pos + 1);
            }
            --currentSize;
            return pos;
        }

        iterator erase(const size_t &ind) {
            if (ind >= currentSize)throw index_out_of_bound();
            iterator itr(data + ind, this);
            erase(itr);
        }

        void push_back(const T &value) {
            if (currentSize == capacity)double_space();
            std::construct_at(data + currentSize, value);
            ++currentSize;
        }

        void pop_back() {
            if (currentSize == 0)throw container_is_empty();
            (data + currentSize - 1)->~T();
            --currentSize;
        }
    };
}
#endif