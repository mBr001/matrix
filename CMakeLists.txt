cmake_minimum_required(VERSION 3.8.2)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(matrix VERSION 1.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(matrix VERSION 1.0.0)
endif()

add_executable(matrix main.cpp)

set_target_properties(matrix PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

################################################################################

install(TARGETS matrix RUNTIME DESTINATION bin)

################################################################################

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_NAME matrix)
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Anton Sochnev <an.sochnev@yandex.ru>")
set(CPACK_DEBIAN_PACKAGE_SECTION "homework")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION 
"some description should be here
 And here."
)
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION})

include(CPack)

