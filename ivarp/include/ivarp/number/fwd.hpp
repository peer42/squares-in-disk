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
// Created by Phillip Keldenich on 25.11.19.
//

#pragma once

namespace ivarp {
    template<typename NT> struct NumberTraits;
    template<typename T> struct IsRationalImpl;

    template<typename T> using IsRational = IsRationalImpl<BareType<T>>;

    template<typename TargetType, typename SourceType> static inline
        std::enable_if_t<!std::is_integral<SourceType>::value, TargetType> convert_number(const SourceType& source);

    template<typename TargetType, typename SourceType> static inline
        std::enable_if_t<std::is_integral<SourceType>::value && std::is_signed<SourceType>::value, TargetType>
            convert_number(const SourceType& source);

    template<typename TargetType, typename SourceType> static inline
        std::enable_if_t<std::is_integral<SourceType>::value && !std::is_signed<SourceType>::value, TargetType>
            convert_number(const SourceType& source);

    using BigInt = mpz_class;
    using Rational = mpq_class;
}
