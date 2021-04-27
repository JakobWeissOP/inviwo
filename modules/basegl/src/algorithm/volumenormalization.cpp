/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2016-2021 Inviwo Foundation
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

#include <modules/basegl/algorithm/volumenormalization.h>

#include <modules/opengl/volume/volumegl.h>
#include <modules/opengl/shader/shaderutils.h>
#include <modules/opengl/volume/volumeutils.h>
#include <modules/opengl/texture/textureutils.h>

#include <inviwo/core/datastructures/volume/volume.h>
#include <inviwo/core/datastructures/volume/volumeram.h>

namespace inviwo {

VolumeNormalization::VolumeNormalization()
    : shader_({{ShaderType::Compute, utilgl::findShaderResource("volumenormalization.comp")}},
              Shader::Build::No)
    , needsCompilation_(false) {
    shader_.getShaderObject(ShaderType::Compute)->addShaderDefine("NORMALIZE_CHANNEL_0");
    shader_.build();
}

void VolumeNormalization::setNormalizeChannel(const size_t channel, const bool normalize) {
    needsCompilation_ = true;

    if (normalize) {
        shader_.getShaderObject(ShaderType::Compute)
            ->addShaderDefine(StrBuffer{"NORMALIZE_CHANNEL_{}", channel});
    } else {
        shader_.getShaderObject(ShaderType::Compute)
            ->removeShaderDefine(StrBuffer{"NORMALIZE_CHANNEL_{}", channel});
    }
}

void VolumeNormalization::setNormalizeChannels(bvec4 normalize) {
    setNormalizeChannel(0, normalize[0]);
    setNormalizeChannel(1, normalize[1]);
    setNormalizeChannel(2, normalize[2]);
    setNormalizeChannel(3, normalize[3]);
}

void VolumeNormalization::reset() { setNormalizeChannels({true, false, false, false}); }

std::shared_ptr<Volume> VolumeNormalization::normalize(const Volume& volume) {
    std::shared_ptr<Volume> outVolume;

    outVolume = std::make_shared<Volume>(volume, noData);
    outVolume->setDataFormat(
        DataFormatBase::get(NumericType::Float, volume.getDataFormat()->getComponents(), 32));

    if (needsCompilation_) {
        needsCompilation_ = false;
        shader_.build();
    }

    const auto dimensions = volume.getDimensions();

    shader_.activate();

    TextureUnitContainer cont;
    utilgl::bindAndSetUniforms(shader_, cont, volume, "inputTexture");
    shader_.setUniform("outputTexture", 0);

    auto outVolumeGL = outVolume->getEditableRepresentation<VolumeGL>();
    outVolumeGL->setWrapping({Wrapping::Clamp, Wrapping::Clamp, Wrapping::Clamp});

    glActiveTexture(GL_TEXTURE0);

    const auto texture = outVolumeGL->getTexture();
    const auto texHandle = texture->getID();
    glBindImageTexture(0, texHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, texture->getInternalFormat());

    outVolumeGL->setSwizzleMask(swizzlemasks::rgba);

    outVolumeGL->getTexture()->bind();

    /*
     * Run normalization.
     */
    glDispatchCompute(static_cast<GLuint>(dimensions.x), static_cast<GLuint>(dimensions.y),
                      static_cast<GLuint>(dimensions.z));

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    shader_.deactivate();

    outVolume->dataMap_.dataRange = outVolume->dataMap_.valueRange = dvec2{0, 1};

    return outVolume;
}

}  // namespace inviwo
