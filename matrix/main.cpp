#include <iostream>
//#include <iterator>    // ostream_iterator <>
#include <exception>
#include <iomanip>
#include <string>      // to_string()
#include <list>
//#include <type_traits>

#include "matrix.h"
using namespace std;

template <class T>
void matrixParamsOutput(const Matrix<T>& rcMatrix)
{
    if(rcMatrix.empty()) { cout << " matrix empty"; return; }
    //
    cout << "Rows = " << rcMatrix.rows()
         << "; Columns = " << rcMatrix.columns()
         << "; Size = " << rcMatrix.size()
         << "; Capacity = " << rcMatrix.capacity() << endl;
}

template <class T>
void matrixOutput(const Matrix<T>& rcMatrix)
{
    if(rcMatrix.empty()) { cout << " matrix empty"; return; }
    //
    if (rcMatrix.refs() == Matrix<T>::isUnshareable())
        cout << 'U';
    else
        cout << rcMatrix.refs();
    //
    cout << ": " << endl;
    //
    T **ppT = rcMatrix.pointer();
    size_t uRows = rcMatrix.rows_capacity();
    size_t uCols = rcMatrix.columns_capacity();
    size_t uRowsUsed = rcMatrix.rows();
    size_t uColsUsed = rcMatrix.columns();

    if (ppT == nullptr || (uRows == 0 || uCols == 0))
    {
        cout << " null" << endl;
    }
    else if(ppT != nullptr && (uRows != 0 && uCols != 0) && (uRowsUsed == 0 && uColsUsed == 0))
    {
        cout << " empty; reserved: " << uRows << " rows; "
             << uCols << " columns. " << endl;
    }
    else
    {
        for (size_t row = 0; row < uRowsUsed; row++) {
            for (size_t col = 0; col < uColsUsed; col++)
                cout << std::setw(3) << ppT[row][col];
            cout << endl;
        }
    }
}

template <class T>
ostream &operator << (ostream &rStream, const Matrix<T>& rcMatrix)
{
    if(rcMatrix.empty())
    {
        rStream << " :  " << "null" << endl;
        return rStream;
    }
    //
    if (rcMatrix.refs() == Matrix<T>::isUnshareable())
        rStream << 'U';
    else
        rStream << rcMatrix.refs();
    //
    rStream << ":";
    // 
    T **ppT = rcMatrix.pointer();
    size_t uRows = rcMatrix.rows_capacity();
    size_t uCols = rcMatrix.columns_capacity();
    size_t uRowsUsed = rcMatrix.rows();
    size_t uColsUsed = rcMatrix.columns();
    //
    if(ppT != nullptr && (uRows != 0 && uCols != 0) && (uRowsUsed == 0 && uColsUsed == 0))
    {
        rStream << " empty; reserved: " << uRows << " rows; "
                << uCols << " columns. " << endl;
    }
    else
    {
        for (size_t uRow = 0; uRow < uRowsUsed; uRow++)
        {
            for (size_t uCol = 0; uCol < uColsUsed; uCol++)
            {
                if(uRow == 0)
                    cout << std::setw(3);
                else
                {
                    if(uCol == 0)
                        cout << std::setw(5);
                    else
                        cout << std::setw(3);
                }
                cout << ppT[uRow][uCol];
            }
            rStream << endl;
        }
    }
    //
    return rStream;
}

