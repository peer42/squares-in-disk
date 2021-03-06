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
// Created by Phillip Keldenich on 22.11.19.
//

#pragma once

namespace ivarp {
namespace impl {
    /// Implementation of evaluation for binary predicates (forward to implementation in tag).
    template<typename Context, typename Tag, typename A1, typename A2, typename ArgArray>
        struct PredicateEvaluateImpl<Context, BinaryMathPred<Tag, A1, A2>, ArgArray, false>
    {
        using NumberType = typename Context::NumberType;
        using CalledType = BinaryMathPred<Tag, A1, A2>;

        IVARP_HD_OVERLOAD_ON_CUDA_NT(NumberType,
            static inline auto eval(const CalledType& c, const ArgArray& args) noexcept(AllowsCUDA<NumberType>::value) {
                return invoke_tag<Tag, Context>(args, c.arg1, c.arg2);
            }
        )
    };
}
}
