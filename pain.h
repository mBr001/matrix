#ifndef _PAIN_H_
#define _PAIN_H_

#include <array>
#include <map>

#include "lib.h"

template<typename T, size_t DIM>
class SmokerMatrix
{
    using Position  = std::array<int, DIM>;
    using Container = std::map<Position, T>;
    
    T defaultValue;
    Container container;
    std::map<Position, int> cellCounter;

    void commit(const Position& pos)
    {
        auto elem = container.find(pos);
        auto cnt = cellCounter.find(pos);

        --cnt->second;

        if (cnt->second == 0 && elem->second == defaultValue) {
            container.erase(elem);
        }

        if (cnt->second == 0) {
            cellCounter.erase(cnt);
        }
    }

    class Cell
    {
        friend SmokerMatrix;

        SmokerMatrix *owner;
        Position pos;
        T& elem;

        Cell()            = delete;
        Cell(const Cell&) = delete;

        Cell(Cell&& c) : owner(c.owner), pos(c.pos), elem(c.elem) {
            c.owner = nullptr;
        };
        
        Cell(SmokerMatrix* owner, const Position& pos, T& elem) 
            : owner(owner), pos(pos), elem(elem) {}

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

        ~Cell() { 
            if (owner) {
                owner->commit(pos);
            }
        }
    };


    // #########################################################################
    Cell getCell(const Position& pos) 
    {
        ++cellCounter[pos];
        if (container.find(pos) == container.end()) {
            container.emplace(pos, defaultValue);
        }
        return Cell(this, pos, container[pos]);
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
                auto p = pos;
                p[DIM - N] = i;
                return View<N-1>(matrix, p);
            } 
            else {
                auto a = pos;
                a[DIM - 1] = i;
                return matrix->getCell(a);
            }
        }
    };


    // #########################################################################
    class ConstIterator 
    {
        friend SmokerMatrix;

        using IterType = typename Container::const_iterator;

        SmokerMatrix *owner;
        IterType mapIterator;

        ConstIterator() = delete;
        ConstIterator(SmokerMatrix *owner, IterType iter) 
            : owner(owner), mapIterator(iter) {}

    public:
        ConstIterator(const ConstIterator&) = default;
        ConstIterator(ConstIterator&&)      = default;

        ConstIterator& operator= (const ConstIterator&) = default;
        ConstIterator& operator= (ConstIterator&&)      = default;

        bool operator!= (const ConstIterator& rhs) const noexcept {
            return mapIterator != rhs.mapIterator;
        }

        bool operator== (const ConstIterator& rhs) const noexcept {
            return mapIterator == rhs.mapIterator;
        }

        auto operator* () const noexcept {
            return makeTuple(mapIterator->first, mapIterator->second);
        }        

        auto operator-> () const noexcept {
            return &mapIterator->second;
        }

    private:
        void inc() noexcept 
        {
            for (++mapIterator; 
                mapIterator != owner->container.end(); ++mapIterator) 
            {
                if (mapIterator->second != owner->defaultValue) {
                    return;
                }
            }
        }

    public:
        ConstIterator& operator++ () noexcept {
            return (inc(), *this);
        }

        ConstIterator operator++ (int) noexcept {
            ConstIterator iter{mapIterator};
            inc();
            return iter;
        }
    };


public:
    SmokerMatrix()                      : defaultValue(T()) {}
    SmokerMatrix(const T& defaultValue) : defaultValue(defaultValue) {}

    auto operator[] (int i) {
        return View<DIM>(this, Position{i})[i];
    }

    size_t size() const noexcept
    {
        size_t size = container.size();
        for (auto posCnt : cellCounter) {
            if (container.at(posCnt.first) == defaultValue) {
                --size;
            }
        }
        return size;
    }

    using const_iterator = ConstIterator;

    const_iterator begin() const noexcept { 
        return ConstIterator(
            const_cast<SmokerMatrix*>(this), container.cbegin()); 
    }
    
    const_iterator end() const noexcept { 
        return ConstIterator(
            const_cast<SmokerMatrix*>(this), container.cend());   
    }

    const_iterator cbegin() const noexcept {
        return ConstIterator(
            const_cast<SmokerMatrix*>(this), container.cbegin());
    }

    const_iterator cend() const noexcept {
        return ConstIterator(
            const_cast<SmokerMatrix*>(this), container.cend());
    }
};

#endif // _PAIN_H_