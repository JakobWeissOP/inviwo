/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2022-2023 Inviwo Foundation
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

#include <inviwo/core/io/transferfunctionxmlwriter.h>
#include <inviwo/core/io/serialization/ticpp.h>
#include <inviwo/core/util/filesystem.h>
#include <inviwo/core/io/datawriterexception.h>
#include <inviwo/core/util/stringconversion.h>
#include <inviwo/core/util/colorconversion.h>

namespace inviwo {

TransferFunctionXMLWriter::TransferFunctionXMLWriter() {
    addExtension({"xml", "XML Transfer Function"});
}

TransferFunctionXMLWriter* TransferFunctionXMLWriter::clone() const {
    return new TransferFunctionXMLWriter(*this);
};

std::string TransferFunctionXMLWriter::toXML(const TransferFunction& tf) {
    try {
        TxElement colormap("ColorMap");
        colormap.SetAttribute("space", "rgb");
        colormap.SetAttribute("name", "Inviwo TransferFunction");

        for (auto p : tf) {
            TxElement point("Point");
            point.SetAttribute<double>("x", p.getPosition());
            point.SetAttribute<double>("o", p.getAlpha());
            const auto color = p.getColor();
            point.SetAttribute<double>("r", color.r);
            point.SetAttribute<double>("g", color.g);
            point.SetAttribute<double>("b", color.b);
            colormap.InsertEndChild(point);
        }

        TxDocument doc;
        doc.InsertEndChild(TxElement("ColorMaps"))->InsertEndChild(colormap);

        TiXmlPrinter printer;
        printer.SetIndent("    ");
        doc.Accept(&printer);

        return printer.Str();

    } catch (TxException& e) {
        throw DataWriterException(e.what(), IVW_CONTEXT_CUSTOM("TransferFunctionXMLWriter"));
    }
}

void TransferFunctionXMLWriter::writeData(const TransferFunction* data,
                                          const std::filesystem::path& filePath) const {
    try {
        auto of = open(filePath);
        of << toXML(*data);
        of.close();
    } catch (TxException& e) {
        throw DataWriterException(e.what(), IVW_CONTEXT_CUSTOM("TransferFunctionXMLWriter"));
    }
};

std::unique_ptr<std::vector<unsigned char>> TransferFunctionXMLWriter::writeDataToBuffer(
    const TransferFunction* data, std::string_view) const {
    std::string xml = toXML(*data);

    auto buffer = std::make_unique<std::vector<unsigned char>>(xml.size());
    std::copy(xml.begin(), xml.end(), buffer->begin());
    return buffer;
};

}  // namespace inviwo
