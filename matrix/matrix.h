#ifndef MATRIX_H
#define MATRIX_H

#include <iosfwd>
#include <cstdlib>
#include <iostream>
#include <cstddef>
#include <initializer_list>
#include <string>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <utility>
#include <vector>

template <class T> struct MatrixImpl
{
    T **m_ppT;
    size_t m_uRows;
    size_t m_uCols;
    size_t m_uRowsUsed;
    size_t m_uColsUsed;
    unsigned long m_uRefs;
    //
    explicit MatrixImpl(size_t uRows = 0, size_t uCols = 0);
    ~MatrixImpl();
    //
    void construct_matrix(const MatrixImpl& rcImpl, T **ppT);
    MatrixImpl(const MatrixImpl &);
    MatrixImpl<T>& operator=(const MatrixImpl &);
    //
    MatrixImpl(MatrixImpl &&);
    MatrixImpl<T>& operator=(MatrixImpl &&);
    //
    void swap(MatrixImpl &) throw();
    //
    template<class T1, class T2> void construct(T1 *pT1, T2&& rrT2)
    {
        new (pT1) T1(std::forward<T2>(rrT2));
    }
    void destroy_elem(T *pT)
    {
        pT->~T();
    }
    template <class FwdIter> void destroy(FwdIter first, FwdIter last)
    {
        while(first != last) { destroy_elem(&*first); ++first; }
    }
    //
    T** reserve_new_mem(size_t uRows, size_t uCols)
    {
        T **ppT = static_cast<T**>(operator new(uRows * sizeof(T*)));
        size_t uRows_reserved;
        try
        {
            for (size_t uRow = 0; uRow < uRows; uRow++)
            {
                uRows_reserved = uRow;
                ppT[uRow] = static_cast<T*>(operator new(uCols * sizeof(T)));
            }
        }
        catch(...)
        {
            memory_release(ppT, uRows_reserved); throw;
        }
        return ppT;
    }
    void memory_release(T **ppT, size_t uRows)
    {
        for (size_t uRow = 0; uRow < uRows; uRow++) operator delete(ppT[uRow]);
        operator delete(ppT); ppT = nullptr;
    }
    void destroy_this()
    {
        if(m_ppT == nullptr) return;
        else if(m_ppT != nullptr && (m_uRowsUsed == 0 || m_uColsUsed == 0))
            memory_release(m_ppT, m_uRows);
        else
        {
            for (size_t uRow = 0; uRow < m_uRowsUsed; uRow++)
                destroy(m_ppT[uRow], m_ppT[uRow] + m_uColsUsed);
            memory_release(m_ppT, m_uRows);
        }
    }   
};


template <class T> MatrixImpl <T>::MatrixImpl(size_t uRows, size_t uCols)
    : m_ppT(static_cast<T**>(
                (0 == uRows || 0 == uCols) ? nullptr : operator new(uRows * sizeof(T*)))),
      m_uRows(uRows), m_uCols(uCols), m_uRowsUsed(0), m_uColsUsed(0), m_uRefs(1)
{
    if(m_ppT != nullptr)
        for (size_t uRow = 0; uRow < uRows; uRow++)
            m_ppT[uRow] = static_cast<T*>(operator new(uCols * sizeof(T)));
}

template <class T>
void MatrixImpl <T>::construct_matrix(const MatrixImpl& rcImpl, T **ppT)
{
    // not constructed elem:
    size_t uRow_not_constructed;
    size_t uCol_not_constructed;
    //
    try
    {
        for(size_t uRow = 0; uRow < rcImpl.m_uRowsUsed; ++uRow)
            for(size_t uCol = 0; uCol < rcImpl.m_uColsUsed; ++uCol)
            {
                uRow_not_constructed = uRow;
                uCol_not_constructed = uCol;
                T& rElem = rcImpl.m_ppT[uRow][uCol];
                construct(&ppT[uRow][uCol], rElem); // exception
            }
    }
    catch(...)
    {
        // destruction just created elems:
        for(size_t uRow = 0; uRow < uRow_not_constructed; ++uRow)
            for(size_t uCol = 0; uCol < uCol_not_constructed; ++uCol)
                destroy_elem(&ppT[uRow][uCol]);
        //
        memory_release(ppT, rcImpl.m_uRows);
        throw;
    }
}

