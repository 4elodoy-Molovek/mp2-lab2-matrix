#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>
#include <stdexcept> // Для исключений
#include <cassert>   // Для assert
#include <algorithm> // Для std::copy и std::swap

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

template<typename T>
class TDynamicVector
{
protected:
    size_t sz;
    T* pMem;
public:
    TDynamicVector(size_t size = 1) : sz(size)
    {
        if (sz > MAX_VECTOR_SIZE)
            throw out_of_range("Vector size is too large");
        if (sz == 0)
            throw out_of_range("Vector size should be greater than zero");
        pMem = new T[sz]();
    }

    TDynamicVector(T* arr, size_t s) : sz(s)
    {
        assert(arr != nullptr && "TDynamicVector ctor requires non-nullptr arg");
        pMem = new T[sz];
        std::copy(arr, arr + sz, pMem);
    }

    TDynamicVector(const TDynamicVector& v) : sz(v.sz)
    {
        pMem = new T[sz];
        std::copy(v.pMem, v.pMem + sz, pMem);
    }

    TDynamicVector(TDynamicVector&& v) noexcept : sz(v.sz), pMem(v.pMem)
    {
        v.sz = 0;
        v.pMem = nullptr;
    }

    ~TDynamicVector()
    {
        delete[] pMem;
    }

    TDynamicVector& operator=(const TDynamicVector& v)
    {
        if (this != &v) 
        {
            delete[] pMem; // Очистка памяти перед присваиванием
            sz = v.sz;
            pMem = new T[sz];
            std::copy(v.pMem, v.pMem + sz, pMem);
        }
        return *this;
    }

    TDynamicVector& operator=(TDynamicVector&& v) noexcept
    {
        if (this != &v) 
        {
            delete[] pMem; // Освобождение текущих ресурсов
            sz = v.sz;
            pMem = v.pMem;
            v.sz = 0;
            v.pMem = nullptr;
        }
        return *this;
    }

    size_t size() const noexcept { return sz; }

    T& operator[](size_t ind)
    {
        if (ind >= sz)
            throw out_of_range("Index out of range");
        return pMem[ind];
    }

    const T& operator[](size_t ind) const
    {
        if (ind >= sz)
            throw out_of_range("Index out of range");
        return pMem[ind];
    }

    T& at(size_t ind)
    {
        if (ind >= sz)
            throw out_of_range("Index out of range");
        return pMem[ind];
    }

    const T& at(size_t ind) const
    {
        if (ind >= sz)
            throw out_of_range("Index out of range");
        return pMem[ind];
    }

    bool operator==(const TDynamicVector& v) const noexcept
    {
        if (sz != v.sz) return false;
        for (size_t i = 0; i < sz; ++i)
            if (pMem[i] != v.pMem[i]) return false;
        return true;
    }

    bool operator!=(const TDynamicVector& v) const noexcept
    {
        return !(*this == v);
    }

    TDynamicVector operator+(T val)
    {
        TDynamicVector result(sz);
        for (size_t i = 0; i < sz; ++i)
            result.pMem[i] = pMem[i] + val;
        return result;
    }

    TDynamicVector operator-(T val)
    {
        TDynamicVector result(sz);
        for (size_t i = 0; i < sz; ++i)
            result.pMem[i] = pMem[i] - val;
        return result;
    }

    TDynamicVector operator*(T val)
    {
        TDynamicVector result(sz);
        for (size_t i = 0; i < sz; ++i)
            result.pMem[i] = pMem[i] * val;
        return result;
    }

    TDynamicVector operator+(const TDynamicVector& v)
    {
        if (sz != v.sz)
            throw out_of_range("Vectors must be of the same size");
        TDynamicVector result(sz);
        for (size_t i = 0; i < sz; ++i)
            result.pMem[i] = pMem[i] + v.pMem[i];
        return result;
    }

    TDynamicVector operator-(const TDynamicVector& v)
    {
        if (sz != v.sz)
            throw out_of_range("Vectors must be of the same size");
        TDynamicVector result(sz);
        for (size_t i = 0; i < sz; ++i)
            result.pMem[i] = pMem[i] - v.pMem[i];
        return result;
    }

