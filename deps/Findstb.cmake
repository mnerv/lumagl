if (DEFINED _FINDSTB_INCLUDED)
    unset(stb_FOUND CACHE)
    unset(stb_DIR CACHE)
    unset(stb_INCLUDE_DIR CACHE)
    unset(stb_LIBRARIES CACHE)
    unset(stb_VERSION CACHE)
    return()
endif()
set(_FINDSTB_INCLUDED TRUE)

find_package(stb QUIET)

if (NOT stb_FOUND)
    message(STATUS "stb not found. Fetching via FetchContent...")

    include(FetchContent)

    find_program(GIT_EXECUTABLE git)
    if (GIT_EXECUTABLE)
        set(STB_FETCH_METHOD "GIT")
    else()
        set(STB_FETCH_METHOD "ZIP")
    endif()

    if (STB_FETCH_METHOD STREQUAL "GIT")
        FetchContent_Declare(
            stb
            GIT_REPOSITORY https://github.com/mononerv/stb.git
            GIT_TAG        698c6fb9889c71494b49c9187d249af5fc87b211
        )
    else()
        message(FATAL_ERROR "Fetch with zip not supported.")
    endif()

    FetchContent_MakeAvailable(stb)
else()
    message(STATUS "stb found.")

    # Ensure we have a usable target
    if (NOT TARGET stb::stb)
        if (TARGET stb)
            add_library(stb::stb ALIAS stb)
        else()
            message(FATAL_ERROR "stb was found, but no CMake target 'stb' or 'stb::stb' exists.")
        endif()
    endif()

    # Populate the standard variables
    set(stb_FOUND TRUE)
    set(stb_LIBRARIES stb::stb)
    get_target_property(_stb_inc stb::stb INTERFACE_INCLUDE_DIRECTORIES)
    set(stb_INCLUDE_DIR "${_stb_inc}")
endif()

if (NOT TARGET stb::stb)
    if (TARGET stb)
        add_library(stb::stb ALIAS stb)
    else()
        message(FATAL_ERROR "Could not find or fetch stb; no target stb or stb::stb available")
    endif()
endif()

set(stb_FOUND        TRUE)
set(stb_LIBRARIES    stb::stb)
set(stb_VERSION      "${stb_VERSION}")         # this comes from the stb project
get_target_property(_stb_inc stb::stb INTERFACE_INCLUDE_DIRECTORIES)
set(stb_INCLUDE_DIR  "${_stb_inc}")
