#ifndef MATRIX_H
#define MATRIX_H

#include <iomanip>
#include <iterator>
#include <memory>
#include <cstring>

#define CHECK_BOUNDS false

template<typename T>
class Matrix {

    public:
        Matrix(size_t rows, size_t cols) :
            m_rows{rows},
            m_cols{cols}
        {
            m_data = std::unique_ptr<T[]>(new T[m_rows*m_cols]());
        }

        Matrix(const std::vector<std::vector<T>> & data) :
            m_rows{data.size()},
            m_cols{data[0].size()}
        {
            m_data = std::unique_ptr<T[]>(new T[m_rows*m_cols]());

            auto lastpos = m_data.get();
            for (auto& row : data) {
                lastpos = std::copy(row.begin(), row.end(), lastpos);
            }
        }

        T& operator()(size_t row, size_t col) {
            check(row, col);
            return m_data[row*m_cols + col];
        }

        T operator()(size_t row, size_t col) const {
            check(row, col);
            return m_data[row*m_cols + col];
        }

        bool operator==(const Matrix<T> & other) const {
            if (m_rows != other.m_rows) {
                return false;
            }
            if (m_cols != other.m_cols) {
                return false;
            }

            return 0 == memcmp(m_data.get(), other.m_data.get(), m_rows*m_cols*sizeof(T));
        }

        void increment(size_t row, size_t col, const T & val) {
            check(row, col);
            m_data[row*m_cols + col] += val;
        }

        size_t rows() const { return m_rows; }
        size_t cols() const { return m_cols; }

        T* row(size_t row) {
            return &(m_data[row*m_cols]);
        }

        T* data() {
            return m_data.get();
        }

        void check(size_t row, size_t col) const {
            #if CHECK_BOUNDS
                if (row + 1 > m_rows) {
                    throw std::out_of_range("Row " + std::to_string(row) + " is out of range.");
                }
                if (row < 0) {
                    throw std::out_of_range("Row " + std::to_string(row) + " is out of range.");
                }
                if (col + 1 > m_cols) {
                    throw std::out_of_range("Col " + std::to_string(col) + " is out of range.");
                }
                if (col < 0) {
                    throw std::out_of_range("Col " + std::to_string(col) + " is out of range.");
                }
            #endif
        }

    private:
        std::unique_ptr<T[]> m_data;

        size_t m_rows;
        size_t m_cols;

};

template<typename T>
std::ostream& operator<<(std::ostream & os, const Matrix<T> & m) {
    for (size_t i = 0; i < m.rows(); i++) {
        for (size_t j = 0; j < m.cols(); j++) {
            if (m(i, j) != 0) {
                os << std::right << std::fixed << std::setw(10) << std::setprecision(6) <<
                    m(i, j) << " ";
            } else {
                os << "           ";
            }
        }
        os << std::endl;
    }

    return os;
}

#endif