void test_push_row()
{
    cout << "-------------------------------------------------------------------" << endl;
    cout << " TEST push_row() - integers: " << endl;

    Matrix<int> m(1, 3);
    cout << endl << "Created empty matrix(1,3): " << endl;
    matrixOutput(m); matrixParamsOutput(m);

    cout << endl << "push_row 1,2,3,4: " << endl;
    m.push_row(1, 2, 3);
    matrixOutput(m); matrixParamsOutput(m);

    cout << endl << "push_row 1,2: " << endl;
    m.push_row(1, 2);
    matrixOutput(m); matrixParamsOutput(m);

    cout << endl << "push_row 4,5,6,7,8: " << endl;
    m.push_row(4, 5, 6, 7, 8);
    matrixOutput(m); matrixParamsOutput(m);

    cout << endl << "push_row 7,8,9: " << endl;
    m.push_row(7, 8, 9);
    matrixOutput(m); matrixParamsOutput(m);

    cout << endl << "push_row 10,11,12,13: " << endl;
    m.push_row(10, 11, 12, 13);
    matrixOutput(m); matrixParamsOutput(m);

    /*
    cout << "-------------------------------------------------------------------" << endl;

    cout << " TEST push_row() - std::string: " << endl;

    string a1("a1"), a2("a2"), a3("a3"), a4("a4"), a5("a5");
    string b1("b1"), b2("b2"), b3("b3"), b4("b4"), b5("b5");

    Matrix<string> ml(2, 2);
    cout << endl << "Created empty matrix: " << endl;
    cout << "Elems before push(const T&): " << a1 << ' ' << a2 << ' ' << b1 << ' ' << b2 << endl;
    ml.push_row(a1, a2);
    ml.push_row(b1, b2);
    matrixOutput(ml); matrixParamsOutput(ml);
    cout << "Elems after push(const T&): " << a1 << ' ' << a2 << ' ' << b1 << ' ' << b2 << endl;

    Matrix<string> mr(2, 2);
    cout << endl << "Created empty matrix: " << endl;
    cout << "Elems before push(T&&): " << a1 << ' ' << a2 << ' ' << b1 << ' ' << b2 << endl;
    mr.push_row(std::move(a1), std::move(a2));
    mr.push_row(std::move(b1), std::move(b2));
    matrixOutput(mr); matrixParamsOutput(mr);
    cout << "Elems after push(T&&): " << a1 << ' ' << a2 << ' ' << b1 << ' ' << b2 << endl;
    */
}
void test_push_column()
{
    cout << "-------------------------------------------------------------------" << endl;
    cout << " TEST push_column(), push_row(): " << endl;

    Matrix<int> w(3,1);
    cout << endl << "Created empty matrix(3,1): " << endl;
    matrixOutput(w); matrixParamsOutput(w);

    cout << endl << "push_column 1,2,3: " << endl;
    w.push_column(1,2,3);
    matrixOutput(w); matrixParamsOutput(w);

    cout << endl << "push_column 1,2: " << endl;
    w.push_column(1,2);
    matrixOutput(w); matrixParamsOutput(w);

    cout << endl << "push_column 4,5,6: " << endl;
    w.push_column(4,5,6);
    matrixOutput(w); matrixParamsOutput(w);

    cout << endl << "push_column 9,10,11: " << endl;
    w.push_column(9,10,11);
    matrixOutput(w); matrixParamsOutput(w);

    cout << endl << "push_row 12,13,14: " << endl;
    w.push_row(12,13,14);
    matrixOutput(w); matrixParamsOutput(w);

    cout << endl << "push_column 15,16,17,18: " << endl;
    w.push_column(15,16,17,18,19);
    matrixOutput(w); matrixParamsOutput(w);

    cout << endl << "push_row 19,20,21,22,23: " << endl;
    w.push_row(19,20,21,22,23);
    matrixOutput(w); matrixParamsOutput(w);

    cout << endl << "push_column 23,24,25,26,27: " << endl;
    w.push_column(23,24,25,26,27);
    matrixOutput(w); matrixParamsOutput(w);

    cout << endl << "push_row 1,2,3,4,5: " << endl;
    w.push_row(1,2,3,4,5);
    matrixOutput(w); matrixParamsOutput(w);
}
void test_swap()
{
    cout << "-------------------------------------------------------------------" << endl;
    cout << "TEST swap(): " << endl;
    cout << endl << "Matrixes() before swap: " << endl;

    Matrix<int> s1(1,3);
    s1.push_row(1,1,1); s1.push_row(2,2,2); s1.push_row(3,3,3);
    matrixOutput(s1); matrixParamsOutput(s1);
    Matrix<int> s2(1,3);
    s2.push_row(4,4,4); s2.push_row(5,5,5); s2.push_row(6,6,6);
    matrixOutput(s2); matrixParamsOutput(s2);

    cout << endl << "Matrixes() after swap: " << endl;
    s1.swap(s2);
    matrixOutput(s1); matrixParamsOutput(s1);
    matrixOutput(s2); matrixParamsOutput(s2);
}
void test_iterators()
{
    cout << "-------------------------------------------------------------------" << endl;
    cout << " TEST const_iterator_c() - columns iteration (along row): " << endl;

    cout << "Matrix for iteration: " << endl;
    Matrix<int> mi(1, 3);
    for(int i = 2; i < 11; i += 3) mi.push_row(i, i-1, i+2);
    matrixOutput(mi); matrixParamsOutput(mi);

    Matrix<int>::const_iterator_cols itr;
    Matrix<int>::const_iterator_cols end_row;
    Matrix<int>::const_iterator_cols begin_row;
    size_t uRows = mi.rows();
    cout << "Columns iteration prefix ++ (from first row/column to last row/column): " << endl;
    for(size_t uRow = 0; uRow < uRows; ++uRow)
    {
        end_row = mi.cend_row(uRow);
        for(itr = mi.cbegin_row(uRow); itr != end_row; ++itr)
            std::cout << std::setw(3) << *itr;
    }
    std::cout << std::endl;
    cout << "Columns iteration prefix -- (from last row/column to first row/column): " << endl;
    for(size_t uRow = uRows - 1; uRow >= 0 ; --uRow)
    {
        begin_row = mi.cbegin_row(uRow); --begin_row;
        itr = mi.cend_row(uRow); --itr;
        for(;itr != begin_row; --itr) std::cout << std::setw(3) << *itr;
        if(uRow == 0) break;
    }
    std::cout << std::endl;
    cout << "-------------------------------------------------------------------" << endl;
    cout << " TEST const_iterator_r() - rows iteration (along column): " << endl;

    Matrix<int>::const_iterator_rows it;
    Matrix<int>::const_iterator_rows first_row;
    Matrix<int>::const_iterator_rows last_row;
    cout << "Rows iteration prefix ++ (from first column/row to last column/row): " << endl;
    size_t uCols = mi.columns();
    for(size_t uCol = 0; uCol < uCols; ++uCol)
    {
        first_row = mi.crow(0, uCol);
        last_row = mi.crow(uRows, uCol);
        for(it = first_row; it != last_row; ++it)
        {
            std::cout << std::setw(3) << *it;
            //std::cout << std::setw(3) << *(it.operator ->());
        }
    }
    std::cout << std::endl;
    cout << "-------------------------------------------------------------------" << endl;
    cout << " TEST iterator_r() - rows iteration and erase_row(): " << endl;

    Matrix<int>::iterator_rows i_row;
    Matrix<int>::iterator_rows firstRow = mi.first_row();
    Matrix<int>::iterator_rows lastRow = mi.last_row();
    //++firstRow;
    for(i_row = firstRow; i_row != lastRow; i_row = mi.first_row())
    {
        std::cout << "first elem of erased row: " << *i_row << std::endl;

        mi.erase_row(i_row);
        lastRow = mi.last_row();

        std::cout << "show matrix: " << std::endl;
        matrixOutput(mi); matrixParamsOutput(mi);
    }
}
void test_reserve()
{
    cout << "-------------------------------------------------------------------" << endl;
    cout << "TEST reserve(): " << endl;

    cout << endl << "Created empty matrix: " << endl;
    Matrix<string> m;
    matrixOutput(m); matrixParamsOutput(m);

    cout << endl << "reserve(2,0): " << endl;
    m.reserve(2, 0);
    matrixOutput(m); matrixParamsOutput(m);

    cout << endl << "reserve(2,3): " << endl;
    m.reserve(2, 3);
    matrixOutput(m); matrixParamsOutput(m);

    cout << endl << "push 2 rows: " << endl;
    string r1("r1"), r2("r2"), r3("r3");
    string v1("v1"), v2("v2"), v3("v3");
    m.push_row(r1, r2, r3);
    m.push_row(v1, v2, v3);
    matrixOutput(m); matrixParamsOutput(m);

    cout << endl << "reserve(3,3): " << endl;
    m.reserve(3, 3);
    matrixOutput(m); matrixParamsOutput(m);

    cout << endl << "reserve(2,2): " << endl;
    m.reserve(2, 2);
    matrixOutput(m); matrixParamsOutput(m);
}

