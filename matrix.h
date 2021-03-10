#include <iostream>
#include <vector>
#include <stdexcept>
#include <cassert>

using namespace std;

template <typename T>
class Matrix {
public:
    using Row = vector<T>;

private:
    vector<vector<T>> rows;

    class Iterator {
        Matrix<T>& M;
        size_t m, n;

    public:
        Iterator(Matrix<T>& M_, size_t m_, size_t n_): M(M_), m(m_), n(n_) {}

        bool operator == (Iterator other) {
            return m == other.m && n == other.n;
        }

        bool operator != (Iterator other) {
            return !(*this == other);
        }

        T& operator * () {
            return M[m][n];
        }

        Iterator& operator++() {
            ++n;
            if (n == M[0].size()) {
                n = 0;
                ++m;
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator old = *this;
            operator++();
            return old;
        }
    };

    class Const_Iterator {
        const Matrix<T>& M;
        size_t m, n;

    public:
        Const_Iterator(const Matrix<T>& M_, size_t m_, size_t n_): M(M_), m(m_), n(n_) {}

        bool operator == (Const_Iterator other) {
            return m == other.m && n == other.n;
        }

        bool operator != (Const_Iterator other) {
            return !(*this == other);
        }

        T operator * () {
            return M[m][n];
        }

        Const_Iterator& operator++() {
            ++n;
            if (n == M[0].size()) {
                n = 0;
                ++m;
            }
            return *this;
        }

        Const_Iterator operator++(int) {
            Const_Iterator old = *this;
            operator++();
            return old;
        }
    };

public:
    Matrix(const vector<Row>& rows_):rows(rows_) {}

    pair<std::size_t, size_t> size() const {
        return make_pair(rows.size(), rows[0].size());
    }

    const Row& operator[] (size_t i) const {
        return rows[i];
    }

    Row& operator[] (size_t i) {
        return rows[i];
    }

    Matrix& operator += (const Matrix& other) {
        for (size_t i = 0; i < rows.size(); ++i) {
            for (size_t j = 0; j < rows[0].size(); ++j) {
                rows[i][j] += other[i][j];
            }
        }
        return *this;
    }

    Matrix operator + (const Matrix& other) const {
        auto res(*this);
        res += other;
        return res;
    }

    template<typename Scalar_T>
    Matrix& operator *= (Scalar_T scalar) {
        for (size_t i = 0; i < rows.size(); ++i) {
            for (size_t j = 0; j < rows[0].size(); ++j) {
                rows[i][j] *= scalar;
            }
        }
        return *this;
    }

    template<typename Scalar_T>
    Matrix operator * (Scalar_T scalar) const {
        auto res(*this);
        res *= scalar;
        return res;
    }

    Matrix& operator *= (const Matrix& other) {
        assert(rows[0].size() == other.rows.size());
        vector<Row> tmp_rows(rows.size(), Row(other.rows[0].size()));
        Matrix tmp(tmp_rows);
        for (size_t k = 0; k < rows[0].size(); ++k) {
            for (size_t i = 0; i < rows.size(); ++i) {
                for (size_t j = 0; j < other.rows[0].size(); ++j) {
                    tmp.rows[i][j] += rows[i][k] * other.rows[k][j];
                }
            }
        }
        *this = tmp;
        return *this;
    }

    Matrix operator * (const Matrix& other) const {
        assert(rows[0].size() == other.rows.size());
        Matrix res(*this);
        res *= other;
        return res;
    }

    Matrix& transpose() {
        vector<Row> tmp_rows(rows[0].size(), vector<T>(rows.size()));
        Matrix tmp(tmp_rows);
        for (size_t i = 0; i < rows.size(); ++i) {
            for (size_t j = 0; j < rows[0].size(); ++j) {
                tmp.rows[j][i] = rows[i][j];
            }
        }
        *this = tmp;
        return *this;
    }
    Matrix transposed() const {
        vector<Row> tmp_rows(rows[0].size(), vector<T>(rows.size()));
        Matrix tmp(tmp_rows);
        for (size_t i = 0; i < rows.size(); ++i) {
            for (size_t j = 0; j < rows[0].size(); ++j) {
                tmp.rows[j][i] = rows[i][j];
            }
        }
        return tmp;
    }

    Iterator begin() {
        return Iterator(*this, 0, 0);
    }

    Iterator end() {
        return Iterator(*this, rows.size(), 0);
    }

    Const_Iterator begin() const {
        return Const_Iterator(*this, 0, 0);
    }

    Const_Iterator end() const {
        return Const_Iterator(*this, rows.size(), 0);
    }
};

template <typename T>
ostream& operator<<(ostream& out, const Matrix<T>& m) {
    auto [p, q] = m.size();
    for (size_t i = 0; i < p; ++i) {
        for (size_t j = 0; j < q; ++j) {
            out << m[i][j];
            if (j != q - 1) out << '\t';
        }
        if (i != p - 1) out << '\n';
    }
    return out;
}
