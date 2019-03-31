#ifndef _PAIN_H_
#define _PAIN_H_

#include "humanity.h"


template<typename T, size_t DIM>
class SmokerMatrix : public HealthyMatrix<T, DIM>
{
    using Position = typename HealthyMatrix<T, DIM>::Position;

    class Cell
    {
        friend SmokerMatrix;

        SmokerMatrix *owner;
        Position pos;
        T elem;

        Cell()            = delete;
        Cell(const Cell&) = delete;

        Cell(Cell&& c) : owner(c.owner), pos(c.pos), elem(c.elem) {
            c.owner = nullptr;
        };
        
        Cell(SmokerMatrix* owner, const Position& pos, const T& elem) 
            : owner(owner), pos(pos), elem(elem) {}

        void commit() 
        {
            if (owner) {
                owner->set(pos, elem);
            }
        }

    public:
        operator T&()             { return elem; }
        operator const T&() const { return elem; }

        Cell& operator= (const T& rhs) {
            return (elem = rhs, *this);
        }

        Cell& operator= (T&& rhs) {
            return (elem = rhs, *this);
        }

        template<typename U>
        Cell& operator= (const U& rhs) {
            return (elem = T(rhs), *this);
        }

        template<typename U>
        Cell& operator= (U&& rhs) {
            return (elem = T(rhs), *this);
        }

        ~Cell() { commit(); }
    };


    // #########################################################################
    Cell at(const Position& pos) {
        return Cell(this, pos, this->get(pos));
    }

    template<size_t N>
    class View
    {
        friend SmokerMatrix;

        SmokerMatrix* matrix;
        Position pos;

        View(SmokerMatrix* m, const Position& pos) : matrix(m), pos(pos) {}
    public:
        auto operator[] (int i) const {
            if constexpr (N != 1) {
                auto a = pos;
                a[DIM - N] = i;
                return View<N-1>(matrix, a);
            } 
            else {
                auto a = pos;
                a[DIM - 1] = i;
                return Cell(matrix, a, matrix->at(a));
            }
        }
    };


public:
    SmokerMatrix()  = default;
    SmokerMatrix(const T& defaultValue) : HealthyMatrix<T, DIM>(defaultValue) {}

    auto operator[] (int i) {
        return View<DIM>(this, Position{i})[i];
    }
};

#endif // _PAIN_H_