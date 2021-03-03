//
// Created by Admin on 22.02.2021.
//
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <iterator>
#include "iterators.h"
#include <chrono>
#include <cassert>

#ifndef TEMPLATE_VECTOR_H
#define TEMPLATE_VECTOR_H

//TODO: make instance for bool
template<class ContainerType>
class Vector {
    friend ConstantIterator<Vector>;
    friend VectorIterator<Vector>;

public:
    typedef std::random_access_iterator_tag       iterator_category;
    typedef ContainerType                         value_type;
    typedef ptrdiff_t                             difference_type;
    typedef value_type*                           pointer;
    typedef value_type&                           reference;
    typedef const value_type&                     const_reference;
    typedef VectorIterator<Vector<value_type>>    iterator;
    typedef ConstantIterator<Vector<value_type>>  const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    Vector() : size_(0), capacity_(2) {
        data_ = new value_type[capacity_];
        std::cout << "Constructor: empty" << std::endl;
    }
    explicit Vector(size_t size) : size_(size), capacity_(size) {
        data_ = new value_type[capacity_]();
        std::cout << "Constructor: n" << std::endl;
    }
    Vector(size_t size, const_reference elem) : size_(size), capacity_(size) {
        data_ = new value_type[capacity_];
        for (int i = 0; i < size_; i++) {
            data_[i] = elem;
        }
        std::cout << "Constructor: n, elem" << std::endl;
    }

    Vector(std::initializer_list<value_type> il) : size_(il.size()), capacity_(il.size()) {
        data_ = new value_type[capacity_];
        int i = 0;
        for (auto item : il) {
            data_[i++] = item;
        }
        std::cout << "Constructor: initializer list" << std::endl;
    };
    Vector& operator= (std::initializer_list<value_type> il){
        size_ = il.size();
        capacity_ = il.size();

        delete[] data_;
        data_ = new value_type[capacity_];

        int i = 0;
        for (auto item : il) {
            data_[i++] = item;
        }
        std::cout << "Assign: = initializer list" << std::endl;
        return *this;
    };

    Vector(const Vector<value_type>& vector) : size_(vector.size()), capacity_(vector.size()) {
        data_ = new value_type[capacity_];

        elementsCopy(data_, vector.data_, vector.size());
        std::cout << "Constructor: = copy" << std::endl;
    }
    Vector<value_type>& operator = (const Vector<value_type>& vector) {
        if (this != &vector) {
            size_ = vector.size();
            capacity_ = vector.size();

            delete[] data_;
            data_ = new value_type[capacity_];

            elementsCopy(data_, vector.data_, vector.size());
            std::cout << "Assign: = copy" << std::endl;
        }

        return *this;
    }

    Vector(Vector<value_type>&& vector) noexcept : size_(vector.size()), capacity_(vector.size()) {
        data_ = vector.data_;
        vector.data_ = nullptr;
        vector.size_ = 0;
        vector.capacity_ = 0;
        std::cout << "Constructor: move" << std::endl;
    }
    Vector<value_type>& operator = (Vector<value_type>&& vector) noexcept {
        if (this != &vector) {
            size_ = vector.size();
            capacity_ = vector.size();
            delete[] data_;

            data_ = vector.data_;
            vector.data_ = nullptr;
            vector.size_ = 0;
            vector.capacity_ = 0;
            std::cout << "Assign: = move" << std::endl;
        }

        return *this;
    }

    void push_back(const_reference elem) {
        //std::cout << "Pushback copy" << std::endl;
        if (size_ == capacity_) {
            reserve(capacity_ * 2);
        }
        data_[size_++] = elem;
    }
    void push_back(value_type&& elem) {
        std::cout << "Pushback move" << std::endl;
        if (size_ == capacity_) {
            reserve(capacity_ * 2);
        }
        data_[size_++] = std::move(elem);
    };

    template<class... Args>
    void emplace_back(Args&&... constructor_args) {
        if (size_ == capacity_) {
            reserve(capacity_ * 2);
        }
        new (data_ + size_++) value_type(std::forward<Args>(constructor_args)...);
    }

