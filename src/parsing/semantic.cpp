// semantic.cpp

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

#include "semantic.hpp"

NumericValue::ptr_t negation_op(NumericValue::ptr_t operand)
{
    if (operand->value_type == NumericValue::EXACT)
        operand->value.exact = -operand->value.exact;
    else if(operand->value_type == NumericValue::FLOATING)
        operand->value.floating = -operand->value.floating;

    return operand;
}

NumericValue::ptr_t plus_op(NumericValue::ptr_t lhs, NumericValue::ptr_t rhs)
{
    // simply add values of the same type
    if (lhs->value_type == NumericValue::EXACT &&
        rhs->value_type == NumericValue::EXACT)
    {
        lhs->value.exact += rhs->value.exact;
    }
    else if (lhs->value_type == NumericValue::FLOATING &&
        rhs->value_type == NumericValue::FLOATING)
    {
        lhs->value.floating += rhs->value.floating;
    }

    // allways convert to higher order representation if types are different
    else if (lhs->value_type == NumericValue::EXACT &&
        rhs->value_type == NumericValue::FLOATING)
    {
        lhs->value.floating = lhs->value.exact + rhs->value.floating;
        lhs->value_type = NumericValue::FLOATING;
    }
    else if (lhs->value_type == NumericValue::FLOATING &&
        rhs->value_type == NumericValue::EXACT)
    {
        lhs->value.floating += rhs->value.exact;
    }

    return lhs;
}


NumericValue::ptr_t minus_op(NumericValue::ptr_t lhs, NumericValue::ptr_t rhs)
{
    // simply subtract values of the same type
    if (lhs->value_type == NumericValue::EXACT &&
        rhs->value_type == NumericValue::EXACT)
    {
        lhs->value.exact -= rhs->value.exact;
    }
    else if (lhs->value_type == NumericValue::FLOATING &&
        rhs->value_type == NumericValue::FLOATING)
    {
        lhs->value.floating -= rhs->value.floating;
    }

    // allways convert to higher order representation if types are different
    else if (lhs->value_type == NumericValue::EXACT &&
        rhs->value_type == NumericValue::FLOATING)
    {
        lhs->value.floating = lhs->value.exact + rhs->value.floating;
        lhs->value_type = NumericValue::FLOATING;
    }
    else if (lhs->value_type == NumericValue::FLOATING &&
        rhs->value_type == NumericValue::EXACT)
    {
        lhs->value.floating -= rhs->value.exact;
    }

    return lhs;
}

NumericValue::ptr_t multiply_op(NumericValue::ptr_t lhs,NumericValue::ptr_t rhs)
{
    // simply multiply values of the same type
    if (lhs->value_type == NumericValue::EXACT &&
        rhs->value_type == NumericValue::EXACT)
    {
        lhs->value.exact *= rhs->value.exact;
    }
    else if (lhs->value_type == NumericValue::FLOATING &&
        rhs->value_type == NumericValue::FLOATING)
    {
        lhs->value.floating *= rhs->value.floating;
    }

    // allways convert to higher order representation if types are different
    else if (lhs->value_type == NumericValue::EXACT &&
        rhs->value_type == NumericValue::FLOATING)
    {
        lhs->value.floating = lhs->value.exact + rhs->value.floating;
        lhs->value_type = NumericValue::FLOATING;
    }
    else if (lhs->value_type == NumericValue::FLOATING &&
        rhs->value_type == NumericValue::EXACT)
    {
        lhs->value.floating *= rhs->value.exact;
    }

    return lhs;
}

NumericValue::ptr_t divide_op(NumericValue::ptr_t lhs, NumericValue::ptr_t rhs)
{
    // division allways produces a floating type
    if (lhs->value_type == NumericValue::EXACT &&
      rhs->value_type == NumericValue::EXACT)
    {
        lhs->value.floating =
            static_cast<double>(lhs->value.exact) / rhs->value.exact;
    }
    else if (lhs->value_type == NumericValue::FLOATING &&
        rhs->value_type == NumericValue::FLOATING)
    {
        lhs->value.floating /= rhs->value.floating;
    }
    else if (lhs->value_type == NumericValue::EXACT &&
        rhs->value_type == NumericValue::FLOATING)
    {
        lhs->value.floating = lhs->value.exact / rhs->value.floating;
    }
    else if (lhs->value_type == NumericValue::FLOATING &&
        rhs->value_type == NumericValue::EXACT)
    {
        lhs->value.floating /= rhs->value.exact;
    }

    lhs->value_type = NumericValue::FLOATING;
    return lhs;
}

NumericValue::ptr_t pow_op(NumericValue::ptr_t lhs, NumericValue::ptr_t rhs)
{
    // exect to the power of an exact is still exact
    if (lhs->value_type == NumericValue::EXACT &&
        rhs->value_type == NumericValue::EXACT)
    {
        lhs->value.exact = std::pow(lhs->value.exact,rhs->value.exact);
    }
    // floating pow operation
    else if (lhs->value_type == NumericValue::FLOATING &&
        rhs->value_type == NumericValue::FLOATING)
    {
        lhs->value.floating = std::pow(lhs->value.floating,rhs->value.floating);
    }

    // convert to floating if exponent or base is floating
    else if (lhs->value_type == NumericValue::EXACT &&
        rhs->value_type == NumericValue::FLOATING)
    {
        lhs->value.floating = std::pow(lhs->value.exact, rhs->value.floating);
        lhs->value_type = NumericValue::FLOATING;
    }
    else if (lhs->value_type == NumericValue::FLOATING &&
        rhs->value_type == NumericValue::EXACT)
    {
        lhs->value.floating = std::pow(lhs->value.floating, rhs->value.exact);
    }

    return lhs;
}

