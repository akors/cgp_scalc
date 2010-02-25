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
#include <stdexcept>
#include <cstdlib>
#include <cerrno>
#include <cmath>


struct NumericError : public std::runtime_error
{
    enum errtype_t {
        ERR_INVALID_FORMAT,
        ERR_OVERFLOW,

        UNKNOWN
    } _errtype;

public:
    /** Default Constructor */
    NumericError() throw()
        : std::runtime_error("Unknown numeric error"), _errtype(UNKNOWN)
    { }

    /** Constructor.
    * @param str The error message
    */
    NumericError(const char* str, errtype_t errtype) throw()
        : std::runtime_error(str), _errtype(errtype)
    {}

    /** Return error message as char array.
    * @return A null- terminated character array containg the error message
    */
    virtual const char* what() const throw()
    { return std::runtime_error::what(); }

    virtual ~NumericError() throw() { }
};



struct NumericValue
{
    // what type of number is this
    enum {
        EXACT,
        FLOATING
    } value_type;

    union {
        long int exact;
        double floating;
    } value;

    NumericValue& from_exact(const char* str)
        throw (NumericError)
    {
        char* endptr;

        value.exact = strtol(str, &endptr, 10);
        if (endptr == str)
        {
            throw(NumericError(
                    "Invalid integer format",
                    NumericError::ERR_INVALID_FORMAT
            ));
        }

        if (errno == ERANGE)
        {
            throw(
                NumericError(
                    "Out of numeric range",
                    NumericError::ERR_OVERFLOW
            ));
        }

        value_type = EXACT;

        return *this;
    }

    NumericValue& from_floating(const char* str)
        throw (NumericError)
    {
        char* endptr;

        value.floating = strtod(str, &endptr);
        if (endptr == str)
        {
            throw(NumericError(
                    "Invalid integer format",
                    NumericError::ERR_INVALID_FORMAT
            ));
        }

        if (errno == ERANGE && value.floating == HUGE_VAL)
        {
            throw(
                NumericError(
                    "Out of numeric range",
                    NumericError::ERR_OVERFLOW
            ));
        }

        value_type = FLOATING;

        return *this;
    }

    NumericValue& operator += (const NumericValue& other)
    {
        // simply add values of the same type
        if (this->value_type == EXACT && other.value_type == EXACT)
        {
            this->value.exact += other.value.exact;
        }
        else if (this->value_type == FLOATING && other.value_type == FLOATING)
        {
            this->value.floating += other.value.floating;
        }

        // allways convert to higher order representation if types are different
        else if (this->value_type == EXACT && other.value_type == FLOATING)
        {
            this->value.floating = this->value.exact + other.value.floating;
            this->value_type = FLOATING;
        }
        else if (this->value_type == FLOATING && other.value_type == EXACT)
        {
            this->value.floating += other.value.exact;
        }

        return *this;
    }

    NumericValue& operator -= (const NumericValue& other)
    {
        // simply subtract values of the same type
        if (this->value_type == EXACT && other.value_type == EXACT)
        {
            this->value.exact -= other.value.exact;
        }
        else if (this->value_type == FLOATING && other.value_type == FLOATING)
        {
            this->value.floating -= other.value.floating;
        }

        // allways convert to higher order representation if types are different
        else if (this->value_type == EXACT && other.value_type == FLOATING)
        {
            this->value.floating = this->value.exact - other.value.floating;
            this->value_type = FLOATING;
        }
        else if (this->value_type == FLOATING && other.value_type == EXACT)
        {
            this->value.floating -= other.value.exact;
        }

        return *this;
    }

    NumericValue& operator *= (const NumericValue& other)
    {
        // simply multiply values of the same type
        if (this->value_type == EXACT && other.value_type == EXACT)
        {
            this->value.exact *= other.value.exact;
        }
        else if (this->value_type == FLOATING && other.value_type == FLOATING)
        {
            this->value.floating *= other.value.floating;
        }

        // allways convert to higher order representation if types are different
        else if (this->value_type == EXACT && other.value_type == FLOATING)
        {
            this->value.floating = this->value.exact * other.value.floating;
            this->value_type = FLOATING;
        }
        else if (this->value_type == FLOATING && other.value_type == EXACT)
        {
            this->value.floating *= other.value.exact;
        }

        return *this;
    }

    NumericValue& operator /= (const NumericValue& other)
    {
        // division allways produces a floating type
        if (this->value_type == EXACT && other.value_type == EXACT)
        {
            this->value.floating =
                static_cast<double>(this->value.exact) / other.value.exact;
        }
        else if (this->value_type == FLOATING && other.value_type == FLOATING)
        {
            this->value.floating /= other.value.floating;
        }
        else if (this->value_type == EXACT && other.value_type == FLOATING)
        {
            this->value.floating = this->value.exact / other.value.floating;
        }
        else if (this->value_type == FLOATING && other.value_type == EXACT)
        {
            this->value.floating /= other.value.exact;
        }

        this->value_type = FLOATING;
        return *this;
    }

    NumericValue& negate()
    {
        if (value_type == EXACT)
            value.exact = -value.exact;
        else if(value_type == FLOATING)
            value.floating = -value.floating;

        return *this;
    }
};


inline std::ostream& operator << (std::ostream& os, const NumericValue& v)
{
    if (v.value_type == NumericValue::EXACT)
        return os<<v.value.exact;
    else if(v.value_type == NumericValue::FLOATING)
        return os<<v.value.floating;
}

#if 0
inline std::istream& operator >> (std::istream& is, NumericValue& v)
{ return is>>v.value.floating; }
#endif

#endif // ifndef SEMANTIC_HPP_
