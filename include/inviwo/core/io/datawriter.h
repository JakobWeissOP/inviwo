/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2012-2023 Inviwo Foundation
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

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/util/fileextension.h>
#include <inviwo/core/util/exception.h>

#include <vector>
#include <ios>
#include <any>

namespace inviwo {

enum class Overwrite { No, Yes };

/**
 * \ingroup dataio
 * @brief A abstract base class for all file writers.
 * @see DataWriterFactory InviwoApplication::getDataWriterFactory
 */
class IVW_CORE_API DataWriter {
public:
    DataWriter();
    DataWriter(const DataWriter& rhs);
    DataWriter& operator=(const DataWriter& that);
    DataWriter(DataWriter&& rhs) noexcept = default;
    DataWriter& operator=(DataWriter&& that) noexcept = default;
    virtual DataWriter* clone() const = 0;
    virtual ~DataWriter() = default;

    const std::vector<FileExtension>& getExtensions() const;
    void addExtension(FileExtension ext);

    Overwrite getOverwrite() const;
    void setOverwrite(Overwrite val);

    /**
     * Verify that you don't overwrite @p path unless @p overwrite is `Yes`.
     * @throws DataWriterException if the condition is broken.
     */
    static void checkOverwrite(const std::filesystem::path& path, Overwrite overwrite);

    /**
     * Verify that you don't overwrite @p path unless @p overwrite is `Yes`.
     * @throws DataWriterException if the condition is broken.
     */
    void checkOverwrite(const std::filesystem::path& path) const;

    /**
     * @brief Set writer specific options
     * See the documentation of the specific writer for the available options
     * @param key the option to set
     * @param value the new value for the option
     * @return true if the option was recognized and set, otherwise false
     */
    virtual bool setOption([[maybe_unused]] std::string_view key, [[maybe_unused]] std::any value) {
        return false;
    }

    /**
     * @brief Query the value of a writer specific option
     * @param key the option to query
     * @return an std::any with the requested option or an empty std::any if the option was not
     * found
     */
    virtual std::any getOption([[maybe_unused]] std::string_view key) const { return std::any{}; }

protected:
    /**
     * Open @p path in @p mode for writing. If the overwrite condition is broken or the file can't
     * be opened an exception is thrown.
     * @throws DataWriterException if the condition is broken, and FileException if the file can't
     * be opened.
     */
    std::ofstream open(const std::filesystem::path& path,
                       std::ios_base::openmode mode = std::ios_base::out) const;

    Overwrite overwrite_;
    std::vector<FileExtension> extensions_;
};

/**
 * @ingroup dataio
 * @brief Template base class for file writers designating what type of data
 * object the writer writes.
 */
template <typename T>
class DataWriterType : public DataWriter {
public:
    DataWriterType() = default;
    DataWriterType(const DataWriterType& rhs) = default;
    DataWriterType& operator=(const DataWriterType& that) = default;
    DataWriterType(DataWriterType&& rhs) noexcept = default;
    DataWriterType& operator=(DataWriterType&& that) noexcept = default;
    virtual DataWriterType* clone() const = 0;
    virtual ~DataWriterType() = default;

    /**
     * @brief Write @p data to @p filePath
     * @throws DataWriterException if anything goes wrong
     */
    virtual void writeData(const T* data, const std::filesystem::path& filePath) const = 0;

    virtual std::unique_ptr<std::vector<unsigned char>> writeDataToBuffer(
        const T* /*data*/, std::string_view /*fileExtension*/) const {
        return nullptr;
    }
};

}  // namespace inviwo
