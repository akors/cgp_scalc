// semantic.hpp

/*
 *   scalc - A simple calculator
 *   Copyright (C) 2010  Alexander Korsunsky
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SEMANTIC_HPP_
#define SEMANTIC_HPP_

#include <iostream>

struct NumericValue
{
    double value;

    NumericValue& operator += (const NumericValue& other)
    {
        this->value += other.value;
        return *this;
    }

    NumericValue& operator -= (const NumericValue& other)
    {
        this->value -= other.value;
        return *this;
    }

    NumericValue& operator *= (const NumericValue& other)
    {
        this->value *= other.value;
        return *this;
    }

    NumericValue& operator /= (const NumericValue& other)
    {
        this->value /= other.value;
        return *this;
    }
};

inline std::ostream& operator << (std::ostream& os, const NumericValue& v)
{ return os<<v.value; }

inline std::istream& operator >> (std::istream& is, NumericValue& v)
{ return is>>v.value; }

#endif // ifndef SEMANTIC_HPP_
