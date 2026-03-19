cmake_minimum_required(VERSION 3.20)
project(mbus_master VERSION 2.0.0 LANGUAGES C)

# ── Zephyr module ──────────────────────────────────────────────────────
if(CONFIG_MBUS_MASTER)
    zephyr_library_named(mbus_master)
    zephyr_library_sources(src/mbus_master.c src/mbus_master_int.c src/mbus_master_utils.c)
    zephyr_library_sources_ifdef(CONFIG_MBUS_MASTER_NT750
        src/nt750/lwrb.c src/nt750/mbus_aux.c src/nt750/mbus_protocol.c src/nt750/nt750.c)
    zephyr_library_include_directories(include)
    return()
endif()

# ── Standalone build ───────────────────────────────────────────────────
add_library(mbus_master STATIC
    src/mbus_master.c
    src/mbus_master_int.c
    src/mbus_master_utils.c
)

target_include_directories(mbus_master
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(mbus_master PROPERTIES C_STANDARD 99)

# ── NT750 module (optional) ───────────────────────────────────────────
option(MBUS_MASTER_NT750 "Include NT750 MBUS converter module" OFF)
if(MBUS_MASTER_NT750)
    target_sources(mbus_master PRIVATE
        src/nt750/lwrb.c
        src/nt750/mbus_aux.c
        src/nt750/mbus_protocol.c
        src/nt750/nt750.c
    )
    target_compile_definitions(mbus_master PUBLIC MBUS_MASTER_CFG_NT750_ENABLED=1)
    message(STATUS "mbus_master: NT750 module enabled")
endif()

# ── Port selection ─────────────────────────────────────────────────────
set(MBUS_MASTER_PORT "none" CACHE STRING "Port: nuvoton, none")
if(MBUS_MASTER_PORT STREQUAL "nuvoton")
    target_sources(mbus_master PRIVATE port/nuvoton/mbus_master_port_nuvoton.c)
    message(STATUS "mbus_master: using Nuvoton UART port")
endif()
