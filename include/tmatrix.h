#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Динамический вектор - 
// шаблонный вектор на динамической памяти
template<typename T>
class TDynamicVector
{
protected:
  size_t sz;
  T* pMem;
public:
  TDynamicVector(size_t size = 1) : sz(size)
  {
    if (sz == 0)
      throw out_of_range("Vector size should be greater than zero");
    pMem = new T[sz]();// {}; // У типа T д.б. констуктор по умолчанию
  }
  TDynamicVector(T* arr, size_t s) : sz(s)
  {
    assert(arr != nullptr && "TDynamicVector ctor requires non-nullptr arg");
    pMem = new T[sz];
    std::copy(arr, arr + sz, pMem);
  }
  TDynamicVector(const TDynamicVector& v)
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
      if (this == &v) return *this;  // Проверка на самоприсваивание
      TDynamicVector temp(v);
      swap(*this, temp);
      return *this;
  }
  TDynamicVector& operator=(TDynamicVector&& v) noexcept
  {
      if (this == &v) return *this;
      delete[] pMem;
      sz = v.sz;
      pMem = v.pMem;
      v.sz = 0;
      v.pMem = nullptr;
      return *this;
  }

  size_t size() const noexcept { return sz; }

  // индексация
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
  // индексация с контролем
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

  // сравнение
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

  // скалярные операции
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

  // векторные операции
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

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicVector& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      istr >> v.pMem[i]; // требуется оператор>> для типа T
    return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicVector& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      ostr << v.pMem[i] << ' '; // требуется оператор<< для типа T
    return ostr;
  }
};


// Динамическая матрица - 
// шаблонная матрица на динамической памяти
template<typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>>
{
  using TDynamicVector<TDynamicVector<T>>::pMem;
  using TDynamicVector<TDynamicVector<T>>::sz;
public:
  TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(s)
  {
      if (s == 0)
          throw out_of_range("Matrix size must be greater than zero");
      for (size_t i = 0; i < sz; ++i)
          pMem[i] = TDynamicVector<T>(sz);
  }

  using TDynamicVector<TDynamicVector<T>>::operator[];

  // сравнение
  bool operator==(const TDynamicMatrix& m) const noexcept
  {
      return TDynamicVector<TDynamicVector<T>>::operator==(m);
  }

  bool operator!=(const TDynamicMatrix& m) const 
  {
      return !(*this == m); // Используем оператор ==
  }

  // матрично-скалярные операции
  TDynamicMatrix operator*(const T& val)
  {
      TDynamicMatrix result(sz);
      for (size_t i = 0; i < sz; ++i)
          result[i] = pMem[i] * val;
      return result;
  }

  // матрично-векторные операции
  TDynamicVector<T> operator*(const TDynamicVector<T>& v)
  {
      if (sz != v.size())
          throw out_of_range("Matrix and vector dimensions must match");
      TDynamicVector<T> result(sz);
      for (size_t i = 0; i < sz; ++i)
          result[i] = pMem[i] * v;
      return result;
  }

  // матрично-матричные операции
  TDynamicMatrix operator+(const TDynamicMatrix& m)
  {
      if (sz != m.sz)
          throw out_of_range("Matrices must have the same dimensions");
      TDynamicMatrix result(sz);
      for (size_t i = 0; i < sz; ++i)
          result[i] = pMem[i] + m[i]; // Используем сложение векторов
      return result;
  }
  TDynamicMatrix operator-(const TDynamicMatrix& m)
  {
      if (sz != m.sz)
          throw out_of_range("Matrices must have the same dimensions");
      TDynamicMatrix result(sz);
      for (size_t i = 0; i < sz; ++i)
          result[i] = pMem[i] - m[i]; // Используем вычитание векторов
      return result;
  }
  TDynamicMatrix operator*(const TDynamicMatrix& m)
  {
      if (sz != m.sz)
          throw out_of_range("Matrices must have the same dimensions");
      TDynamicMatrix result(sz);
      for (size_t i = 0; i < sz; ++i) {
          for (size_t j = 0; j < sz; ++j) {
              result[i][j] = T();
              for (size_t k = 0; k < sz; ++k)
                  result[i][j] += pMem[i][k] * m[k][j];
          }
      }
      return result;
  }

  // ввод/вывод
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

  // Публичный метод для безопасного доступа к элементам
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
