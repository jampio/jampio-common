cmake_minimum_required(VERSION 3.10)
project(jampiocommon)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(CheckTypeSize)
check_type_size("void*" CMAKE_SIZEOF_VOID_P)
message(STATUS "sizeof void*: " ${CMAKE_SIZEOF_VOID_P})
message(STATUS "CMAKE_CROSSCOMPILING: " ${CMAKE_CROSSCOMPILING})
message(STATUS "CMAKE_SYSTEM_PROCESSOR: " ${CMAKE_SYSTEM_PROCESSOR})
message(STATUS "CMAKE_LIBRARY_ARCHITECTURE: " ${CMAKE_LIBRARY_ARCHITECTURE})
find_package(jampioshared REQUIRED)
find_package(PkgConfig REQUIRED)
message(STATUS "PKG_CONFIG_PATH: " $ENV{PKG_CONFIG_PATH})
pkg_check_modules(ZIP zlib REQUIRED)
message(STATUS "  in path: " ${ZIP_LIBDIR})
pkg_check_modules(MINIZIP minizip REQUIRED)
message(STATUS "  in path: " ${MINIZIP_LIBDIR})
set(SOURCES
	src/cm/load.cpp
	src/cmd.cpp
	src/CommandLine.cpp
	src/common.cpp
	src/cvar.cpp
	src/files.cpp
)
add_library(jampiocommonpc STATIC ${SOURCES})
add_library(jampiocommonded STATIC ${SOURCES})
target_compile_definitions(jampiocommonded PUBLIC DEDICATED)
target_include_directories(jampiocommonpc PRIVATE ${ZLIB_INCLUDE_DIR} ${MINIZIP_INCLUDE_DIR})
target_include_directories(jampiocommonded PRIVATE ${ZLIB_INCLUDE_DIR} ${MINIZIP_INCLUDE_DIR})
target_include_directories(jampiocommonpc PUBLIC ${jampioshared_INCLUDE})
target_include_directories(jampiocommonded PUBLIC ${jampioshared_INCLUDE})
target_link_libraries(jampiocommonpc jampioshared)
target_link_libraries(jampiocommonded jampioshared)
# minizip static libs already contain zlib
target_link_libraries(jampiocommonpc ${MINIZIP_STATIC_LIBRARIES})
target_link_libraries(jampiocommonded ${MINIZIP_STATIC_LIBRARIES})
# custom install enforces lib/<arch> on certain systems
if(USE_CUSTOM_INSTALL)
	include(GNUInstallDirsCustom)
else()
	include(GNUInstallDirs)
endif()
install(TARGETS jampiocommonpc EXPORT jampiocommonpcConfig DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS jampiocommonded EXPORT jampiocommondedConfig DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jampio/common FILES_MATCHING PATTERN "*.h")
install(EXPORT jampiocommonpcConfig DESTINATION ${CMAKE_INSTALL_LIBDIR}/jampiocommonpc)
install(EXPORT jampiocommondedConfig DESTINATION ${CMAKE_INSTALL_LIBDIR}/jampiocommonded)