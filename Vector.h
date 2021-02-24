//
// Created by Admin on 22.02.2021.
//
#include <iostream>
#include <chrono>

#ifndef TEMPLATE_VECTOR_H
#define TEMPLATE_VECTOR_H

//TODO: make instance for bool
template<class ContainerType>
class Vector {

public:

    Vector();
    explicit Vector(std::size_t size);
    Vector(std::size_t size, const ContainerType& elem);

    Vector(std::initializer_list<ContainerType> il);
    Vector& operator= (std::initializer_list<ContainerType> il);

    Vector(const Vector<ContainerType>&);
    Vector<ContainerType>& operator = (const Vector<ContainerType>&);
    Vector(Vector<ContainerType>&&) noexcept;
    Vector<ContainerType> operator = (Vector<ContainerType>&&) noexcept;

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

    ContainerType& operator[] (std::size_t index);
    const ContainerType& operator[] (std::size_t index) const;
    ContainerType& at(std::size_t index);
    const ContainerType& at(std::size_t index) const;

    virtual ~Vector() {
        delete[] data_;
    };

private:
    ContainerType* data_;
    std::size_t size_;
    std::size_t capacity_;

};


#endif //TEMPLATE_VECTOR_H
