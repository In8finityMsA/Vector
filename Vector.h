//
// Created by Admin on 22.02.2021.
//
#include <iostream>
#include <chrono>
#include <stdexcept>

#ifndef TEMPLATE_VECTOR_H
#define TEMPLATE_VECTOR_H

//TODO: make instance for bool
template<class ContainerType>
class Vector {

public:

    Vector() : size_(0), capacity_(1) {
        data_ = new ContainerType[capacity_];
    }
    explicit Vector(std::size_t size) : size_(size), capacity_(size) {
        data_ = new ContainerType[capacity_]();
    }
    Vector(std::size_t size, const ContainerType& elem) : size_(size), capacity_(size) {
        data_ = new ContainerType[capacity_];
        for (int i = 0; i < size_; i++) {
            data_[i] = elem;
        }
    }

    Vector(std::initializer_list<ContainerType> il) : size_(il.size()), capacity_(il.size()) {
        data_ = new int[capacity_];
        int i = 0;
        for (auto item : il) {
            data_[i++] = item;
        }
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
        return *this;
    };

    Vector(const Vector<ContainerType>& vector) : size_(vector.size()), capacity_(vector.size()) {
        data_ = new ContainerType[capacity_];

        for (int i = 0; i < vector.size(); i++) {
            data_[i] = vector[i];
        }
    }
    Vector<ContainerType>& operator = (const Vector<ContainerType>& vector) {
        if (this == &vector) {
            return  *this;
        }
        size_ = vector.size();
        capacity_ = vector.size();

        delete[] data_;
        data_ = new ContainerType[capacity_];

        for (int i = 0; i < vector.size(); i++) {
            data_[i] = vector[i];
        }

        return *this;
    }
    Vector(Vector<ContainerType>&& vector) noexcept : size_(vector.size()), capacity_(vector.size()) {
        data_ = vector.data_;
    }
    Vector<ContainerType>& operator = (Vector<ContainerType>&& vector) noexcept {
        size_ = vector.size();
        capacity_ = vector.size();
        delete[] data_;

        data_ = vector.data_;

        return *this;
    }

    void push_back(const ContainerType& elem);
    void push_back(ContainerType&& elem);
    void pop_back();
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
