/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2017-2023 Inviwo Foundation
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

// Automatically generated file do not change!
#include <inviwo/core/common/inviwomodulefactoryobject.h>
#include <inviwo/core/common/version.h>
#include <inviwo/devmodule/devmodulemodulesharedlibrary.h>
#include <inviwo/devmodule/devmodulemodule.h>

namespace inviwo {

InviwoModuleFactoryObject* createDevmoduleModule() {
    return new InviwoModuleFactoryObjectTemplate<DevmoduleModule>(
        "Devmodule", // Module name 
        "1.0.0", // Module version
        "All default processors", // Description
        "0.9.11.1", // Inviwo core version when built 
        {"core"}, // Dependencies
        {"1.0.0"}, // Version number of dependencies
        {}, // List of aliases
        // List of license information
        {},
        ProtectedModule::off // protected
    );
}

#ifdef INVIWO_ALL_DYN_LINK  // Prevent multiple definitions of function in static build
extern "C" {
IVW_MODULE_DEVMODULE_API InviwoModuleFactoryObject* createModule() {
    return createDevmoduleModule();
}
}
#endif

}  //namespace inviwo
