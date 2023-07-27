cmake_minimum_required(VERSION 3.0)
project(rust-interop C)

# Enable ExternalProject CMake module
include(ExternalProject)

# Set default ExternalProject root directory
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/Rust)

message("\nPLATFORM_DREAMCAST: ${PLATFORM_DREAMCAST}")

set(CMAKE_C_STANDARD 99)

include_directories(src)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 --fast-math")

set(
    SOURCES
        src/main.c
)

if(ENGINE_DEBUG)
    add_definitions(-DENGINE_DEBUG)
endif()

if(PLATFORM_DREAMCAST)
    link_directories(rust/game-code/target/debug/)
    link_libraries(game_code)

    add_definitions(-DDREAMCAST)
else()

endif()
link_libraries(m)

if(PLATFORM_DREAMCAST)
    add_executable(rust-interop ${EXTRA_OBJS} ${SOURCES})

    # Add rust_code as a CMake target
    ExternalProject_Add(
            rust_code
            DOWNLOAD_COMMAND ""
            CONFIGURE_COMMAND ""
            BUILD_COMMAND cargo gccrs build COMMAND cargo gccrs build --release
            BINARY_DIR "${CMAKE_SOURCE_DIR}/rust/game-code"
            INSTALL_COMMAND ""
            LOG_BUILD ON)

    add_dependencies(rust-interop rust_code)
else()
    link_directories(rust/game-code/target/debug/)
    link_libraries(game_code)

    add_definitions(-g)
    if(WIN32)
        add_executable(rust-interop.exe ${EXTRA_OBJS} ${SOURCES})
    else()
        add_executable(rust-interop ${EXTRA_OBJS} ${SOURCES})

        # Add rust_code as a CMake target
        ExternalProject_Add(
                rust_code
                DOWNLOAD_COMMAND ""
                CONFIGURE_COMMAND ""
                BUILD_COMMAND cargo build COMMAND cargo build --release
                BINARY_DIR "${CMAKE_SOURCE_DIR}/rust/game-code"
                INSTALL_COMMAND ""
                LOG_BUILD ON)

        add_dependencies(rust-interop rust_code)
    endif()
endif()
