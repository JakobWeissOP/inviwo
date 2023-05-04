/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2019-2023 Inviwo Foundation
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

#include <inviwo/dataframeqt/processors/dataframetable.h>

#include <inviwo/core/io/datawriterutil.h>                             // for saveData
#include <inviwo/core/metadata/processorwidgetmetadata.h>              // for ProcessorWidgetMet...
#include <inviwo/core/network/networklock.h>                           // for NetworkLock
#include <inviwo/core/processors/processor.h>                          // for Processor
#include <inviwo/core/processors/processorstate.h>                     // for CodeState, CodeSta...
#include <inviwo/core/processors/processortags.h>                      // for Tags, Tag, Tags::CPU
#include <inviwo/core/processors/processorwidget.h>                    // for ProcessorWidget
#include <inviwo/core/properties/boolproperty.h>                       // for BoolProperty
#include <inviwo/core/properties/invalidationlevel.h>                  // for InvalidationLevel
#include <inviwo/core/properties/ordinalproperty.h>                    // for IntSize2Property
#include <inviwo/core/properties/property.h>                           // for Property, Property...
#include <inviwo/core/properties/propertysemantics.h>                  // for PropertySemantics
#include <inviwo/core/properties/valuewrapper.h>                       // for PropertySerializat...
#include <inviwo/core/util/exception.h>                                // for Exception
#include <inviwo/core/util/sourcecontext.h>                            // for IVW_CONTEXT
#include <inviwo/core/util/statecoordinator.h>                         // for StateCoordinator
#include <inviwo/dataframe/datastructures/dataframe.h>                 // for DataFrameInport
#include <inviwo/dataframeqt/dataframetableprocessorwidget.h>          // for DataFrameTableProc...
#include <modules/brushingandlinking/ports/brushingandlinkingports.h>  // for BrushingAndLinking...

#include <functional>  // for __base
#include <limits>      // for numeric_limits
#include <map>         // for map
#include <utility>     // for move

#include <glm/vec2.hpp>  // for operator!=

namespace inviwo {
class FileExtension;

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo DataFrameTable::processorInfo_{
    "org.inviwo.DataFrameTable",   // Class identifier
    "DataFrame Table",             // Display name
    "Data Output",                 // Category
    CodeState::Stable,             // Code state
    Tags::CPU | Tag{"DataFrame"},  // Tags
};
const ProcessorInfo DataFrameTable::getProcessorInfo() const { return processorInfo_; }

DataFrameTable::DataFrameTable()
    : Processor()
    , inport_("inport")
    , brushLinkPort_("brushingAndLinking")

    , dimensions_("dimensions", "Canvas Size", size2_t(512, 300), size2_t(1, 1),
                  size2_t(10000, 10000), size2_t(1, 1), InvalidationLevel::Valid)
    , position_("position", "Canvas Position", ivec2(128, 128),
                ivec2(std::numeric_limits<int>::lowest()), ivec2(std::numeric_limits<int>::max()),
                ivec2(1, 1), InvalidationLevel::Valid, PropertySemantics::Text)
    , showIndexColumn_("showIndexColumn", "Show Index Column", false, InvalidationLevel::Valid)
    , showCategoryIndices_("showCategoryIndices", "Show Category Indices", false)
    , showFilteredRowCols_("showFilteredItems", "Show Filtered Items", true)
    , widgetMetaData_{
          createMetaData<ProcessorWidgetMetaData>(ProcessorWidgetMetaData::CLASS_IDENTIFIER)} {
    widgetMetaData_->addObserver(this);

    addPort(inport_);
    addPort(brushLinkPort_);
    addProperties(dimensions_, position_, showIndexColumn_, showCategoryIndices_,
                  showFilteredRowCols_);

    dimensions_.setSerializationMode(PropertySerializationMode::None);
    dimensions_.onChange([this]() { widgetMetaData_->setDimensions(dimensions_.get()); });
    position_.onChange([this]() { widgetMetaData_->setPosition(position_.get()); });

    showIndexColumn_.onChange([this]() {
        if (auto w = getWidget()) {
            w->setIndexColumnVisible(showIndexColumn_);
        }
    });
    showFilteredRowCols_.onChange([this]() {
        if (auto w = getWidget()) {
            w->setFilteredRowsVisible(showFilteredRowCols_);
        }
    });
}

DataFrameTable::~DataFrameTable() {
    if (processorWidget_) {
        processorWidget_->setVisible(false);
    }
}

void DataFrameTable::process() {
    if (auto w = getWidget()) {
        if (inport_.isChanged() || showCategoryIndices_.isModified()) {
            w->setDataFrame(inport_.getData(), showCategoryIndices_);
        }
        w->brushingUpdate();
    }
}

void DataFrameTable::setProcessorWidget(std::unique_ptr<ProcessorWidget> processorWidget) {
    auto widget = dynamic_cast<DataFrameTableProcessorWidget*>(processorWidget.get());
    if (processorWidget && !widget) {
        throw Exception(
            "Expected DataFrameTableProcessorWidget in DataFrameTable::setProcessorWidget",
            IVW_CONTEXT);
    }

    if (widget) {
        widget->setManager(brushLinkPort_.getManager());
        widget->setIndexColumnVisible(showIndexColumn_);
        widget->setFilteredRowsVisible(showFilteredRowCols_);
    }

    Processor::setProcessorWidget(std::move(processorWidget));
    isSink_.update();
    isReady_.update();
}

void DataFrameTable::onProcessorWidgetPositionChange(ProcessorWidgetMetaData*) {
    if (widgetMetaData_->getPosition() != position_.get()) {
        Property::OnChangeBlocker blocker{position_};
        position_.set(widgetMetaData_->getPosition());
    }
}

void DataFrameTable::onProcessorWidgetDimensionChange(ProcessorWidgetMetaData*) {
    if (widgetMetaData_->getDimensions() != dimensions_.get()) {
        Property::OnChangeBlocker blocker{dimensions_};
        dimensions_.set(widgetMetaData_->getDimensions());
    }
}

void DataFrameTable::onProcessorWidgetVisibilityChange(ProcessorWidgetMetaData*) {
    isSink_.update();
    isReady_.update();
    invalidate(InvalidationLevel::InvalidOutput);
}

DataFrameTableProcessorWidget* DataFrameTable::getWidget() const {
    if (auto widget = static_cast<DataFrameTableProcessorWidget*>(processorWidget_.get())) {
        return widget;
    } else {
        return nullptr;
    }
}

void DataFrameTable::setWidgetSize(size2_t dim) {
    NetworkLock lock(this);
    dimensions_.set(dim);
}

size2_t DataFrameTable::getWidgetSize() const { return dimensions_; }

std::optional<std::filesystem::path> DataFrameTable::exportFile(
    const std::filesystem::path& path, std::string_view name,
    const std::vector<FileExtension>& candidateExtensions, Overwrite overwrite) const {

    if (auto data = inport_.getData()) {
        return util::saveData(*data, path, name, candidateExtensions, overwrite);
    }

    throw Exception("Inport has no data", IVW_CONTEXT);
}

}  // namespace inviwo