template <class T> MatrixImpl <T>::MatrixImpl(const MatrixImpl &rcImpl)
    : m_ppT(static_cast<T**>(
                (0 == rcImpl.m_uRows || 0 == rcImpl.m_uCols) ?
                    nullptr : operator new(rcImpl.m_uRows * sizeof(T*))
                    )),
      m_uRows(rcImpl.m_uRows), m_uCols(rcImpl.m_uCols),
      m_uRowsUsed(rcImpl.m_uRowsUsed), m_uColsUsed(rcImpl.m_uColsUsed),
      m_uRefs(1)
{
    for (size_t uRow = 0; uRow < rcImpl.m_uRows; uRow++)
        m_ppT[uRow] = static_cast<T*>(operator new(rcImpl.m_uCols * sizeof(T)));
    //
    construct_matrix(rcImpl, m_ppT);
}

template <class T> MatrixImpl<T>& MatrixImpl <T>::operator=(const MatrixImpl &rcImpl)
{
    if(this != &rcImpl)
    {
        m_ppT = (static_cast<T**>(
                     (0 == rcImpl.m_uRows || 0 == rcImpl.m_uCols) ?
                         nullptr : operator new(rcImpl.m_uRows * sizeof(T*))
                         ));
        m_uRows = rcImpl.m_uRows;
        m_uCols = rcImpl.m_uCols;
        m_uRowsUsed = rcImpl.m_uRowsUsed;
        m_uColsUsed = rcImpl.m_uColsUsed;
        m_uRefs = rcImpl.m_uRefs;
        //
        for (size_t uRow = 0; uRow < rcImpl.m_uRows; uRow++)
            m_ppT[uRow] = static_cast<T*>(operator new(rcImpl.m_uCols * sizeof(T)));
        //
        construct_matrix(rcImpl, m_ppT);
    }
    return *this;
}

template <class T> MatrixImpl <T>::MatrixImpl(MatrixImpl&& rrImpl)
{
    if (rrImpl.m_ppT != nullptr)
    {
        m_ppT = rrImpl.m_ppT;
        m_uRows = rrImpl.m_uRows;
        m_uCols = rrImpl.m_uCols;
        m_uRowsUsed = rrImpl.m_uRowsUsed;
        m_uColsUsed = rrImpl.m_uColsUsed;
        m_uRefs = rrImpl.m_uRefs;
        //
        rrImpl.m_ppT = nullptr;
        rrImpl.m_uRows = 0;
        rrImpl.m_uCols = 0;
        rrImpl.m_uRowsUsed = 0;
        rrImpl.m_uColsUsed = 0;
        rrImpl.m_uRefs = 0;
    }
}

template <class T> MatrixImpl<T>& MatrixImpl <T>::operator=(MatrixImpl &&rrImpl)
{
    if (rrImpl.m_ppT != nullptr && this != &rrImpl)
    {
        destroy_this();
        //
        m_ppT = rrImpl.m_ppT;
        m_uRows = rrImpl.m_uRows;
        m_uCols = rrImpl.m_uCols;
        m_uRowsUsed = rrImpl.m_uRowsUsed;
        m_uColsUsed = rrImpl.m_uColsUsed;
        m_uRefs = rrImpl.m_uRefs;
        //
        rrImpl.m_ppT = nullptr;
        rrImpl.m_uRows = 0;
        rrImpl.m_uCols = 0;
        rrImpl.m_uRowsUsed = 0;
        rrImpl.m_uColsUsed = 0;
        rrImpl.m_uRefs = 0;
    }
    return *this;
}

template <class T> MatrixImpl <T>::~MatrixImpl()
{
    // destroy elements and/or release memory:
    destroy_this();
}

