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

#include <modules/discretedata/discretedatamoduledefine.h>
#include <modules/discretedata/ports/datasetport.h>
#include <modules/discretedata/dataset.h>
#include <modules/discretedata/channels/channel.h>
#include <inviwo/core/common/inviwo.h>

#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/optionproperty.h>

namespace inviwo {

namespace discretedata {

using GridPrimitiveProperty = TemplateOptionProperty<GridPrimitive>;

class IVW_MODULE_DISCRETEDATA_API DataChannelProperty : public CompositeProperty {
public:
    using ChannelFilter = std::function<bool(const std::shared_ptr<const Channel>)>;
    static bool FilterPassAll(const std::shared_ptr<const Channel>) { return true; }

    // Methods
public:
    // static const std::string classIdentifier;
    virtual std::string getClassIdentifier() const override {
        return "inviwo.discretedata.datachannelproperty";
    }

    DataChannelProperty(DataSetInport& dataInport, const std::string& identifier,
                        const std::string& displayName, ChannelFilter filter = &FilterPassAll,
                        InvalidationLevel invalidationLevel = InvalidationLevel::InvalidOutput,
                        PropertySemantics semantics = PropertySemantics::Default);
    virtual DataChannelProperty* clone() const override { return new DataChannelProperty(*this); }
    virtual ~DataChannelProperty() {}

    virtual void updateChannelList();
    std::shared_ptr<const Channel> getCurrentChannel();

    DataSetInport& datasetInput_;
    ChannelFilter channelFilter_;
    OptionPropertyString channelName_;
    GridPrimitiveProperty gridPrimitive_;

private:
    //std::shared_ptr<std::function<void()>> gridPrimitiveOnChange_;
    bool ongoingChange_ = false;
};

}  // namespace discretedata
}  // namespace inviwo