struct Test
{
    int m_n;
    int m_nCheck;
    bool m_bThrowInMove, m_bThrowLater;
    static bool ms_bThrowDeferred;
    std::list <double> m_List;
    //
    Test(int n, bool bThrowInMove = false, bool bThrowLater = false)
        : m_n(n),
          m_nCheck(1),
          m_bThrowInMove(bThrowInMove),
          m_bThrowLater(bThrowLater),
          m_List(100, 1.)
    {
        cout << "Test(" << setw(3) << n << ")" << endl;
    }
    //
    Test(const Test &rcTest)
        : m_n(rcTest.m_n),
          m_nCheck(rcTest.m_nCheck),
          m_bThrowInMove(rcTest.m_bThrowInMove),
          m_bThrowLater(rcTest.m_bThrowLater),
          m_List(rcTest.m_List)
    {
        cout << "Test(const Test &rcTest), rcTest == " << rcTest << endl;
        if (m_bThrowInMove)
        {
            cout << "Throwing..." << endl;
            throw std::logic_error(string("Test(") + to_string(m_n) + ")");
        }
        if (m_bThrowLater && ms_bThrowDeferred)
        {
            cout << "Throwing..." << endl;
            throw std::logic_error("Test(const Test &)");
        }
    }
    //
    friend std::ostream &operator << (std::ostream &rStream, const Test &rcTest)
    {
        if (rcTest.m_nCheck == 1)
            rStream << setw(3) << rcTest.m_n;
        else
            rStream << "invalid";
        //
        return rStream;
    }
    //
    Test(Test &&) = delete;
    Test &operator = (const Test &) = delete;
    Test &operator = (Test &&) = delete;
};