template <class T>
void MatrixImpl <T>::swap(MatrixImpl &rImpl) throw()
{
    std::swap(m_ppT, rImpl.m_ppT);
    std::swap(m_uRows, rImpl.m_uRows);
    std::swap(m_uCols, rImpl.m_uCols);
    std::swap(m_uRowsUsed, rImpl.m_uRowsUsed);
    std::swap(m_uColsUsed, rImpl.m_uColsUsed);
    std::swap(m_uRefs, rImpl.m_uRefs);
}
//-------------------------------------------------------------------------------------------------
template <class T> class Matrix //: private MatrixImpl <T>
{
private:
    static const size_t unshareable = static_cast <size_t> (-1);
    MatrixImpl<T> *m_pImpl;
    //
    void deep_copy(bool bMarkUnshareable = false);
    //
    template <class T1, class ... Ts>
    void push_row_recursive(T **ppT, size_t uPushRow, size_t& uCol, T1&& t1, Ts&& ... ts);
    void push_row_recursive(T **ppT, size_t uPushRow, size_t& uCol);
    //
    template <class T1, class ... Ts>
    void push_column_recursive(T **ppT, size_t& uRow, size_t uPushCol,
                               std::vector<T*>& vecPushColumn, T1&& t1, Ts&& ... ts);
    void push_column_recursive(T **ppT, size_t& uRow, size_t uPushCol,
                               std::vector<T*>& vecPushColumn);
    //
public:
    explicit Matrix(size_t uRows = 0, size_t uCols = 0);
    ~Matrix();
    //
    Matrix(const Matrix& rcMatrix);
    Matrix<T>& operator = (const Matrix& rcMatrix);
    //
    Matrix(Matrix<T> &&rrMatrix);
    Matrix<T>& operator = (Matrix&& rrMatrix);
    //
    T& operator () (size_t uRow, size_t uCol);
    T operator () (size_t uRow, size_t uCol) const;
    //
    bool empty() const;
    T** pointer() const;
    size_t rows() const;
    size_t columns() const;
    size_t size() const;
    size_t rows_capacity() const;
    size_t columns_capacity() const;
    size_t capacity() const;
    size_t refs() const;
    static const size_t isUnshareable();
    //
    template <class ... Ts> void push_row(Ts&& ... ts);
    template <class ... Ts> void push_column(Ts&& ... ts);
    //
    void swap(Matrix<T> &rMatrix) throw();
    void reserve(size_t uRows, size_t uCols);
    //
    class const_iterator_cols
    {
        T *m_pT;
    public:
        explicit const_iterator_cols(T *pT = nullptr): m_pT(pT){}
        ~const_iterator_cols(){}
        const_iterator_cols(const const_iterator_cols& rci): m_pT(rci.m_pT) {}
        const_iterator_cols& operator=(const const_iterator_cols& rci)
        {
            if(this != &rci) m_pT = rci.m_pT; return *this;
        }
        T& operator*() const { return *m_pT; }
        T* operator->() const { return m_pT; }
        bool operator==(const const_iterator_cols &rci) const
        {
            return (m_pT == rci.m_pT) ? true : false;
        }
        bool operator!=(const const_iterator_cols &rci) const
        {
            return (m_pT != rci.m_pT) ? true : false;
        }
        const_iterator_cols &operator++() { ++m_pT; return *this; }
        const_iterator_cols operator++(int) { const_iterator_cols copy(*this); ++m_pT; return copy; }
        const_iterator_cols &operator--() { --m_pT; return *this; }
        const_iterator_cols operator--(int) { const_iterator_cols copy(*this); --m_pT; return copy; }
    };
    const_iterator_cols cbegin_row(size_t uRow = 0) const
    {
        return const_iterator_cols(m_pImpl->m_ppT[uRow]);
    }
    const_iterator_cols cend_row(size_t uRow = 0) const
    {
        return const_iterator_cols(m_pImpl->m_ppT[uRow] + m_pImpl->m_uColsUsed);
    }
    //
    class const_iterator_rows
    {
        T **m_ppT;
        size_t m_uCol;
    public:
        explicit const_iterator_rows(T **ppT = nullptr, size_t uCol = 0): m_ppT(ppT), m_uCol(uCol) {}
        ~const_iterator_rows(){}
        const_iterator_rows(const const_iterator_rows& rci): m_ppT(rci.m_ppT), m_uCol(rci.m_uCol) {}
        const_iterator_rows& operator=(const const_iterator_rows& rci)
        {
            if(this != &rci){ m_ppT = rci.m_ppT; m_uCol = rci.m_uCol; }
            return *this;
        }
        T& operator*() const
        {
            T *pT = *m_ppT;
            return *(pT + m_uCol);
        }
        T* operator->() const
        {
            T *pT = *m_ppT;
            return (pT + m_uCol);
        }
        bool operator==(const const_iterator_rows &rci) const
        {
            return (m_ppT == rci.m_ppT) ? true : false;
        }
        bool operator!=(const const_iterator_rows &rci) const
        {
            return (m_ppT != rci.m_ppT) ? true : false;
        }
        const_iterator_rows &operator++() { ++m_ppT; return *this; }
        const_iterator_rows operator++(int) { const_iterator_rows copy(*this); ++m_ppT; return copy; }
        const_iterator_rows &operator--() { --m_ppT; return *this; }
        const_iterator_rows operator--(int) { const_iterator_rows copy(*this); --m_ppT; return copy; }
    };
    const_iterator_rows cbegin_of_column(size_t uCol = 0) const
    {
        return const_iterator_rows(m_pImpl->m_ppT, uCol);
    }
    const_iterator_rows cend_of_column(size_t uCol = 0) const
    {
        return const_iterator_rows(m_pImpl->m_ppT + m_pImpl->m_uRowsUsed, uCol);
    }
    const_iterator_rows crow(size_t uRows = 0, size_t uCol = 0) const
    {
        return const_iterator_rows(m_pImpl->m_ppT + uRows, uCol);
    }
    //
    class iterator_rows
    {
        T **m_ppT;
    public:
        explicit iterator_rows(T **ppT = nullptr): m_ppT(ppT) {}
        ~iterator_rows(){}
        iterator_rows(const iterator_rows& rci): m_ppT(rci.m_ppT) {}
        iterator_rows& operator=(const iterator_rows& rci)
        {
            if(this != &rci){ m_ppT = rci.m_ppT; }
            return *this;
        }
        T& operator*() const
        {
            return **m_ppT;
        }
        T* operator->() const
        {
            return *m_ppT;
        }
        T** get() const
        {
            return m_ppT;
        }
        bool operator==(const iterator_rows &rci) const
        {
            return (m_ppT == rci.m_ppT) ? true : false;
        }
        bool operator!=(const iterator_rows &rci) const
        {
            return (m_ppT != rci.m_ppT) ? true : false;
        }
        iterator_rows &operator++() { ++m_ppT; return *this; }
        iterator_rows operator++(int) { iterator_rows copy(*this); ++m_ppT; return copy; }
        iterator_rows &operator--() { --m_ppT; return *this; }
        iterator_rows operator--(int) { iterator_rows copy(*this); --m_ppT; return copy; }
    };
    iterator_rows first_row()
    {
        return iterator_rows(m_pImpl->m_ppT);
    }
    iterator_rows last_row()
    {
        return iterator_rows(m_pImpl->m_ppT + m_pImpl->m_uRowsUsed);
    }
    //
    void erase_row(iterator_rows& i);
};
//-------------------------------------------------------------------------------------------------
template <class T> Matrix<T>::Matrix(size_t uRows, size_t uCols) // : MatrixImpl <T> (uRows, uCols),
    : m_pImpl(new MatrixImpl<T>(uRows, uCols))
{
    //
}

