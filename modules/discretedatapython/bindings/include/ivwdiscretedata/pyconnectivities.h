/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2019-2020 Inviwo Foundation
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

#include <warn/push>
#include <warn/ignore/shadow>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <warn/pop>

#include <modules/discretedata/connectivity/structuredgrid.h>
#include <modules/discretedata/connectivity/tripolargrid.h>
#include <fmt/format.h>

namespace inviwo {
using namespace discretedata;

namespace discretepyutil {
namespace detail {

struct IVW_MODULE_DISCRETEDATA_API CreateStructuredGridDispatcher {

    template <typename Result, int N>
    Result operator()(const pybind11::array_t<int>& vertData) {
        // for (ind n = 0; n < vertData.size(); ++n)
        //     std::cout << static_cast<ind>(*vertData.data(n)) << " ";
        // std::cout << std::endl;

        // ivwAssert(vertData.ndim() == N,
        //           fmt::format("Given data size does not match template dimension: {} != {}",
        //                       vertData.ndim(), N));

        std::array<ind, N> numVertices;
        for (ind n = 0; n < N; ++n) numVertices[n] = static_cast<ind>(*vertData.data(N - 1 - n));
        std::shared_ptr<Connectivity> grid =
            std::make_shared<StructuredGrid<ind(N)>>(std::move(numVertices));
        return grid;
    }
};

struct IVW_MODULE_DISCRETEDATA_API CreateTripolarGridDispatcher {

    template <typename Result, int N>
    Result operator()(const pybind11::array_t<int>& vertData) {
        // ivwAssert(vertData.ndim() == N,
        //           fmt::format("Given data size {} does not match template dimension {}.",
        //                       vertData.ndim(), N));

        std::array<ind, N> numVertices;
        for (ind n = 0; n < N; ++n) numVertices[n] = static_cast<ind>(*vertData.data(N - 1 - n));
        std::shared_ptr<Connectivity> grid =
            std::make_shared<TripolarGrid<ind(N)>>(std::move(numVertices));
        return grid;
    }
};

}  // namespace detail

std::shared_ptr<Connectivity> createStructuredGrid(const pybind11::array_t<int>& size);
std::shared_ptr<Connectivity> createTripolarGrid(const pybind11::array_t<int>& size);
std::shared_ptr<Connectivity> createPointCloudGrid(const pybind11::array_t<int>& size);
}  // namespace discretepyutil

void exposeConnectivities(pybind11::module& m);

}  // namespace inviwo
