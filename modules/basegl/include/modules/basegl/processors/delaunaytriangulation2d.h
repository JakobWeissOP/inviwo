/*********************************************************************************
*
* Inviwo - Interactive Visualization Workshop
*
* Copyright (c) 2013-2018 Inviwo Foundation
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

#ifndef IVW_DELAUNAYTRIANGULATION2D_H
#define IVW_DELAUNAYTRIANGULATION2D_H

#include <modules/basegl/baseglmoduledefine.h>

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/datastructures/geometry/simplemesh.h>

#include <inviwo/core/ports/polylineport.h>
#include <inviwo/core/ports/meshport.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/optionproperty.h>

#include <modules/basegl/algorithm/vec2_indexed.h>

#include <vector>
#include <memory>

namespace inviwo {
    /**
    * \class
    * \brief 
    */
    class IVW_MODULE_BASEGL_API DelaunayTriangulation2D : public Processor {
    public:
        DelaunayTriangulation2D();
        virtual ~DelaunayTriangulation2D() = default;

        virtual void process() override;

        virtual const ProcessorInfo getProcessorInfo() const override;
        static const ProcessorInfo processorInfo_;

    protected:

    private:
        PolylineInport ptsInport_;
        MeshOutport meshOutport_;
        MeshOutport meshEdgesOutport_;

        OptionPropertyInt axisOfProjection_;
        FloatVec3Property volumeVoxelSpacing_;
        BoolProperty triangulateInViewSpace_;
        CameraProperty camera_;

        FloatVec3Property planeNormal_;

        std::vector<Vec2Indexed> setupPoints2D(const std::vector<vec3>& pts_3d);
        std::pair<std::shared_ptr<SimpleMesh>, std::shared_ptr<SimpleMesh>> createMeshFrom2dPts(
            const std::vector<Vec2Indexed>& pts_2d,
            const std::vector<vec3>& pts_3d) const;
    };
}

#endif // IVW_DELAUNAYTRIANGULATION2D_H