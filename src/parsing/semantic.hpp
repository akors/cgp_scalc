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

#include <memory>

#ifdef __GNUC__ // on gcc, std::tr1::shared_ptr is not part of memory by default
#   include <tr1/memory>
#endif

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
    typedef std::tr1::shared_ptr<NumericValue> ptr_t;

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



struct Expression
{
    typedef std::tr1::shared_ptr<Expression> ptr_t;

    // output class value to stream
    virtual std::ostream& to_stream(std::ostream& os) const = 0;

    // return numeric value
    virtual NumericValue numeric_value() const = 0;

    // virtual destructor
    virtual ~Expression() {}
};

inline std::ostream& operator << (std::ostream& os, const Expression& exp)
{ return exp.to_stream(os); }


struct NumericExpression : public Expression
{
    NumericExpression(const NumericValue& val)
      : _val(val)
    {}

    virtual NumericValue numeric_value() const
    { return _val; }

    virtual std::ostream& to_stream(std::ostream& os) const
    { os<<_val; }

    virtual ~NumericExpression() {};

    NumericValue _val;
};


struct UnaryOperation : public Expression
{
    typedef NumericValue (*unary_operation_t)(const NumericValue&);

    UnaryOperation(Expression::ptr_t operand, unary_operation_t expr_operator)
        : _operand(operand), _expr_operator(expr_operator)
    { }

    virtual NumericValue numeric_value() const
    {
        return _expr_operator(_operand->numeric_value());
    }

    virtual std::ostream& to_stream(std::ostream& os) const
    {
        os<<numeric_value();
    }

    virtual ~UnaryOperation() {}

private:
    Expression::ptr_t _operand;
    unary_operation_t _expr_operator;
};


struct BinaryOperation : public Expression
{
    typedef NumericValue (*binary_operation_t)
        (const NumericValue&, const NumericValue&);

    BinaryOperation(
        Expression::ptr_t lhs, Expression::ptr_t rhs,
        binary_operation_t expr_operator
    )
        : _lhs(lhs), _rhs(rhs), _expr_operator(expr_operator)
    { }

    virtual NumericValue numeric_value() const
    {
        return _expr_operator(_lhs->numeric_value(), _rhs->numeric_value());
    }

    virtual std::ostream& to_stream(std::ostream& os) const
    {
        os<<numeric_value();
    }

    virtual ~BinaryOperation() {}

private:
    Expression::ptr_t _lhs, _rhs;
    binary_operation_t _expr_operator;
};

NumericValue negation_op(const NumericValue& operand);
NumericValue plus_op(const NumericValue& lhs, const NumericValue& rhs);
NumericValue minus_op(const NumericValue& lhs, const NumericValue& rhs);
NumericValue multiply_op(const NumericValue& lhs, const NumericValue& rhs);
NumericValue divide_op(const NumericValue& lhs, const NumericValue& rhs);
NumericValue pow_op(const NumericValue& lhs, const NumericValue& rhs);

#endif // ifndef SEMANTIC_HPP_