    void pop_back() {
        if (!empty()) {
            if (std::is_destructible<value_type>::value)
                data_[size_ - 1].~value_type();
            else if (std::is_default_constructible<value_type>::value)
                data_[size_ - 1] = value_type();
            size_--;
        }
    };

    iterator insert(size_t index, const_reference elem) {
        insert(index, 1, elem);
    };
    iterator insert(size_t index, size_t count, const_reference elem) {
        std::cout << "Insert copy" << std::endl;
        if (index == size_) {
            for (int i = 0; i < count; i++)
                push_back(elem);
        }
        else if (index < size_) {
            if (size_ + count > capacity_) {
                std::cout << "Realloc" << std::endl;
                ///Maybe make this allocate more than count if it's greater than doubled capacity?
                capacity_ += capacity_ > count ? capacity_ : count; //Multiply capacity_ by 2 or add count for storing new elements
                pointer oldMemory = reallocate(capacity_);
                elementsMove(data_, oldMemory, /*count:*/ index);
                elementsMove(data_ + index + count, oldMemory + index,size_ - index);
                delete[] oldMemory;
            }
            else {
                elementsShiftRight(data_ + index, size_ - index, /*shift:*/ count);
            }

            for (int i = index; i < index + count; i++) {
                data_[i] = elem;
            }
            size_ += count;
        }
        else throw std::runtime_error("Out of bounds exception!");

        return iterator (data_ + index);
    }
    iterator insert(size_t index, value_type&& elem) {
        std::cout << "Insert move" << std::endl;
        if (index == size_) {
            push_back(std::move(elem));
        }
        else if (index < size_) {
            if (size_ + 1 > capacity_) {
                std::cout << "Realloc move" << std::endl;
                pointer oldMemory = reallocate();
                elementsMove(data_, oldMemory, /*count:*/ index);
                elementsMove(data_ + index + 1, oldMemory + index, size_ - index);
                delete[] oldMemory;
            }
            else {
                elementsShiftRight(data_ + index, size_ - index, /*shift:*/ 1);
            }

            data_[index] = std::move(elem);
            size_++;
        }
        else throw std::runtime_error("Out of bounds exception!");

        return iterator (data_ + index);
    }
    iterator insert(size_t index, std::initializer_list<value_type> il) {
        std::cout << "Insert init list" << std::endl;
        if (index == size_) {
            for (auto iter = il.begin(); iter != il.end(); iter++) {
                push_back(*iter);
            }
        }
        else if (index < size_) {
            if (size_ + il.size() > capacity_) {
                std::cout << "Realloc" << std::endl;
                capacity_ += capacity_ > il.size() ? capacity_ : il.size(); //Multiply capacity_ by 2 or add il.size for storing new elements
                pointer oldMemory = reallocate(capacity_);
                elementsMove(data_, oldMemory, /*count:*/ index);
                elementsMove(data_ + index + il.size(), oldMemory + index, size_ - index);
                delete[] oldMemory;
            }
            else {
                elementsShiftRight(data_ + index, size_ - index, /*shift:*/ il.size());
            }

            auto iter = il.begin();
            for (int i = index; i < index + il.size(); i++) {
                data_[i] = *(iter++);
            }
            size_ += il.size();
        }
        else throw std::runtime_error("Out of bounds exception!");

        return iterator (data_ + index);
    }

    iterator erase(size_t index) {
        erase(index, 1);
    }
    iterator erase(size_t index, size_t count) {
        if (index == size_ - count) {
            for (int i = 0; i < count; i++) {
                pop_back();
            }
        }
        else if (index < size_ - count) {
            elementsShiftLeft(data_ + index + count, size_ - index - count, /*shift:*/ count);
            size_ -= count;
        }
        else throw std::runtime_error("Out of bounds exception!");

        return iterator (data_ + index);
    }
    void clear() {
        while (size_) {
            pop_back();
        }
    };