template <class T> Matrix<T>::~Matrix()
{
    if(m_pImpl != nullptr)
        if (-- m_pImpl->m_uRefs == 0)
            delete m_pImpl;
}

template <class T>
const size_t Matrix<T>::isUnshareable()
{
    return unshareable;
}

template <class T>
void Matrix<T>::deep_copy(bool bMarkUnshareable /* = false */)
{
    if (m_pImpl->m_uRefs > 1 && m_pImpl->m_uRefs != unshareable)
    {
        // Creating a copy of Impl for this object
        MatrixImpl<T> *pImpl = new MatrixImpl<T>(*m_pImpl);
        -- m_pImpl->m_uRefs;
        m_pImpl = pImpl;
    }
    else {} //reserve(uRows, uCols);
    //
    m_pImpl->m_uRefs = (bMarkUnshareable ? unshareable : 1);
}

template <class T> Matrix<T>::Matrix(const Matrix& rcMatrix)
{
    if(rcMatrix.m_pImpl->m_uRefs != unshareable)
    {
        m_pImpl = rcMatrix.m_pImpl;
        ++ m_pImpl->m_uRefs;
    }
    else
        m_pImpl = new MatrixImpl <T> (*rcMatrix.m_pImpl);
}

template <class T> Matrix<T>& Matrix<T>::operator = (const Matrix& rcMatrix)
{
    if(this != &rcMatrix)
    {
        if(rcMatrix.m_pImpl->m_uRefs != unshareable)
        {
            -- m_pImpl->m_uRefs;
            //
            m_pImpl = rcMatrix.m_pImpl;
            ++ m_pImpl->m_uRefs;
        }
        else
            m_pImpl = new MatrixImpl <T> (*rcMatrix.m_pImpl);
    }
    return *this;
}