bool Test::ms_bThrowDeferred = false;

void output_columns(Matrix<Test>::const_iterator_cols cbegin_of_row,
                    Matrix<Test>::const_iterator_cols cend_of_row,
                    const Matrix<Test>& rcMatrix)
{
    matrixParamsOutput(rcMatrix);
    cout << "Content:\n";

    Matrix<Test>::const_iterator_cols it = cbegin_of_row;
    for(; it != cend_of_row; ++it)
        cout << std::setw(3) << *it;
    cout << endl;

    size_t uRows = rcMatrix.rows();
    Matrix<Test>::const_iterator_cols end_row;
    for(size_t uRow = 1; uRow < uRows; ++uRow)
    {
        end_row = rcMatrix.cend_row(uRow);
        for(it = rcMatrix.cbegin_row(uRow); it != end_row; ++it)
            cout << std::setw(3) << *it;
        cout << endl;
    }
    cout << endl;
}

void output_rows(Matrix<Test>::const_iterator_rows cbegin_of_column,
                 Matrix<Test>::const_iterator_rows cend_of_column,
                 const Matrix<Test>& rcMatrix)
{
    matrixParamsOutput(rcMatrix);
    cout << "Content:\n";

    Matrix<Test>::const_iterator_rows it;
    Matrix<Test>::const_iterator_rows first_row;
    Matrix<Test>::const_iterator_rows last_row;

    first_row = cbegin_of_column;
    last_row = cend_of_column;
    for(it = first_row; it != last_row; ++it)
        cout << std::setw(3) << *it;
    cout << endl;

    size_t uCols = rcMatrix.columns();
    size_t uRows = rcMatrix.rows();
    for(size_t uCol = 1; uCol < uCols; ++uCol)
    {
        first_row = rcMatrix.crow(0, uCol);
        last_row = rcMatrix.crow(uRows, uCol);
        for(it = first_row; it != last_row; ++it)
            cout << std::setw(3) << *it;
        cout << endl;
    }
    cout << endl;
}

void output(const std::exception &rcException)
{
    cout << "Exception: " << rcException.what() << endl;
}


