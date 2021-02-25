//
// Created by Admin on 22.02.2021.
//
#include <iostream>
#include <chrono>
#include <stdexcept>
#include <type_traits>

#ifndef TEMPLATE_VECTOR_H
#define TEMPLATE_VECTOR_H

//TODO: make instance for bool
template<class ContainerType>
class Vector {

public:

    using PointerType = ContainerType*;

    Vector() : size_(0), capacity_(2) {
        data_ = new ContainerType[capacity_];
        std::cout << "Constructor: empty" << std::endl;
    }
    explicit Vector(size_t size) : size_(size), capacity_(size) {
        data_ = new ContainerType[capacity_]();
        std::cout << "Constructor: n" << std::endl;
    }
    Vector(size_t size, const ContainerType& elem) : size_(size), capacity_(size) {
        data_ = new ContainerType[capacity_];
        for (int i = 0; i < size_; i++) {
            data_[i] = elem;
        }
        std::cout << "Constructor: n, elem" << std::endl;
    }

    Vector(std::initializer_list<ContainerType> il) : size_(il.size()), capacity_(il.size()) {
        data_ = new ContainerType[capacity_];
        int i = 0;
        for (auto item : il) {
            data_[i++] = item;
        }
        std::cout << "Constructor: initializer list" << std::endl;
    };
    Vector& operator= (std::initializer_list<ContainerType> il){
        size_ = il.size();
        capacity_ = il.size();

        delete[] data_;
        data_ = new ContainerType[capacity_];

        int i = 0;
        for (auto item : il) {
            data_[i++] = item;
        }
        std::cout << "Assign: = initializer list" << std::endl;
        return *this;
    };

    Vector(const Vector<ContainerType>& vector) : size_(vector.size()), capacity_(vector.size()) {
        data_ = new ContainerType[capacity_];

        if (std::is_integral<ContainerType>::value) {
            memcpy(data_, vector.data_, size_ * sizeof(ContainerType));
            std::cout << "Constructor: = copy integral" << std::endl;
        }
        else {
            for (int i = 0; i < vector.size(); i++) {
                data_[i] = vector[i];
            }
            std::cout << "Constructor: = copy not integral" << std::endl;
        }
    }
    Vector<ContainerType>& operator = (const Vector<ContainerType>& vector) {
        if (this != &vector) {
            size_ = vector.size();
            capacity_ = vector.size();

            delete[] data_;
            data_ = new ContainerType[capacity_];

            if (std::is_integral<ContainerType>::value) {
                memcpy(data_, vector.data_, size_ * sizeof(ContainerType));
                std::cout << "Assign: = copy integral" << std::endl;
            } else {
                for (int i = 0; i < vector.size(); i++) {
                    data_[i] = vector[i];
                }
                std::cout << "Assign: = copy not integral" << std::endl;
            }
        }

        return *this;
    }

