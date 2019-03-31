#ifndef _HUMANITY_H_
#define _HUMANITY_H_

#include <map>
#include <array>

#include "lib.h"


template<typename T, size_t DIM>
class HealthyMatrix 
{
    using Position  = std::array<int, DIM>;
    using Container = std::map<Position, T>;

    T defaultValue;
    Container container;


    void set(const Position& pos, const T& elem) 
    {
        auto iter = container.find(pos);
        bool found = iter != container.end();

        if (found && elem != defaultValue) {
            iter->second = elem;
        } 
        else if (found  && elem == defaultValue) {
            container.erase(iter);
        } 
        else if (elem != defaultValue) {
            container.emplace(pos, elem);
        }
    }

    const T& get(const Position& pos) const 
    {
        auto iter = container.find(pos);
        if (iter != container.end()) {
            return iter->second;
        } 
        return defaultValue;
    }
    
    // #########################################################################
    class ConstIterator 
    {
        using IterType = typename Container::const_iterator;
        
        friend HealthyMatrix;

        IterType mapIterator; 

        ConstIterator() = delete;
        ConstIterator(IterType iter) : mapIterator(iter) {}

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

        ConstIterator& operator++ () noexcept {
            return (++mapIterator, *this);
        }

        ConstIterator operator++ (int) noexcept {
            ConstIterator iter{mapIterator};
            ++mapIterator;
            return iter;
        }
    };


public:
    HealthyMatrix()                      : defaultValue(T())          {}
    HealthyMatrix(const T& defaultValue) : defaultValue(defaultValue) {}

    HealthyMatrix(const HealthyMatrix&) = default;
    HealthyMatrix(HealthyMatrix&&)      = default;
    
    HealthyMatrix& operator= (const HealthyMatrix&) = default;
    HealthyMatrix& operator= (HealthyMatrix&&)      = default;


    // TODO: ints?
    template<typename... Pos>
    void set(const T& elem, int x1, Pos... p) 
    {
        static_assert(sizeof...(p) + 1 == DIM, 
            "invalid number of index arguments");
        set(Position{x1, p...}, elem);
    }

    template<typename... Pos>
    const T& get(int x1, Pos... p) 
    {
        static_assert(sizeof...(p) + 1 == DIM, 
            "invalid number of index arguments");
        return get(Position{x1, p...});
    }


    size_t size() const noexcept { 
        return container.size(); 
    }


    using const_iterator = ConstIterator;

    const_iterator begin() const noexcept { 
        return ConstIterator(container.cbegin()); 
    }
    
    const_iterator end() const noexcept { 
        return ConstIterator(container.cend());   
    }

    const_iterator cbegin() const noexcept {
        return ConstIterator(container.cbegin());
    }

    const_iterator cend() const noexcept {
        return ConstIterator(container.cend());
    }
};

#endif // _HUMANITY_H_