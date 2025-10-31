if (DEFINED _FINDCTRE_INCLUDED)
    unset(ctre_FOUND CACHE)
    unset(ctre_DIR CACHE)
    unset(ctre_INCLUDE_DIR CACHE)
    unset(ctre_LIBRARIES CACHE)
    unset(ctre_VERSION CACHE)
    return()
endif()
set(_FINDCTRE_INCLUDED TRUE)

find_package(ctre QUIET)

if (NOT ctre_FOUND)
    message(STATUS "ctre not found. Fetching via FetchContent...")

    include(FetchContent)

    find_program(GIT_EXECUTABLE git)
    if (GIT_EXECUTABLE)
        set(CTRE_FETCH_METHOD "GIT")
    else()
        set(CTRE_FETCH_METHOD "ZIP")
    endif()

    if (CTRE_FETCH_METHOD STREQUAL "GIT")
        FetchContent_Declare(
            ctre
            GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
            GIT_TAG        v3.10.0
        )
    else()
        FetchContent_Declare(
            ctre
            URL https://github.com/hanickadot/compile-time-regular-expressions/archive/refs/tags/v3.10.0.zip
        )
    endif()

    # Turn off BUILD_TESTING globally to prevent CTest from being included in CTRE
    set(BUILD_TESTING OFF CACHE BOOL "Disable testing globally" FORCE)
    # Set the CTRE_BUILD_TESTS option before including the CTRE library
    set(CTRE_BUILD_TESTS OFF CACHE BOOL "Build ctre Tests" FORCE)
    FetchContent_MakeAvailable(ctre)
else()
    message(STATUS "ctre found.")

    # Ensure we have a usable target
    if (NOT TARGET ctre::ctre)
        if (TARGET ctre)
            add_library(ctre::ctre ALIAS ctre)
        else()
            message(FATAL_ERROR "ctre was found, but no CMake target 'ctre' or 'ctre::ctre' exists.")
        endif()
    endif()

    # Populate the standard variables
    set(ctre_FOUND TRUE)
    set(ctre_LIBRARIES ctre::ctre)
    get_target_property(_ctre_inc ctre::ctre INTERFACE_INCLUDE_DIRECTORIES)
    set(ctre_INCLUDE_DIR "${_ctre_inc}")
endif()

if (NOT TARGET ctre::ctre)
    if (TARGET ctre)
        add_library(ctre::ctre ALIAS ctre)
    else()
        message(FATAL_ERROR "Could not find or fetch ctre; no target ctre or ctre::ctre available")
    endif()
endif()

set(ctre_FOUND        TRUE)
set(ctre_LIBRARIES    ctre::ctre)
set(ctre_VERSION      "${ctre_VERSION}")         # this comes from the ctre project
get_target_property(_ctre_inc ctre::ctre INTERFACE_INCLUDE_DIRECTORIES)
set(ctre_INCLUDE_DIR  "${_ctre_inc}")
