//
// Created by Admin on 02.03.2021.
//
#include <iterator>

#ifndef TEMPLATE_ITERATORS_H
#define TEMPLATE_ITERATORS_H

template<class _vector>
class ConstantIterator {

public:

    typedef std::random_access_iterator_tag  iterator_category;
    typedef typename _vector::value_type     value_type;
    typedef ptrdiff_t                        difference_type;
    typedef const value_type*                pointer;
    typedef const value_type&                reference;

    /*using iterator_category = std::random_access_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = typename _vector::ValueType;
    using pointer           = const value_type*;
    using reference         = const value_type&;*/

public:
    ConstantIterator() noexcept : m_ptr() {}

    ConstantIterator(pointer ptr) noexcept
            : m_ptr(ptr) {}

    ConstantIterator(const ConstantIterator& other)
            : m_ptr(other.m_ptr) {}

    ConstantIterator& operator = (const ConstantIterator& other) {
        m_ptr = other.m_ptr;
        return *this;
    }

    virtual ~ConstantIterator() = default;

    ConstantIterator operator += (difference_type offset) {
        //
        m_ptr += offset;
        return *this;
    }

    ConstantIterator operator + (difference_type offset) const { //depend on +=
        ConstantIterator tmp = *this;
        return tmp += offset;
    }

    friend ConstantIterator operator + (difference_type offset, const ConstantIterator& iterator) {
        return iterator + offset;
    }

    ConstantIterator operator -= (difference_type offset) { //depend on +=
        return (*this) += -offset;
    }

    ConstantIterator operator - (difference_type offset) const { //depend on +=
        ConstantIterator tmp = *this;
        return tmp -= offset;
    }

    ConstantIterator& operator++() noexcept {
        //
        m_ptr++;
        return *this;
    }
    ConstantIterator operator++(int) noexcept { //depend on ++prefix
        auto temp = *this;
        ++(*this);
        return temp;
    }

    ConstantIterator& operator--() {
        //
        m_ptr--;
        return *this;
    }
    ConstantIterator operator--(int) { //depend on --prefix
        auto temp = *this;
        --(*this);
        return temp;
    }

    difference_type operator-(const ConstantIterator& other) const {
        //
        return m_ptr - other.m_ptr;
    }

    reference operator[] (size_t index) const { //depend on +=
        return *(*this + index);
    }
    reference operator* () const {
        //
        return *m_ptr;
    }
    pointer operator -> () const {
        //
        return m_ptr;
    }

    bool operator == (const ConstantIterator& other) const {
        //
        return m_ptr == other.m_ptr;
    }

    bool operator != (const ConstantIterator& other) const { //depends on ==
        return !(*this == other);
    }

    bool operator < (const ConstantIterator& other) const {
        //
        return m_ptr < other.m_ptr;
    }

    bool operator > (const ConstantIterator& other) const { //depends on <
        return other < *this;
    }

    bool operator <= (const ConstantIterator& other) const { //depends on <
        return !(other < *this);
    }

    bool operator >= (const ConstantIterator& other) const { //depends on <
        return !(*this < other);
    }

    friend void swap (ConstantIterator& a, ConstantIterator& b)
    noexcept(std::is_nothrow_move_constructible<value_type>::value && std::is_nothrow_move_assignable<value_type>::value) {
        swap(*a, *b);
    }

private:
    pointer m_ptr;
};

template<class _vector>
class VectorIterator : public ConstantIterator<_vector> {

public:

    typedef std::random_access_iterator_tag  iterator_category;
    typedef typename _vector::value_type     value_type;
    typedef ptrdiff_t                        difference_type;
    typedef value_type*                      pointer;
    typedef value_type&                      reference;

private:

    typedef ConstantIterator<_vector>        base;

    /*using iterator_category = std::random_access_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = typename _vector::ValueType;
    using pointer           = value_type*;
    using reference         = value_type&;*/

public:
    VectorIterator(pointer ptr)
            : base(ptr) {}

    VectorIterator(const VectorIterator& other)
            : base(other) {}

    VectorIterator& operator = (const VectorIterator& other) {
        base::operator=(other);
        return *this;
    }

    virtual ~VectorIterator() = default;

    VectorIterator operator += (difference_type offset) {
        base::operator+=(offset);
        return *this;
    }

    VectorIterator operator + (difference_type offset) const { //depend on +=
        VectorIterator tmp = *this;
        return tmp += offset;
    }

    friend VectorIterator operator + (difference_type offset, const VectorIterator& iterator) {
        return iterator + offset;
    }

    VectorIterator operator -= (difference_type offset) { //depend on +=
        base::operator-=(offset);
        return *this;
    }

    VectorIterator operator - (difference_type offset) const { //depend on +=
        VectorIterator tmp = *this;
        return tmp -= offset;
    }

    difference_type operator-(const VectorIterator& other) const {
        return base::operator-(other);
    }

    VectorIterator& operator++() {
        base::operator++();
        return *this;
    }
    VectorIterator operator++(int) { //depend on ++prefix
        auto temp = *this;
        base::operator++();
        return temp;
    }

    VectorIterator& operator--() {
        base::operator--();
        return *this;
    }
    VectorIterator operator--(int) { //depend on --prefix
        auto temp = *this;
        base::operator--();
        return temp;
    }

    reference operator[] (size_t index) const {
        return const_cast<reference>(base::operator[](index));
    }
    reference operator* () const {
        return const_cast<reference>(base::operator*());
    }
    pointer operator -> () const {
        return const_cast<pointer>(base::operator->());
    }

};

#endif //TEMPLATE_ITERATORS_H
