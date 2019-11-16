cmake_minimum_required(VERSION 3.7)

if(NOT DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(ENV{TRAVIS_BUILD_NUMBER} 777)
endif()

project(allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

configure_file(version.h.in ${PROJECT_SOURCE_DIR}/version.h)

add_executable(allocator allocator.cpp container.h block_allocator.h hard.h easy.h utility.h)

set_target_properties(allocator PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
        )

target_link_libraries(allocator
    Threads::Threads
)

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT "SergeiNA at github.com")

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)