/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2012-2021 Inviwo Foundation
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

#include <modules/discretedata/dataset.h>
#include <modules/discretedata/channels/bufferchannel.h>
#include <inviwo/core/util/utilities.h>

namespace inviwo {
namespace discretedata {

bool DataSet::hasChannel(std::shared_ptr<const Channel> channel) const {
    for (auto& c : channels_) {
        if (c.second == channel) return true;
    }
    return false;
}

DataChannelMap DataSet::getChannels() const { return DataChannelMap(channels_); }

std::shared_ptr<const Channel> DataSet::addChannel(const Channel* channel) {
    std::shared_ptr<const Channel> sharedChannel(channel);
    addChannel(sharedChannel);

    return sharedChannel;
}

void DataSet::addChannel(std::shared_ptr<const Channel> sharedChannel) {
    util::validateIdentifier(sharedChannel->getName(), "Channel", IVW_CONTEXT);
    channels_.insert(std::make_pair(
        std::make_pair(sharedChannel->getName(), sharedChannel->getGridPrimitiveType()),
        sharedChannel));
}

bool DataSet::removeChannel(std::shared_ptr<const Channel> channel) {
    return channels_.erase(std::make_pair(channel->getName(), channel->getGridPrimitiveType())) !=
           0;
}

std::shared_ptr<const Channel> DataSet::getFirstChannel() const {
    auto it = channels_.begin();

    if (it == channels_.end()) return std::shared_ptr<const Channel>();

    return it->second;
}

std::shared_ptr<const Channel> DataSet::getChannel(const std::string& name,
                                                   const GridPrimitive definedOn) const {
    auto key = std::make_pair(name, definedOn);
    return getChannel(key);
}

std::shared_ptr<const Channel> DataSet::getChannel(
    const std::pair<std::string, GridPrimitive>& key) const {
    auto it = channels_.find(key);

    if (it == channels_.end()) return std::shared_ptr<const Channel>();

    return it->second;
}

std::vector<std::pair<std::string, GridPrimitive>> DataSet::getChannelNames() const {
    ind numChannels = size();

    std::vector<std::pair<std::string, GridPrimitive>> channelNames;
    channelNames.reserve(numChannels);
    for (auto& key : channels_) {
        channelNames.push_back(key.first);
    }

    return channelNames;
}

const SamplerMap& DataSet::getSamplers() const { return samplers_; }

bool DataSet::addSampler(std::shared_ptr<const DataSetSamplerBase> sampler) {
    if (!sampler) LogError("Sampler null");
    if (sampler->getDimension() != static_cast<unsigned int>(grid_->getDimension()))
        LogError("Sampler and Grid not same dimension.");

    if (!hasChannel(sampler->coordinates_)) LogError("That channel is not in this dataset");
    util::validateIdentifier(sampler->coordinates_->getName(), "DataSetSampler", IVW_CONTEXT);
    if (!sampler || sampler->getDimension() != static_cast<unsigned int>(grid_->getDimension()) ||
        !hasChannel(sampler->coordinates_)) {
        LogError("Could not add sampler.");
        if (!sampler) LogError("  Sampler is null");
        if (sampler && sampler->getDimension() != static_cast<unsigned int>(grid_->getDimension()))
            LogError(fmt::format("  Wrong format: {} != {}", sampler->getDimension(),
                                 static_cast<unsigned int>(grid_->getDimension())));
        return false;
    }

    samplers_.insert({sampler->getIdentifier(), sampler});
    LogInfo("Added new sampler: " << sampler->getIdentifier());
    return true;
}

}  // namespace discretedata
}  // namespace inviwo