    Vector(Vector<ContainerType>&& vector) noexcept : size_(vector.size()), capacity_(vector.size()) {
        data_ = vector.data_;
        vector.data_ = nullptr;
        vector.size_ = 0;
        vector.capacity_ = 0;
        std::cout << "Constructor: move" << std::endl;
    }
    Vector<ContainerType>& operator = (Vector<ContainerType>&& vector) noexcept {
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

    void push_back(const ContainerType& elem) {
        std::cout << "Pushback copy" << std::endl;
        if (size_ == capacity_) {
            reserve(capacity_ * 2);
        }
        data_[size_++] = elem;
    }
    void push_back(ContainerType&& elem) {
        std::cout << "Pushback move" << std::endl;
        if (size_ == capacity_) {
            reserve(capacity_ * 2);
        }
        data_[size_++] = std::move(elem);
    };
    void pop_back() {
        size_--;
    };

    void insert(size_t index, const ContainerType& elem) {
        insert(index, 1, elem);
    };
    void insert(size_t index, size_t count, const ContainerType& elem) {
        std::cout << "Insert copy" << std::endl;
        if (index == size_) {
            for (int i = 0; i < count; i++)
                push_back(elem);
        }
        else if (index < size_) {
            if (size_ + count > capacity_) {
                std::cout << "Realloc" << std::endl;
                capacity_ += capacity_ > count ? capacity_ : count; //Multiply capacity_ by 2 or add count for storing new elements
                PointerType oldMemory = reallocate(capacity_);
                elementsMove(oldMemory, data_, /*count:*/ index);
                elementsMove(oldMemory + index, data_ + index + count, size_ - index);
                delete[] oldMemory;
            }
            else {
                elementsShift(data_ + index, size_ - index, /*shift:*/ count);
            }

            for (int i = index; i < index + count; i++) {
                data_[i] = elem;
            }
            size_ += count;
        }
        else throw std::runtime_error("Out of bounds exception!");
    }
    void insert(size_t index, ContainerType&& elem) {
        std::cout << "Insert move" << std::endl;
        if (index == size_) {
            push_back(std::move(elem));
        }
        else if (index < size_) {
            if (size_ + 1 > capacity_) {
                std::cout << "Realloc move" << std::endl;
                PointerType oldMemory = reallocate();
                elementsMove(oldMemory, data_, /*count:*/ index);
                elementsMove(oldMemory + index, data_ + index + 1, size_ - index);
                delete[] oldMemory;
            }
            else {
                elementsShift(data_ + index, size_ - index, /*shift:*/ 1);
            }

            data_[index] = std::move(elem);
            size_++;
        }
        else throw std::runtime_error("Out of bounds exception!");
    }
    void insert(size_t index, std::initializer_list<ContainerType> il) {
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
                PointerType oldMemory = reallocate(capacity_);
                elementsMove(oldMemory, data_, /*count:*/ index);
                elementsMove(oldMemory + index, data_ + index + il.size(), size_ - index);
                delete[] oldMemory;
            }
            else {
                elementsShift(data_ + index, size_ - index, /*shift:*/ il.size());
            }

            auto iter = il.begin();
            for (int i = index; i < index + il.size(); i++) {
                data_[i] = *(iter++);
            }
            size_ += il.size();
        }
        else throw std::runtime_error("Out of bounds exception!");
    }

    void erase(size_t index);
    void erase(size_t first, size_t last);
    void clear();

    ContainerType& front() const;
    ContainerType& back() const;
    ContainerType* begin() const;
    ContainerType* end() const;

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
            elementsMove(oldMemory, data_, size_);
            delete[] oldMemory;
        }
    };
    void resize(size_t size, const ContainerType& elem = ContainerType() );
    void shrink_to_fit();

    ContainerType& operator[] (size_t index) {
        return data_[index];
    }
    const ContainerType& operator[] (size_t index) const {
        return data_[index];
    }
    ContainerType& at(size_t index) const {
        if (index < size_) {
            return data_[index];
        }
        else throw std::runtime_error("Out of bounds exception!");
    };

    virtual ~Vector() {
        delete[] data_;
    };

private:
    ContainerType* data_;
    size_t size_;
    size_t capacity_;

    [[nodiscard]] PointerType reallocate() {
        auto tempPtr = data_;
        data_ = new ContainerType[capacity_ *= 2];

        return tempPtr;
    }
    [[nodiscard]] PointerType reallocate(size_t amount) {
        capacity_ = amount;
        auto tempPtr = data_;
        data_ = new ContainerType[amount];

        return tempPtr;
    }

    void elementsMove(PointerType src, PointerType dst, size_t count) {
        for (PointerType ptr = dst; ptr < dst + count; ptr++) {
            *ptr = std::move( *(src++) );
        }
    }

    void elementsCopy(PointerType src, PointerType dst, size_t count) {
        for (PointerType ptr = dst; ptr < dst + count; ptr++) {
            *ptr = *(src++);
        }
    }

    //make it more readable
    void elementsShift(PointerType begin, size_t count, size_t shift) {
        PointerType dst = begin + count + shift - 1;
        for (PointerType src = begin + count - 1; src >= begin; src--) {
            *(dst--) = std::move( *src );
        }
    }

};


#endif //TEMPLATE_VECTOR_H
