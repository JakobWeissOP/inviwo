/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2022 Inviwo Foundation
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

#include <modules/discretedata/processors/changeinvalidvalue.h>
#include <modules/discretedata/channels/analyticchannel.h>

namespace inviwo {
namespace discretedata {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo ChangeInvalidValue::processorInfo_{
    "org.inviwo.ChangeInvalidValue",  // Class identifier
    "Change Invalid Value",           // Display name
    "Undefined",                      // Category
    CodeState::Experimental,          // Code state
    Tags::None,                       // Tags
};
const ProcessorInfo ChangeInvalidValue::getProcessorInfo() const { return processorInfo_; }

ChangeInvalidValue::ChangeInvalidValue() : Processor(), dataIn_("dataIn"), dataOut_("dataOut") {

    addPorts(dataIn_, dataOut_);
}

void ChangeInvalidValue::process() {
    if (!dataIn_.hasData()) {
        dataOut_.clear();
        return;
    }

    auto data = dataIn_.getData();
    if (!data) {
        dataOut_.clear();
        return;
    }

    auto newData = std::make_shared<DataSet>(data->getName(), data->getGrid());

    // Update properties displaying values.
    for (Property* prop : this->getProperties()) {
        prop->setReadOnly(true);
    }
    for (auto channel : data->getChannels()) {
        auto format = channel.second->getDataFormatId();
        if (format == DataFormatId::Float32 || format == DataFormatId::Float64 ||
            format == DataFormatId::Int32 || format == DataFormat<size_t>::id())
            Channel::dispatchSharedPointer<void, channeldispatching::OrdinalScalars, 1,
                                           DISCRETEDATA_MAX_NUM_DIMENSIONS>(
                channel.second, detail::UpdatePropertyAndChannelDispatcher(), this, newData.get());
    }
    std::vector<Property*> allProps(this->getProperties());
    for (Property* prop : allProps) {
        if (prop->getReadOnly()) this->removeProperty(prop);
    }

    for (auto& sampler : data->getSamplers()) {
        if (!newData->addSampler(sampler.second)) {
            // TODO: Make new sampler because underlying channel changed -____-
        }
        std::cout << "Invalid value Proc: Added sampler " << sampler.first << std::endl;
    }

    dataOut_.setData(newData);
}

}  // namespace discretedata
}  // namespace inviwo