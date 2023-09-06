cmake_minimum_required(VERSION 3.23...3.27 FATAL_ERROR)
project(devmodule
    HOMEPAGE_URL https://github.com/inviwo/inviwo
    LANGUAGES CXX
)
list(APPEND CMAKE_PREFIX_PATH 
    /Users/peter/Documents/Inviwo/build/inviwo-vcpkg-dynamic/_CPack_Packages/Darwin/TGZ/Inviwo-v0.9.11.1/Inviwo.app/Contents/share/inviwo
    /Users/peter/Documents/Inviwo/build/inviwo-vcpkg-dynamic/vcpkg_installed/arm64-osx-dynamic/share/fmt
    /Users/peter/Documents/Inviwo/build/inviwo-vcpkg-dynamic/vcpkg_installed/arm64-osx-dynamic/share/roaring
    /Users/peter/Documents/Inviwo/build/inviwo-vcpkg-dynamic/vcpkg_installed/arm64-osx-dynamic/share/warn
    /Users/peter/Documents/Inviwo/build/inviwo-vcpkg-dynamic/vcpkg_installed/arm64-osx-dynamic/share/utf8cpp
    /Users/peter/Documents/Inviwo/build/inviwo-vcpkg-dynamic/vcpkg_installed/arm64-osx-dynamic/share/units
    /Users/peter/Documents/Inviwo/build/inviwo-vcpkg-dynamic/vcpkg_installed/arm64-osx-dynamic/share/tclap
    /Users/peter/Documents/Inviwo/build/inviwo-vcpkg-dynamic/vcpkg_installed/arm64-osx-dynamic/share/glm
)

option(BUILD_SHARED_LIBS "Build shared libs, else static libs" ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)


add_library(inviwo-module-devmodule)

target_compile_definitions(inviwo-module-devmodule
    PRIVATE
        INVIWO_ALL_DYN_LINK
)

set_target_properties(inviwo-module-devmodule
     PROPERTIES 
     DEFINE_SYMBOL IVW_MODULE_DEVMODULE_EXPORTS
)

target_sources(inviwo-module-devmodule
    PUBLIC
    FILE_SET HEADERS
    TYPE HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES
        include/inviwo/devmodule/devmodulemodule.h
        include/inviwo/devmodule/devmodulemodulesharedlibrary.h
        include/inviwo/devmodule/devmodulemoduledefine.h
        include/inviwo/devmodule/processors/exampleprocessor.h
    PRIVATE
        src/devmodulemodule.cpp
        src/devmodulemodulesharedlibrary.cpp
        src/processors/exampleprocessor.cpp
)

find_package(inviwo-core)
#find_package(inviwo-module-base)

target_link_libraries(inviwo-module-devmodule
    PUBLIC
        inviwo::core
#        inviwo::module::base
)
