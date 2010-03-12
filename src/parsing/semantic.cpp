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

NumericValue negation_op(const NumericValue& operand)
{
    NumericValue retval;

    if (operand.value_type == NumericValue::EXACT)
        retval.value.exact = -operand.value.exact;
    else if(operand.value_type == NumericValue::FLOATING)
        retval.value.floating = -operand.value.floating;

    retval.value_type = operand.value_type;

    return retval;
}

NumericValue plus_op(const NumericValue& lhs, const NumericValue& rhs)
{
    NumericValue retval;

    // simply add values of the same type
    if (lhs.value_type == NumericValue::EXACT &&
        rhs.value_type == NumericValue::EXACT)
    {
        retval.value.exact = lhs.value.exact + rhs.value.exact;
        retval.value_type = NumericValue::EXACT;
    }
    else if (lhs.value_type == NumericValue::FLOATING &&
        rhs.value_type == NumericValue::FLOATING)
    {
        retval.value.floating = lhs.value.floating + rhs.value.floating;
        retval.value_type = NumericValue::FLOATING;
    }

    // allways convert to higher order representation if types are different
    else if (lhs.value_type == NumericValue::EXACT &&
        rhs.value_type == NumericValue::FLOATING)
    {
        retval.value.floating = lhs.value.exact + rhs.value.floating;
        retval.value_type = NumericValue::FLOATING;
    }
    else if (lhs.value_type == NumericValue::FLOATING &&
        rhs.value_type == NumericValue::EXACT)
    {
        retval.value.floating = lhs.value.floating + rhs.value.exact;
        retval.value_type = NumericValue::FLOATING;
    }

    return retval;
}

NumericValue minus_op(const NumericValue& lhs, const NumericValue& rhs)
{
    NumericValue retval;

    // simply subtract values of the same type
    if (lhs.value_type == NumericValue::EXACT &&
        rhs.value_type == NumericValue::EXACT)
    {
        retval.value.exact = lhs.value.exact - rhs.value.exact;
        retval.value_type = NumericValue::EXACT;
    }
    else if (lhs.value_type == NumericValue::FLOATING &&
        rhs.value_type == NumericValue::FLOATING)
    {
        retval.value.floating = lhs.value.floating - rhs.value.floating;
        retval.value_type = NumericValue::FLOATING;
    }

    // allways convert to higher order representation if types are different
    else if (lhs.value_type == NumericValue::EXACT &&
        rhs.value_type == NumericValue::FLOATING)
    {
        retval.value.floating = lhs.value.exact - rhs.value.floating;
        retval.value_type = NumericValue::FLOATING;
    }
    else if (lhs.value_type == NumericValue::FLOATING &&
        rhs.value_type == NumericValue::EXACT)
    {
        retval.value.floating = lhs.value.floating - rhs.value.exact;
        retval.value_type = NumericValue::FLOATING;
    }

    return retval;
}

NumericValue multiply_op(const NumericValue& lhs, const NumericValue& rhs)
{
    NumericValue retval;

    // simply multiply values of the same type
    if (lhs.value_type == NumericValue::EXACT &&
        rhs.value_type == NumericValue::EXACT)
    {
        retval.value.exact = lhs.value.exact * rhs.value.exact;
        retval.value_type = NumericValue::EXACT;
    }
    else if (lhs.value_type == NumericValue::FLOATING &&
        rhs.value_type == NumericValue::FLOATING)
    {
        retval.value.floating = lhs.value.floating * rhs.value.floating;
        retval.value_type = NumericValue::FLOATING;
    }

    // allways convert to higher order representation if types are different
    else if (lhs.value_type == NumericValue::EXACT &&
        rhs.value_type == NumericValue::FLOATING)
    {
        retval.value.floating = lhs.value.exact * rhs.value.floating;
        retval.value_type = NumericValue::FLOATING;
    }
    else if (lhs.value_type == NumericValue::FLOATING &&
        rhs.value_type == NumericValue::EXACT)
    {
        retval.value.floating = lhs.value.floating * rhs.value.exact;
        retval.value_type = NumericValue::FLOATING;
    }

    return retval;
}

NumericValue divide_op(const NumericValue& lhs, const NumericValue& rhs)
{
    NumericValue retval;

    // division allways produces a floating type
    if (lhs.value_type == NumericValue::EXACT &&
      rhs.value_type == NumericValue::EXACT)
    {
        retval.value.floating =
            static_cast<double>(lhs.value.exact) / rhs.value.exact;
    }
    else if (lhs.value_type == NumericValue::FLOATING &&
        rhs.value_type == NumericValue::FLOATING)
    {
        retval.value.floating = lhs.value.floating / rhs.value.floating;
    }
    else if (lhs.value_type == NumericValue::EXACT &&
        rhs.value_type == NumericValue::FLOATING)
    {
        retval.value.floating = lhs.value.exact / rhs.value.floating;
    }
    else if (lhs.value_type == NumericValue::FLOATING &&
        rhs.value_type == NumericValue::EXACT)
    {
        retval.value.floating = lhs.value.floating / rhs.value.exact;
    }

    retval.value_type = NumericValue::FLOATING;
    return retval;
}

NumericValue pow_op(const NumericValue& lhs, const NumericValue& rhs)
{
    NumericValue retval;

    // exact to the power of an exact is still exact
    if (lhs.value_type == NumericValue::EXACT &&
        rhs.value_type == NumericValue::EXACT)
    {
        retval.value.exact = std::pow(lhs.value.exact,rhs.value.exact);
        retval.value_type = NumericValue::EXACT;
    }
    // floating pow operation
    else if (lhs.value_type == NumericValue::FLOATING &&
        rhs.value_type == NumericValue::FLOATING)
    {
        retval.value.floating = std::pow(lhs.value.floating,rhs.value.floating);
        retval.value_type = NumericValue::FLOATING;
    }

    // convert to floating if exponent or base is floating
    else if (lhs.value_type == NumericValue::EXACT &&
        rhs.value_type == NumericValue::FLOATING)
    {
        retval.value.floating = std::pow(lhs.value.exact, rhs.value.floating);
        retval.value_type = NumericValue::FLOATING;
    }
    else if (lhs.value_type == NumericValue::FLOATING &&
        rhs.value_type == NumericValue::EXACT)
    {
        retval.value.floating = std::pow(lhs.value.floating, rhs.value.exact);
        retval.value_type = NumericValue::FLOATING;
    }

    return retval;
}

