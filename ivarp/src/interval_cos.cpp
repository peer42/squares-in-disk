// The code is open source under the MIT license.
// Copyright 2019-2020, Phillip Keldenich, TU Braunschweig, Algorithms Group
// https://ibr.cs.tu-bs.de/alg
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Created by Phillip Keldenich on 22.10.19.
//

#include "sincos_period_info.hpp"
#include "mpfr_helpers.hpp"

namespace ivarp {
namespace impl {
    /// Implementation of rounded cosine for nonnegative floating-point values.
    template<bool RoundUp, typename F> static inline
        std::enable_if_t<std::is_floating_point<F>::value, F> round_cos(F x, unsigned /*precision_ignored*/)
    {
        MPFR_DECL_INIT(mx, std::numeric_limits<F>::digits); // NOLINT
        mpfr_set_fltp(mx, x, RoundUp ? MPFR_RNDD : MPFR_RNDU); // rounding should only occur for subnormals
                                                               // (i.e. very close to zero, where
                                                               // rounddown is correct if we want to round up)
        mpfr_cos(mx, mx, RoundUp ? MPFR_RNDU : MPFR_RNDD);
        return mpfr_get_fltp<F>(mx, RoundUp ? MPFR_RNDU : MPFR_RNDD);
    }

    /// Implementation of rounded cosine for nonnegative rational values.
    template<bool RoundUp> static inline Rational round_cos(const Rational& r, unsigned precision) {
        return round_mpfr_op_rational<mpfr_cos, RoundUp>(r, precision);
    }

    /// Implementation of interval cosine for interval that do not wrap across a multiple of 2pi.
    template<typename FloatType> static inline Interval<FloatType>
        interval_cos_nowrap(const PeriodInfo<FloatType>& period, Interval<FloatType> x, unsigned precision)
    {
        if(period.lb_period_flt <= 0.5) {
            if(period.ub_period_flt <= 0.5) {
                return Interval<FloatType>{
                        round_cos<false>(x.ub(), precision),
                        round_cos<true>(x.lb(), precision),
                        x.possibly_undefined()
                };
            } else {
                return Interval<FloatType>{
                        FloatType(-1),
                        (std::max)(round_cos<true>(x.ub(), precision), round_cos<true>(x.lb(), precision)),
                        x.possibly_undefined()
                };
            }
        } else {
            return Interval<FloatType>{
                    round_cos<false>(x.lb(), precision),
                    round_cos<true>(x.ub(), precision),
                    x.possibly_undefined()
            };
        }
    }

    /// Implementation of interval cosine for intervals for which the lower bound is one 2pi period before the upper bound.
    template<typename NumberType> static inline
        Interval<NumberType>
            interval_cos_wrap(const PeriodInfo<NumberType>& period, Interval<NumberType> x, unsigned precision)
    {
        if(period.lb_period_flt <= 0.5 || period.ub_period_flt >= 0.5) {
            return Interval<NumberType>{-1, 1, x.possibly_undefined()};
        } else {
            return Interval<NumberType>{
                    (std::min)(round_cos<false>(x.lb(), precision), round_cos<false>(x.ub(), precision)),
                    1, x.possibly_undefined()
            };
        }
    }

    /// Implementation of cosine for non-negative intervals.
    template<typename NumberType> static inline Interval<NumberType>
        interval_cos_nonnegative(const Interval<NumberType>& x,
                                 unsigned precision)
    {
        PeriodInfo<NumberType> period = get_period_info(x, precision);

        // the bounds are not definitely in adjacent periods
        if(opacify(period.lb_period_int + NumberType(1)) < period.ub_period_int) {
            return Interval<NumberType>{NumberType(-1), NumberType(1), x.possibly_undefined()};
        }

        if(period.lb_period_int == period.ub_period_int) {
            // no wrap-around
            return interval_cos_nowrap(period, x, precision);
        } else {
            // wrap-around
            return interval_cos_wrap(period, x, precision);
        }
    }

    template<typename Number> static inline
        Interval<Number> interval_cos_symm(const Interval<Number>& x, unsigned precision)
    {
        using IV = Interval<Number>;

        if(!is_finite(x)) {
            return IV{-1, 1, x.possibly_undefined()};
        }

        if(x.ub() <= 0) {
            return interval_cos_nonnegative(-x, precision);
        } else if(x.lb() < 0) {
            Number mx = -x.lb();
            if(mx < x.ub()) {
                mx = x.ub();
            }
            return interval_cos_nonnegative(IV{0, mx}, precision);
        } else {
            return interval_cos_nonnegative(x, precision);
        }
    }

    IFloat  builtin_interval_cos(IFloat x) {
        return interval_cos_symm(x, 0);
    }

    IDouble builtin_interval_cos(IDouble x) {
        return interval_cos_symm(x, 0);
    }

    IRational rational_interval_cos(const IRational& x, unsigned precision) {
        return interval_cos_symm(x, precision);
    }
}
}