template <class T> Matrix<T>::Matrix(Matrix<T>&& rrMatrix)
{
    if (rrMatrix.m_pImpl != nullptr)
    {
        m_pImpl = std::move(rrMatrix.m_pImpl);
        rrMatrix.m_pImpl = nullptr;
    }
}

template <class T> Matrix<T>& Matrix<T>::operator = (Matrix&& rrMatrix)
{
    if (rrMatrix.m_pImpl != nullptr && this != &rrMatrix)
    {
        bool bImplShared = false;
        if(m_pImpl == rrMatrix.m_pImpl)
            bImplShared = true;
        //
        m_pImpl = std::move(rrMatrix.m_pImpl);
        //
        if(bImplShared && m_pImpl->m_uRefs > 1 && m_pImpl->m_uRefs != unshareable)
            -- m_pImpl->m_uRefs;
        //
        rrMatrix.m_pImpl = nullptr;
    }
    return *this;
}

template <class T> T& Matrix<T>::operator() (size_t uRow, size_t uCol)
{
    deep_copy(true);
    return m_pImpl->m_ppT[uRow][uCol];
}

template <class T> T Matrix<T>::operator () (size_t uRow, size_t uCol) const
{
    return m_pImpl->m_ppT[uRow][uCol];
}
//-------------------------------------------------------------------------------------------------
template <class T> bool Matrix<T>::empty() const
{
    if(m_pImpl == nullptr) return true;
    if(m_pImpl->m_ppT == nullptr) return true;
    return false;
}
template <class T> T** Matrix<T>::pointer() const { return m_pImpl->m_ppT; }
template <class T> size_t Matrix<T>::rows() const { return m_pImpl->m_uRowsUsed; }
template <class T> size_t Matrix<T>::columns() const { return m_pImpl->m_uColsUsed; }
template <class T> size_t Matrix<T>::size() const { return m_pImpl->m_uRowsUsed * m_pImpl->m_uColsUsed; }
template <class T> size_t Matrix<T>::rows_capacity() const { return m_pImpl->m_uRows; }
template <class T> size_t Matrix<T>::columns_capacity() const { return m_pImpl->m_uCols; }
template <class T> size_t Matrix<T>::capacity() const { return m_pImpl->m_uRows * m_pImpl->m_uCols; }
template <class T> size_t Matrix<T>::refs() const { return m_pImpl->m_uRefs; }

template <class T> void Matrix<T>::swap(Matrix& rMatrix) throw()
{
    m_pImpl->swap(*rMatrix.m_pImpl);
}