    reference front() {
        return operator[](0);
    };
    const_reference front() const {
        return operator[](0);
    };
    reference back() {
        return operator[](size_ - 1);
    };
    const_reference back() const {
        return operator[](size_ - 1);
    };

    iterator begin() noexcept {
        return iterator(data_);
    }
    const_iterator begin() const noexcept {
        return const_iterator(data_);
    }
    const_iterator cbegin() const noexcept {
        return begin();
    }
    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }
    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    iterator end() noexcept {
        return iterator(data_ + size_);
    }
    const_iterator end() const noexcept {
        return const_iterator(data_ + size_);
    }
    const_iterator cend() const noexcept {
        return end();
    }
    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    size_t capacity() const noexcept {
        return capacity_;
    };
    size_t size() const noexcept {
        return size_;
    };
    bool empty() const noexcept {
        return size_ == 0;
    };

    void reserve(size_t capacity) {
        if (capacity > capacity_) {
            auto oldMemory = reallocate(capacity); //capacity_ member variable is renewed in reallocate
            elementsMove(data_, oldMemory, size_);
            delete[] oldMemory;
        }
    };
    void resize(size_t size, const_reference elem = value_type() ) {
        while (size < size_) {
            pop_back(); //decrements size_
        }
       if (size > size_) {
            if (size > capacity_) {
                reserve(size);
            }
            while (size > size_) {
                push_back(elem); //increments size_
            }
        }
    }
    void shrink_to_fit() {
        if (capacity() > size_) {
            capacity_ = 0; //to trick reserve function
            reserve(size_);
        }
    }

    reference operator[] (size_t index) {
        return data_[index];
    }
    const_reference operator[] (size_t index) const {
        return data_[index];
    }
    reference at(size_t index) {
        if (index < size_) {
            return data_[index];
        }
        else throw std::runtime_error("Out of bounds exception!");
    };
    const_reference at(size_t index) const {
        if (index < size_) {
            return data_[index];
        }
        else throw std::runtime_error("Out of bounds exception!");
    };

    virtual ~Vector() {
        delete[] data_;
    };

private:
    pointer data_;
    size_t size_;
    size_t capacity_;

    /**
     * @return pointer to old memory
     */
    [[nodiscard]] pointer reallocate() {
        auto tempPtr = data_;
        data_ = new value_type[capacity_ *= 2];

        return tempPtr;
    }
    /**
     * @return pointer to old memory
     */
    [[nodiscard]] pointer reallocate(size_t amount) {
        capacity_ = amount;
        auto tempPtr = data_;
        data_ = new value_type[amount];

        return tempPtr;
    }

    void elementsMove(pointer dst, pointer src, size_t count) {
        if (std::is_integral<value_type>::value) {
            memcpy(dst, src, count * sizeof(value_type));
        } else {
            for (pointer ptr = dst; ptr < dst + count; ptr++) {
                *ptr = std::move( *(src++) );
            }
        }

    }

    void elementsCopy(pointer dst, pointer src, size_t count) {
        if (std::is_integral<value_type>::value) {
            memcpy(dst, src, count * sizeof(value_type));
        } else {
            for (pointer ptr = dst; ptr < dst + count; ptr++) {
                *ptr = *(src++);
            }
        }
    }

    void elementsShift(pointer begin, size_t chunkSize, long long shift) {
        if (shift > 0) {
            elementsShiftRight(begin, chunkSize, shift);
        }
        else if (shift < 0) {
            elementsShiftLeft(begin, chunkSize, std::abs(shift));
        }
    }

    //make it more readable
    void elementsShiftRight(pointer begin, size_t chunkSize, size_t shift) {
        pointer dst = begin + chunkSize + shift - 1;
        for (pointer src = begin + chunkSize - 1; src >= begin; src--) {
            *(dst--) = std::move( *src );
        }
    }

    void elementsShiftLeft(pointer begin, size_t chunkSize, size_t shift) {
        pointer dst = begin - shift;
        for (pointer src = begin; src < begin + chunkSize; src++) {
            *(dst++) = std::move( *src );
        }
    }

};


#endif //TEMPLATE_VECTOR_H
