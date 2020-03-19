cmake_minimum_required(VERSION 3.12)

project(bayan VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Threads)
find_package(Boost COMPONENTS program-options crc filesystem REQUIRED)
                              
set(CMAKE_CXX_STANDARD 17)

add_executable(bayan src/main.cpp src/options.h src/hash.h src/filedesc.h src/file_handler.h)

set_target_properties(bayan PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

target_link_libraries(bayan
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT jacksonrr3@yandex.ru)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