int main()
{
    cout << "0. Reference counter: " << endl;
    //
    Matrix<int> m1; //(1, 3);
    m1.push_row(1, 2, 3);
    //m1.push_row(4, 5, 6);

    Matrix<int> m2(m1);
    Matrix<int> m3; m3 = m2;
    cout << "1)\n" << m1 << m2 << m3;
    int& rElem = m1(0, 1);
    Matrix<int> m4(m1);
    cout << m4;
    rElem = 4;
    cout << "2)\n" << m1 << m2 << m3 << m4;

    m1.push_column(7); //, 8);
    Matrix<int> m5(m1);
    cout << "3)\n" << m1 << m2 << m3 << m4 << m5;
    m4.push_column(9); //, 10);
    cout << "4)\n" << m1 << m2 << m3 << m4 << m5;

    m2 = m1;
    cout << "5)\n" << m1 << m2 << m3 << m4 << m5;

    m5 = std::move(m1);
    cout << "6)\n" << m1 << m2 << m3 << m4 << m5;

    Matrix<int> m6(std::move(m2));
    cout << "7)\n" << m1 << m2 << m3 << m4 << m5 << m6;

    cout << endl;
    //---------------------------------------------------------------------------------------------
    using Matrix = Matrix <Test>;
    //
    // 1. push_row()
    //
    cout << "1. push_row()" << endl;
    //
    Matrix mx1; //(1, 3);
    int i = 0;
    for (; i < 7; i += 3)
        mx1.push_row(Test(i), Test(i+1), Test(i+2)); //, Test(500));
    //
    for (; mx1.size() < mx1.capacity(); i += 3)
        mx1.push_row(Test(i), Test(i+1), Test(i+2));
    //
    auto cbegin_of_first_col = mx1.cbegin_of_column(0);
    auto cend_of_first_col = mx1.cend_of_column(0);
    output_rows(cbegin_of_first_col, cend_of_first_col, mx1);
    //
    try
    {
        mx1.push_row(Test(997), Test(998), Test(999, true));
    }
    catch (const exception &rcException)
    {
        output(rcException);
    }
    //
    output_rows(cbegin_of_first_col, cend_of_first_col, mx1);
    //
    mx1.push_row(Test(9), Test(10), Test(11));
    //
    cbegin_of_first_col = mx1.cbegin_of_column(0);
    cend_of_first_col = mx1.cend_of_column(0);
    output_rows(cbegin_of_first_col, cend_of_first_col, mx1);
    //
    try
    {
        mx1.push_row(Test(9997), Test(9998), Test(9999, true));
    }
    catch (const exception &rcException)
    {
        output(rcException);
    }
    //
    output_rows(cbegin_of_first_col, cend_of_first_col, mx1);
    //
    mx1.push_row(Test(12), Test(13, false, true), Test(14));
    mx1.push_row(Test(15), Test(16), Test(17));
    //
    cbegin_of_first_col = mx1.cbegin_of_column(0);
    cend_of_first_col = mx1.cend_of_column(0);
    output_rows(cbegin_of_first_col, cend_of_first_col, mx1);
    //---------------------------------------------------------------------------------------------
    // 2. Copy construction
    //
    cout << "2. Copy construction" << endl;
    Test::ms_bThrowDeferred = true;
    try
    {
        Test& rT = mx1(0, 1); // mx1 must be unshareable to do copy
        Matrix mx2 = mx1;
    }
    catch (const exception &rcException)
    {
        output(rcException);
    }
    catch (...)
    {
        cout << "catch (...)" << endl;
    }
    cout << endl;
    //---------------------------------------------------------------------------------------------
    // 3. Copy assignment
    //
    cout << "3. Copy assignment" << endl;
    Matrix mx3; //(1, 3);
    mx3.push_row(Test(1), Test(1), Test(1));
    //
    cbegin_of_first_col = mx3.cbegin_of_column(0);
    cend_of_first_col = mx3.cend_of_column(0);
    output_rows(cbegin_of_first_col, cend_of_first_col, mx3);
    //
    cout << "Copy assignment: " << endl;
    //
    try
    {
        mx3 = mx1;
    }
    catch (const exception &rcException)
    {
        output(rcException);
    }
    output_rows(cbegin_of_first_col, cend_of_first_col, mx3);
    //---------------------------------------------------------------------------------------------
    // 4. push_column()
    //
    Test::ms_bThrowDeferred = false;
    cout << "4. push_column()" << endl;
    //
    Matrix my1; //(3, 1);
    int j = 0;
    for (; j < 6; j += 3)
        my1.push_column(Test(j), Test(j+1), Test(j+2)); //, Test(500));
    //
    cbegin_of_first_col = my1.cbegin_of_column(0);
    cend_of_first_col = my1.cend_of_column(0);
    //
    output_rows(cbegin_of_first_col, cend_of_first_col, my1);
    //
    for (; my1.size() < my1.capacity(); j += 3)
        my1.push_column(Test(j), Test(j+1), Test(j+2));
    //
    output_rows(cbegin_of_first_col, cend_of_first_col, my1);
    //
    try
    {
        my1.push_column(Test(97), Test(98), Test(99, true));
    }
    catch (const exception &rcException)
    {
        output(rcException);
    }
    //
    output_rows(cbegin_of_first_col, cend_of_first_col, my1);
    //
    // Iterators will be invalid:
    cout << "Iterators will be invalid: " << endl;
    //
    // copying all matrix -> iterators invalid
    my1.push_column(Test(9), Test(10), Test(11));
    //
    cout << "Define new iterators before output: " << endl;
    //
    cbegin_of_first_col = my1.cbegin_of_column(0);
    cend_of_first_col = my1.cend_of_column(0);
    output_rows(cbegin_of_first_col, cend_of_first_col, my1);
    //
    try
    {
        my1.push_column(Test(997), Test(998), Test(999, true));
    }
    catch (const exception &rcException)
    {
        output(rcException);
    }
    //
    output_rows(cbegin_of_first_col, cend_of_first_col, my1);
    //
    my1.push_column(Test(12), Test(13, false, true), Test(14));
    //my1.push_column(Test(15), Test(16), Test(17)); // <- iterators will be invalid
//    //
    output_rows(cbegin_of_first_col, cend_of_first_col, my1);
    //
    //---------------------------------------------------------------------------------------------
    // Tests:
    //    
    //test_push_row();
    //test_push_column();
    //test_swap();
    //test_reserve();
    //test_iterators();
    //cout << "-------------------------------------------------------------------" << endl;

    //system("pause");
    return 0;
}