template <class T> void Matrix<T>::reserve(size_t uRows, size_t uCols)
{
    if(m_pImpl->m_ppT == nullptr || (m_pImpl->m_uRows == 0 || m_pImpl->m_uCols == 0))
    {
        T** ppT = m_pImpl->reserve_new_mem(uRows, uCols);
        m_pImpl->m_ppT = ppT;
        m_pImpl->m_uRows = uRows;
        m_pImpl->m_uCols = uCols;
    }
    else
    {
        if(uRows == m_pImpl->m_uRows && uCols == m_pImpl->m_uCols)
            return;
        else if(uRows > m_pImpl->m_uRows && uCols == m_pImpl->m_uCols)
        {
            T **ppT = m_pImpl->reserve_new_mem(uRows, uCols);
            // copying pointers:
            for (size_t i = 0; i < m_pImpl->m_uRowsUsed; ++i)
                ppT[i] = m_pImpl->m_ppT[i];
            //
            m_pImpl->m_ppT = ppT;
            m_pImpl->m_uRows = uRows;
        }
        else if(uRows >= m_pImpl->m_uRows && uCols > m_pImpl->m_uCols)
        {
            T **ppT_Old = m_pImpl->m_ppT;
            size_t uRowsUsed_Old = m_pImpl->m_uRowsUsed;
            size_t uColsUsed_Old = m_pImpl->m_uColsUsed;
            //
            T **ppT = m_pImpl->reserve_new_mem(uRows, uCols);
            size_t uRow_not_constructed;
            size_t uCol_not_constructed;
            try
            {
                // copying elements:
                for(size_t uRow = 0; uRow < m_pImpl->m_uRowsUsed; ++uRow)
                    for(size_t uCol = 0; uCol < m_pImpl->m_uColsUsed; ++uCol)
                    {
                        uRow_not_constructed = uRow;
                        uCol_not_constructed = uCol;
                        T& rT = m_pImpl->m_ppT[uRow][uCol];
                        m_pImpl->construct(&ppT[uRow][uCol], rT); // exception
                    }
                //
                m_pImpl->m_ppT = ppT;
                m_pImpl->m_uRows = uRows;
                m_pImpl->m_uCols = uCols;
                //
                // destroy elements and release old memory:
                for(size_t uRow = 0; uRow < uRowsUsed_Old; ++uRow)
                    for(size_t uCol = 0; uCol < uColsUsed_Old; ++uCol)
                        m_pImpl->destroy_elem(&ppT_Old[uRow][uCol]);
                m_pImpl->memory_release(ppT, uRows);
            }
//            catch(const std::exception& )
//            {
//                // destruction just created elems:
//                for(size_t uRow = 0; uRow < uRow_not_constructed; ++uRow)
//                    for(size_t uCol = 0; uCol < uCol_not_constructed; ++uCol)
//                        m_pImpl->destroy_elem(&ppT[uRow][uCol]);
//                //
//                m_pImpl->memory_release(ppT, uRows);
//                throw;
//            }
            catch(...)
            {
                // destruction just created elems:
                for(size_t uRow = 0; uRow < uRow_not_constructed; ++uRow)
                    for(size_t uCol = 0; uCol < uCol_not_constructed; ++uCol)
                        m_pImpl->destroy_elem(&ppT[uRow][uCol]);
                m_pImpl->memory_release(ppT, uRows);
                throw;
            }
        }
        else if(uRows < m_pImpl->m_uRows && uCols == m_pImpl->m_uCols) return;
        else if(uRows == m_pImpl->m_uRows && uCols < m_pImpl->m_uCols) return;
        else if(uRows < m_pImpl->m_uRows && uCols > m_pImpl->m_uCols)  return;
        else if(uRows > m_pImpl->m_uRows && uCols < m_pImpl->m_uCols)  return;
        else if(uRows < m_pImpl->m_uRows && uCols < m_pImpl->m_uCols) return;
    }
}

template <class T> void Matrix<T>::erase_row(iterator_rows& i)
{
    T** ppT_erased = i.get();
    if(ppT_erased == nullptr) return;
    // destruction elems in row:
    m_pImpl->destroy(*ppT_erased, *ppT_erased + m_pImpl->m_uColsUsed);
    // copying pointers below erased pointer:
    T **ppT_last = m_pImpl->m_ppT + m_pImpl->m_uRows;
    T **ppT = ppT_erased;
    T **ppT_next = ++ppT_erased;
    while (ppT < ppT_last)
    {
        *ppT = *ppT_next;
        ++ppT; ++ppT_next;
    }
    -- m_pImpl->m_uRowsUsed;
    if(m_pImpl->m_uRowsUsed == 0) m_pImpl->m_uColsUsed = 0;
}
//-------------------------------------------------------------------------------------------------
template <class T>
template <class T1, class ... Ts>
void Matrix<T>::push_row_recursive(T **ppT, size_t uPushRow, size_t& uCol, T1&& t1, Ts&& ... ts)
{
    m_pImpl->construct(&ppT[uPushRow][uCol], t1); // exception
    ++uCol;
    push_row_recursive(ppT, uPushRow, uCol, ts...);
}

