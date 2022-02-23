/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2012-2020 Inviwo Foundation
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

#include <modules/discretedata/sampling/extrudeddatasetsampler.h>
#include <modules/discretedata/channels/channeldispatching.h>
#include <inviwo/core/util/zip.h>

#include <stack>

namespace inviwo {
namespace discretedata {
template <unsigned int SpatialDims, unsigned int BaseDims>
ExtrudedDataSetSampler<SpatialDims, BaseDims>::ExtrudedDataSetSampler(
    std::shared_ptr<const Connectivity> grid,
    std::shared_ptr<const DataChannel<double, SpatialDims>> coordinates,
    const Interpolant<SpatialDims, BaseDims>& interpolant, const std::array<float, SpatialDims>& coordsMin,
    const std::array<float, SpatialDims>& coordsMax)
    : ExtrudedDataSetSamplerBase(grid, std::static_pointer_cast<const Channel>(coordinates))
    , interpolant_(interpolant.copy())
    , coordsMin_(coordsMin)
    , coordsMax_(coordsMax) {
    if (!coordinates_) std::cout << "Oopsie doo coordinates!" << std::endl;
    if (!grid_) std::cout << "Oopsie doo grid!" << std::endl;
    // if (coordinates_->getGridPrimitiveType() != GridPrimitive::Vertex ||
    //     coordinates_->getNumComponents() != SpatialDims ||
    //     coordinates_->size() != grid->getNumElements()) {
    //     LogError("Incompatible grid and coordinate channel given, aborting.");
    //     return;
    // }
    // }

    Matrix<SpatialDims + 1, float> modelMat;
    for (unsigned dim = 0; dim < SpatialDims; ++dim) {
        modelMat[dim][dim] = coordsMax[dim] - coordsMin[dim];
        modelMat[SpatialDims][dim] = coordsMin[dim];
        // std::cout << "# " << coordsMin[dim] << " - " << coordsMax[dim] << std::endl;
    }
    modelMat[SpatialDims][SpatialDims] = 1;
    this->setModelMatrix(modelMat);
}

template <unsigned int SpatialDims, unsigned int BaseDims>
ExtrudedDataSetSampler<SpatialDims, BaseDims>::~ExtrudedDataSetSampler() {
    delete interpolant_;
}

template <unsigned int SpatialDims, unsigned int BaseDims>
ExtrudedDataSetSampler<SpatialDims, BaseDims>::ExtrudedDataSetSampler(ExtrudedDataSetSampler&& tree)
    : ExtrudedDataSetSampler(tree.grid_, tree.coordinates_) {}

template <unsigned int SpatialDims, unsigned int BaseDims>
void ExtrudedDataSetSampler<SpatialDims, BaseDims>::setInterpolant(
    const Interpolant<SpatialDims, BaseDims>& interpolant) {
    interpolant_ = interpolant.copy();
}

template <unsigned int SpatialDims, unsigned int BaseDims>
const Interpolant<SpatialDims, BaseDims>& ExtrudedDataSetSampler<SpatialDims, BaseDims>::getInterpolant() const {
    return *interpolant_;
}

}  // namespace discretedata
}  // namespace inviwo
