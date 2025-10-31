if (DEFINED _FINDFMT_INCLUDED)
    unset(fmt_FOUND CACHE)
    unset(fmt_DIR CACHE)
    unset(fmt_INCLUDE_DIR CACHE)
    unset(fmt_LIBRARIES CACHE)
    unset(fmt_VERSION CACHE)
    return()
endif()
set(_FINDFMT_INCLUDED TRUE)

find_package(fmt QUIET)

if (NOT fmt_FOUND)
    message(STATUS "fmt not found. Fetching via FetchContent...")

    include(FetchContent)

    find_program(GIT_EXECUTABLE git)
    if (GIT_EXECUTABLE)
        set(FMT_FETCH_METHOD "GIT")
    else()
        set(FMT_FETCH_METHOD "ZIP")
    endif()

    if (FMT_FETCH_METHOD STREQUAL "GIT")
        FetchContent_Declare(
            fmt
            GIT_REPOSITORY https://github.com/fmtlib/fmt.git
            GIT_TAG        11.2.0
        )
    else()
        FetchContent_Declare(
            fmt
            URL https://github.com/fmtlib/fmt/releases/download/11.2.0/fmt-11.2.0.zip
        )
    endif()

    FetchContent_MakeAvailable(fmt)
else()
    message(STATUS "fmt found.")

    # Ensure we have a usable target
    if (NOT TARGET fmt::fmt)
        if (TARGET fmt)
            add_library(fmt::fmt ALIAS fmt)
        else()
            message(FATAL_ERROR "fmt was found, but no CMake target 'fmt' or 'fmt::fmt' exists.")
        endif()
    endif()

    # Populate the standard variables
    set(fmt_FOUND TRUE)
    set(fmt_LIBRARIES fmt::fmt)
    get_target_property(_fmt_inc fmt::fmt INTERFACE_INCLUDE_DIRECTORIES)
    set(fmt_INCLUDE_DIR "${_fmt_inc}")
endif()

if (NOT TARGET fmt::fmt)
    if (TARGET fmt)
        add_library(fmt::fmt ALIAS fmt)
    else()
        message(FATAL_ERROR "Could not find or fetch fmt; no target fmt or fmt::fmt available")
    endif()
endif()

set(fmt_FOUND        TRUE)
set(fmt_LIBRARIES    fmt::fmt)
set(fmt_VERSION      "${fmt_VERSION}")         # this comes from the fmt project
get_target_property(_fmt_inc fmt::fmt INTERFACE_INCLUDE_DIRECTORIES)
set(fmt_INCLUDE_DIR  "${_fmt_inc}")
