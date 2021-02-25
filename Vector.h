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

    Vector() : size_(0), capacity_(1) {
        data_ = new ContainerType[capacity_];
        std::cout << "Constructor: empty" << std::endl;
    }
    explicit Vector(std::size_t size) : size_(size), capacity_(size) {
        data_ = new ContainerType[capacity_]();
        std::cout << "Constructor: n" << std::endl;
    }
    Vector(std::size_t size, const ContainerType& elem) : size_(size), capacity_(size) {
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
                auto start = std::chrono::steady_clock::now();
                for (int i = 0; i < vector.size(); i++) {
                    data_[i] = vector[i];
                }
                std::cout << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count() << std::endl;
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

    void reallocate() {
        auto tempPtr = data_;
        data_ = new ContainerType[capacity_ *= 2];
        for (int i = 0; i < size_; i++) {
            data_[i] = tempPtr[i];
        }
        delete[] tempPtr;
    }
    void push_back(const ContainerType& elem) {
        if (size_ >= capacity_) {
            reallocate();
            std::cout << "Realloc finished" << std::endl;
        }
        if (data_ != nullptr)
            data_[size_++] = elem;
        else throw std::runtime_error("Null pointer exception!");
    }
    void push_back(ContainerType&& elem) {
        if (size_ >= capacity_) {
            reallocate();
        }
        if (data_ != nullptr) {
            data_[size_++] = std::move(elem);
        }
        else throw std::runtime_error("Null pointer exception!");
    };
    void pop_back() {
        size_--;
    };
    void insert(std::size_t index, const ContainerType& elem);

    void erase(std::size_t index);
    void erase(std::size_t first, std::size_t last);
    void clear();

    ContainerType& front() const;
    ContainerType& back() const;
    ContainerType* begin() const;
    ContainerType* end() const;
    std::size_t size() const noexcept {
        return size_;
    };
    bool empty() const noexcept {
        return size_ == 0;
    };

    void reserve(std::size_t capacity);
    void resize(std::size_t size, const ContainerType& elem = ContainerType() );
    void shrink_to_fit();

    ContainerType& operator[] (std::size_t index) {
        return data_[index];
    }
    const ContainerType& operator[] (std::size_t index) const {
        return data_[index];
    }
    ContainerType& at(std::size_t index) const {
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
    std::size_t size_;
    std::size_t capacity_;

};


#endif //TEMPLATE_VECTOR_H
