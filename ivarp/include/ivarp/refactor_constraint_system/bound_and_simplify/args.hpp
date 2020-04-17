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
// Created by Phillip Keldenich on 20.01.20.
//

#pragma once

namespace ivarp {
namespace impl {
    template<typename ArgIndex, typename ArgBounds> struct BoundAndSimplify<MathArg<ArgIndex>, ArgBounds, void> {
        using OldType = MathArg<ArgIndex>;
        static constexpr std::size_t index = ArgIndex::value;

        static inline IVARP_H auto apply(OldType&& old) noexcept {
            static_assert(!std::is_lvalue_reference<OldType>::value, "Non-rvalue passed into bound_and_simplify!");

            using ArgBound = typename ArgBounds::template At<index>;
            using BoundType = ExpressionBounds<ArgBound::lb, ArgBound::ub>;
            return BoundedMathExpr<OldType, BoundType>{std::forward<OldType>(old)};
        }
    };
}
}