    T operator*(const TDynamicVector& v) noexcept(noexcept(T()))
    {
        if (sz != v.sz)
            throw out_of_range("Vectors must be of the same size");
        T result = T();
        for (size_t i = 0; i < sz; ++i)
            result += pMem[i] * v.pMem[i];
        return result;
    }

    friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept
    {
        std::swap(lhs.sz, rhs.sz);
        std::swap(lhs.pMem, rhs.pMem);
    }

    friend istream& operator>>(istream& istr, TDynamicVector& v)
    {
        for (size_t i = 0; i < v.sz; i++)
            istr >> v.pMem[i];
        return istr;
    }

    friend ostream& operator<<(ostream& ostr, const TDynamicVector& v)
    {
        for (size_t i = 0; i < v.sz; i++)
            ostr << v.pMem[i] << ' ';
        return ostr;
    }
};

template<typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>>
{
    using TDynamicVector<TDynamicVector<T>>::pMem;
    using TDynamicVector<TDynamicVector<T>>::sz;
public:
    TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(s)
    {
        if (s > MAX_MATRIX_SIZE)
            throw out_of_range("Matrix size is too large");
        if (s == 0)
            throw out_of_range("Matrix size must be greater than zero");
        for (size_t i = 0; i < sz; ++i)
            pMem[i] = TDynamicVector<T>(sz);
    }

    using TDynamicVector<TDynamicVector<T>>::operator[];

    bool operator==(const TDynamicMatrix& m) const noexcept
    {
        return TDynamicVector<TDynamicVector<T>>::operator==(m);
    }

    bool operator!=(const TDynamicMatrix& m) const
    {
        return !(*this == m);
    }

    TDynamicMatrix operator*(const T& val)
    {
        TDynamicMatrix result(sz);
        for (size_t i = 0; i < sz; ++i)
            result[i] = pMem[i] * val;
        return result;
    }

    TDynamicVector<T> operator*(const TDynamicVector<T>& v)
    {
        if (sz != v.size())
            throw out_of_range("Matrix and vector dimensions must match");
        TDynamicVector<T> result(sz);
        for (size_t i = 0; i < sz; ++i)
            result[i] = pMem[i] * v;
        return result;
    }

    TDynamicMatrix operator+(const TDynamicMatrix& m)
    {
        if (sz != m.sz)
            throw std::invalid_argument("Matrices must have the same size for addition.");

        TDynamicMatrix<T> result(sz);
        for (size_t i = 0; i < sz; ++i) 
        {
            result[i] = pMem[i] + m.pMem[i];
        }

        return result;
    }

    TDynamicMatrix operator-(const TDynamicMatrix& m)
    {
        if (sz != m.sz)
            throw out_of_range("Matrices must have the same dimensions");
        TDynamicMatrix result(sz);
        for (size_t i = 0; i < sz; ++i)
            result[i] = pMem[i] - m[i];
        return result;
    }

    TDynamicMatrix operator*(const TDynamicMatrix& m)
    {
        if (sz != m.sz)
            throw std::invalid_argument("Matrices must have the same size for multiplication.");

        TDynamicMatrix<T> result(sz);
        for (size_t i = 0; i < sz; ++i) 
        {
            for (size_t j = 0; j < sz; ++j)
            {
                result[i][j] = 0; // Инициализируем элемент результата
                for (size_t k = 0; k < sz; ++k) 
                {
                    result[i][j] += (*this)[i][k] * m[k][j];
                }
            }
        }

        return result;
    }

    friend istream& operator>>(istream& istr, TDynamicMatrix& m)
    {
        for (size_t i = 0; i < m.sz; ++i)
            istr >> m.pMem[i];
        return istr;
    }

    friend ostream& operator<<(ostream& ostr, const TDynamicMatrix& m)
    {
        for (size_t i = 0; i < m.sz; ++i)
            ostr << m.pMem[i] << '\n';
        return ostr;
    }

    size_t size() const
    {
        return TDynamicVector<TDynamicVector<T>>::size();
    }

    TDynamicVector<T>& at(size_t index)
    {
        return TDynamicVector<TDynamicVector<T>>::at(index);
    }

    const TDynamicVector<T>& at(size_t index) const
    {
        return TDynamicVector<TDynamicVector<T>>::at(index);
    }
};

#endif
