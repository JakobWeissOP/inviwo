/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2012-2019 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#pragma once

#include <inviwo/core/util/formatdispatching.h>
#include <modules/discretedata/discretedatatypes.h>

#include <tuple>
#include <type_traits>
#include <fmt/format.h>

namespace inviwo {
namespace discretedata {
namespace channeldispatching {

namespace detail {

#include <warn/push>
#include <warn/ignore/constant-conditional>

/**
 * Helper class to find the matching ind based on the given numComponents.
 * Does a binary search within specified range.
 */
template <typename Result, typename Scalar, int B, int E>
struct NumComponentDispatchHelper {
    static const int M = (B + E) / 2;

    template <typename Callable, typename... Args>
    static Result dispatch(ind numComponents, Callable&& obj, Args&&... args) {

        if constexpr (B <= E) {
            if (numComponents == M) {
#ifdef _WIN32  // TODO: remove win fix when VS does the right thing...
                return (obj.operator()<Result, Scalar, M>(std::forward<Args>(args)...));
#else
                return (obj.template operator()<Result, Scalar, M>(std::forward<Args>(args)...));
#endif
            } else if (numComponents < M) {
                return NumComponentDispatchHelper<Result, Scalar, B, M - 1>::dispatch(
                    numComponents, std::forward<Callable>(obj), std::forward<Args>(args)...);
            } else {
                return NumComponentDispatchHelper<Result, Scalar, M + 1, E>::dispatch(
                    numComponents, std::forward<Callable>(obj), std::forward<Args>(args)...);
            }
        } else {
            throw inviwo::dispatching::DispatchException(
                std::to_string(numComponents) + " components are not supportd in given range",
                IvwContextCustom("Dispatching"));
        }
    }
};

template <ind Min, ind Max>
struct CombinedDispatcher {
    template <typename Result, typename Scalar, typename Callable, typename... Args>
    auto operator()(ind numComponents, Callable&& obj, Args&&... args) -> Result {

        // Pass in the combined dispatcher as the Callable, and forward the generic Callable as a
        // parameter.
        return detail::NumComponentDispatchHelper<Result, Scalar, Min, Max>::dispatch(
            numComponents, std::forward<Callable>(obj), std::forward<Args>(args)...);
    }
};

#include <warn/pop>

}  // namespace detail

/**
 * Function for dispatching a DataFormat and dimension based on a DataFormatId and ind.
 * Both are found using binary search.
 *
 * # Template arguments:
 *  * __Result__ the return type of the lambda.
 *  * __Predicate__ A type that is used to filter the list of types to consider in the
 *    dispatching. The `dispatching::filter` namespace have a few standard ones predefined.
 *  * __Min__ Minimum dimension to consider.
 *  * __Max__ Maximal dimension to consider.
 *
 * @param callable This should be a struct with a generic call operator taking three template
 * arguments: the result type, the DataFormat type and the dimension. The callable will be called
 * with the supplied arguments (`args`).
 * @param args Any arguments that should be passed on to the lambda.
 *
 *
 * @throws dispatching::DispatchException in the case that the format of the buffer is not in
 * the list of formats after the filtering or the dimension is not within [Min, Max].
 */
template <typename Result, template <class> class Predicate, ind Min, ind Max, typename Callable,
          typename... Args>
auto dispatch(DataFormatId format, ind numComponents, Callable&& obj, Args&&... args) -> Result {

    detail::CombinedDispatcher<Min, Max> nestedDispatcher;

    // First, use the base format dispatching. Then, dispatch on dimension.
    return inviwo::dispatching::dispatch<Result, Predicate>(format, nestedDispatcher, numComponents,
                                                            std::forward<Callable>(obj),
                                                            std::forward<Args>(args)...);
}

/**
 * Function for dispatching a DataFormat and dimension based on a DataFormatId and ind.
 * Both are found using binary search.
 *
 * # Template arguments:
 *  * __Result__ the return type of the lambda.
 *  * __Predicate__ A type that is used to filter the list of types to consider in the
 *    dispatching. The `dispatching::filter` namespace have a few standard ones predefined.
 *  * __Min__ Minimum dimension to consider.
 *  * __Max__ Maximal dimension to consider.
 *
 * @param callable This should be a struct with a generic call operator taking three template
 * arguments: the result type, the DataFormat type and the dimension. The callable will be called
 * with the supplied arguments (`args`).
 * @param args Any arguments that should be passed on to the lambda.
 *
 *
 * @throws dispatching::DispatchException in the case that the format of the buffer is not in
 * the list of formats after the filtering or the dimension is not within [Min, Max].
 */
template <typename Result, template <class> class Predicate, ind Min, ind Max, typename Callable,
          typename... Args>
auto dispatch(const DataFormatBase* combinedFormat, Callable&& obj, Args&&... args) -> Result {

    detail::CombinedDispatcher<Min, Max> nestedDispatcher;

    DataFormatId scalarType =
        DataFormatBase::get(combinedFormat->getNumericType(), 1, combinedFormat->getPrecision())
            ->getId();
    size_t numComponents = combinedFormat->getComponents();
    // First, use the base format dispatching. Then, dispatch on dimension.
    return inviwo::dispatching::dispatch<Result, Predicate>(
        scalarType, nestedDispatcher, numComponents, std::forward<Callable>(obj),
        std::forward<Args>(args)...);
}

template <typename Result, int B, int E, typename Callable, typename... Args>
static Result dispatchNumber(ind number, Callable&& obj, Args&&... args) {

    if constexpr (B <= E) {
        static const int M = (B + E) / 2;
        static_assert(M >= B);
        if (number == M) {
#ifdef _WIN32  // TODO: remove win fix when VS does the right thing...
            return (obj.operator()<Result, M>(std::forward<Args>(args)...));
#else
            return (obj.template operator()<Result, M>(std::forward<Args>(args)...));
#endif
        } else if (number < M) {
            return dispatchNumber<Result, B, M - 1>(number, std::forward<Callable>(obj),
                                                    std::forward<Args>(args)...);
        } else {
            return dispatchNumber<Result, M + 1, E>(number, std::forward<Callable>(obj),
                                                    std::forward<Args>(args)...);
        }
    } else {
        throw inviwo::dispatching::DispatchException(
            fmt::format("Size {} not within range.", number), IVW_CONTEXT_CUSTOM("Dispatching"));
    }
}

/**
 *	Matches all scalar types that have an OrdinalProperty equivalent.
 */
template <typename Format>
struct OrdinalScalars : std::integral_constant<bool, Format::id() == DataFormatId::Float32 ||
                                                         Format::id() == DataFormatId::Float64 ||
                                                         Format::id() == DataFormatId::Int32 ||
                                                         Format::id() == DataFormat<size_t>::id()> {
};

/**
 *	Matches all scalar types that do NoT have an OrdinalProperty equivalent.
 */
template <typename Format>
struct NonOrdinalScalars
    : std::integral_constant<bool, Format::id() != DataFormatId::Float32 &&
                                       Format::id() != DataFormatId::Float64 &&
                                       Format::id() != DataFormatId::Int32 &&
                                       Format::id() != DataFormat<size_t>::id()> {};

}  // namespace channeldispatching
}  // namespace discretedata
}  // namespace inviwo
