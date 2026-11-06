# rythme for a web browser (WebAssembly, Emscripten). Windows builds it from rythme.vcxproj
# (build.ps1); this file is the web one, run by build_web.ps1 against kazilib's SFML web
# build (kazilib web/build_deps.ps1):
#   emcmake cmake -S src/client -B <build> -G Ninja -DSFML_DIR=...
# Output: index.html / .js / .wasm - the folder kazin.dev/projects/rythme serves.
cmake_minimum_required(VERSION 3.21)
project(rythme_web CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RYTHME_SRC ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(SFML_STATIC_LIBRARIES ON)
find_package(SFML 3 REQUIRED COMPONENTS Graphics)
include(${RYTHME_SRC}/dep/kazilib/web/kazilib_web.cmake)

file(GLOB RYTHME_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(FILTER RYTHME_SOURCES EXCLUDE REGEX "pch\.cpp$")

add_executable(rythme ${RYTHME_SOURCES})
kazilib_web_target(rythme)
kazilib_web_imgui(rythme)
target_include_directories(rythme PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_precompile_headers(rythme PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/pch.h)
# kazilib's sources build without rythme's header
set_source_files_properties(${KAZILIB_WEB_IMGUI_SOURCES} ${KAZILIB_WEB_SOURCES} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
target_link_libraries(rythme PRIVATE SFML::Graphics)
