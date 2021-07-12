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

#include <modules/discretedata/channels/datachannel.h>
#include <inviwo/core/datastructures/spatialdata.h>

namespace inviwo {
namespace discretedata {

template <typename T, ind N>
class SpatialEntityChannel : public SpatialEntity<(unsigned int)N> {
public:
    SpatialEntityChannel(std::shared_ptr<const DataChannel<T, N>> channel)
        : SpatialEntity<(unsigned int)N>(), channel_(channel) {
        std::cout << "Created Entity Channel" << std::endl;
    }
    SpatialEntityChannel(const SpatialEntityChannel& other)
        : SpatialEntity<(unsigned int)N>(other), channel_(other.channel_) {
        std::cout << "Copied Entity Channel" << std::endl;
    }
    SpatialEntityChannel& operator=(const SpatialEntityChannel& other) = delete;

    virtual SpatialEntityChannel<T, N>* clone() const {
        std::cout << "Cloned Entity Channel" << std::endl;
        return new SpatialEntityChannel<T, N>(*this);
    }

    virtual const SpatialCoordinateTransformer<N>& getCoordinateTransformer() const override {
        std::cout << "Spatial Entity Channel get Coordinate Transformer" << std::endl;
        return SpatialEntity<(unsigned int)N>::getCoordinateTransformer();
    }

    const std::shared_ptr<const DataChannel<T, N>> channel_;
};
}  // namespace discretedata
}  // namespace inviwo