template <class T>
void Matrix<T>::push_row_recursive(T **ppT, size_t uPushRow, size_t& uCol) {}

template <class T>
template <class ... Ts> void Matrix<T>::push_row(Ts&& ... ts)
{
    if(m_pImpl->m_ppT == nullptr) reserve(1, sizeof...(Ts));
    if (sizeof...(Ts) < m_pImpl->m_uColsUsed) return;
    //
    deep_copy(); //(m_pImpl->m_uRowsUsed + 1, m_pImpl->m_uColsUsed);
    //
    T **ppT_old = m_pImpl->m_ppT;
    T **ppT_new{};
    T **ppT = ppT_old;
    //
    bool bNewMemoryReserved = false;
    size_t uNewRows = 0;
    size_t uOldRows = 0;
    //
    // auto reserve additional memory for rows:
    if(m_pImpl->m_uRowsUsed == m_pImpl->m_uRows)
    {
        // reserve current row count + 2 rows:
        uOldRows = m_pImpl->m_uRows;
        uNewRows = uOldRows + 2;
        ppT_new = m_pImpl->reserve_new_mem(uNewRows, m_pImpl->m_uCols);
        // copying pointers:
        for (size_t i = 0; i < m_pImpl->m_uRowsUsed; ++i)
            ppT_new[i] = ppT_old[i];
        //
        bNewMemoryReserved = true;
        ppT = ppT_new;
        m_pImpl->m_ppT = ppT_new;
        m_pImpl->m_uRows = uNewRows;
    }
    // push row:
    size_t uRow_to_push = m_pImpl->m_uRowsUsed;
    size_t uCol_not_constructed = 0;
    try
    {
        push_row_recursive(ppT, uRow_to_push, uCol_not_constructed, ts...);
        //
        ++ m_pImpl->m_uRowsUsed;
        //
        if(m_pImpl->m_uColsUsed == 0)
            m_pImpl->m_uColsUsed = m_pImpl->m_uCols;
    }
    catch(...)
    {
        if(bNewMemoryReserved)
        {
            // destruction just created elems in new memory:
            m_pImpl->destroy(ppT_new[uRow_to_push], ppT_new[uRow_to_push] + uCol_not_constructed);
            //
            // new memory release:
            for (size_t row = uOldRows; row < uNewRows; row++) operator delete(ppT_new[row]);
            operator delete(ppT_new);
            //
            m_pImpl->m_ppT = ppT_old;
            m_pImpl->m_uRows = uOldRows;
        }
        else
        {
            // destruction just created elems in old memory:
            m_pImpl->destroy(ppT_old[uRow_to_push], ppT_old[uRow_to_push] + uCol_not_constructed);
        }
        throw;
    }
}
//-------------------------------------------------------------------------------------------------
template <class T>
template <class T1, class ... Ts>
void Matrix<T>::push_column_recursive(T **ppT, size_t& uRow, size_t uPushCol,
                                      std::vector<T*>& vecPushColumn,
                                      T1&& t1, Ts&& ... ts)
{
    T *pT = &ppT[uRow][uPushCol];
    m_pImpl->construct(&ppT[uRow][uPushCol], t1); // exception
    vecPushColumn.push_back(pT);
    ++uRow;
    push_column_recursive(ppT, uRow, uPushCol, vecPushColumn, ts...);
}

template <class T>
void Matrix<T>::push_column_recursive(T **ppT, size_t &uRow, size_t uPushCol,
                                      std::vector<T*>& vecPushColumn) {}

template <class T>
template <class ... Ts>
void Matrix<T>::push_column(Ts&& ... ts)
{
    if(m_pImpl->m_ppT == nullptr) reserve(sizeof...(Ts), 1);
    if (sizeof...(Ts) < m_pImpl->m_uRowsUsed) return;
    //
    deep_copy();
    //
    // for pointers of column elements (for exception case):
    std::vector<T*> vecPushColumn;
    vecPushColumn.reserve(m_pImpl->m_uColsUsed);
    //
    T **ppT_old = m_pImpl->m_ppT;
    T **ppT_new{};
    T **ppT = ppT_old;
    //
    bool bNewMatrixReserved = false;
    size_t uCols_New = 0;
    size_t uCols_Old = 0;
    size_t uColsUsed_Old = m_pImpl->m_uColsUsed;
    // auto reserve memory for all matrix:
    if (m_pImpl->m_uColsUsed == m_pImpl->m_uCols)
    {
        // reserve current column count + 2 columns:
        uCols_Old = m_pImpl->m_uCols;
        uCols_New = uCols_Old + 2;
        //
        ppT_new = m_pImpl->reserve_new_mem(m_pImpl->m_uRows, uCols_New);
        // not constructed elem:
        size_t uRow_not_constructed;
        size_t uCol_not_constructed;
        //
        // copying all elems in new memory:
        try
        {
            for(size_t uRow = 0; uRow < m_pImpl->m_uRowsUsed; ++uRow)
                for(size_t uCol = 0; uCol < m_pImpl->m_uColsUsed; ++uCol)
                {
                    uRow_not_constructed = uRow;
                    uCol_not_constructed = uCol;
                    T& elem = m_pImpl->m_ppT[uRow][uCol];
                    m_pImpl->construct(&ppT_new[uRow][uCol], elem);
                }
        }
        catch(...)
        {
            // destruction just created elems:
            for(size_t uRow = 0; uRow < uRow_not_constructed; ++uRow)
                for(size_t uCol = 0; uCol < uCol_not_constructed; ++uCol)
                    m_pImpl->destroy_elem(&ppT_new[uRow][uCol]);
            m_pImpl->memory_release(ppT_new, m_pImpl->m_uRows);
            throw;
        }
        //
        bNewMatrixReserved = true;
        ppT = ppT_new;
    }
    // push column:
    size_t uCol_to_push = m_pImpl->m_uColsUsed;
    size_t uRow = 0;
    //
    try
    {
        push_column_recursive(ppT, uRow, uCol_to_push, vecPushColumn, ts...);
        //
        ++ m_pImpl->m_uColsUsed;
        m_pImpl->m_ppT = ppT;
        //
        if(m_pImpl->m_uRowsUsed == 0)
            m_pImpl->m_uRowsUsed = m_pImpl->m_uRows;
        //
        // if memory is new - release old memory:
        if(bNewMatrixReserved)
        {
            // destruction old elems in old matrix:
            for (size_t uRow = 0; uRow < m_pImpl->m_uRowsUsed; uRow++)
                m_pImpl->destroy(ppT_old[uRow], ppT_old[uRow] + uColsUsed_Old);
            //
            // release memory for old matrix:
            m_pImpl->memory_release(ppT_old, m_pImpl->m_uRows);
            //
            m_pImpl->m_uCols = uCols_New;
        }       
    }
    catch(...)
    {
        if(bNewMatrixReserved) // new memory reserved
        {
            // destruction just created elems in column in new memory:
            for(T *pT : vecPushColumn) m_pImpl->destroy_elem(pT);

            // destruction old elems in new matrix:
            for (size_t uRow = 0; uRow < m_pImpl->m_uRowsUsed; uRow++)
                m_pImpl->destroy(ppT_new[uRow], ppT_new[uRow] + m_pImpl->m_uColsUsed);

            // release memory for new matrix:
            m_pImpl->memory_release(ppT_new, m_pImpl->m_uRows);
            //
            m_pImpl->m_ppT = ppT_old;
        }
        else // new memory not reserved
        {
            // destruction just created elems in column in old memory:
            for(T *pT : vecPushColumn) m_pImpl->destroy_elem(pT);
        }
        throw;
    }
}
//-------------------------------------------------------------------------------------------------

#endif // MATRIX_H
