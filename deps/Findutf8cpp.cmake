if (DEFINED _FINDUTF8CPP_INCLUDED)
    unset(utf8cpp_FOUND CACHE)
    unset(utf8cpp_DIR CACHE)
    unset(utf8cpp_INCLUDE_DIR CACHE)
    unset(utf8cpp_LIBRARIES CACHE)
    unset(utf8cpp_VERSION CACHE)
    return()
endif()
set(_FINDUTF8CPP_INCLUDED TRUE)

find_package(utf8cpp QUIET)

if (NOT utf8cpp_FOUND)
    message(STATUS "utf8cpp not found. Fetching via FetchContent...")

    include(FetchContent)

    find_program(GIT_EXECUTABLE git)
    if (GIT_EXECUTABLE)
        set(UTF8CPP_FETCH_METHOD "GIT")
    else()
        set(UTF8CPP_FETCH_METHOD "ZIP")
    endif()

    if (UTF8CPP_FETCH_METHOD STREQUAL "GIT")
        FetchContent_Declare(
            utf8cpp
            GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
            GIT_TAG        v4.0.6
        )
    else()
        FetchContent_Declare(
            utf8cpp
            URL https://github.com/nemtrif/utfcpp/archive/refs/tags/v4.0.6.zip
        )
    endif()

    FetchContent_MakeAvailable(utf8cpp)
else()
    message(STATUS "utf8cpp found.")

    # Ensure we have a usable target
    if (NOT TARGET utf8cpp::utf8cpp)
        if (TARGET utf8cpp)
            add_library(utf8cpp::utf8cpp ALIAS utf8cpp)
        else()
            message(FATAL_ERROR "utf8cpp was found, but no CMake target 'utf8cpp' or 'utf8cpp::utf8cpp' exists.")
        endif()
    endif()

    # Populate the standard variables
    set(utf8cpp_FOUND TRUE)
    set(utf8cpp_LIBRARIES utf8cpp::utf8cpp)
    get_target_property(_utf8cpp_inc utf8cpp::utf8cpp INTERFACE_INCLUDE_DIRECTORIES)
    set(utf8cpp_INCLUDE_DIR "${_utf8cpp_inc}")
endif()

if (NOT TARGET utf8cpp::utf8cpp)
    if (TARGET utf8cpp)
        add_library(utf8cpp::utf8cpp ALIAS utf8cpp)
    else()
        message(FATAL_ERROR "Could not find or fetch utf8cpp; no target utf8cpp or utf8cpp::utf8cpp available")
    endif()
endif()

set(utf8cpp_FOUND        TRUE)
set(utf8cpp_LIBRARIES    utf8cpp::utf8cpp)
set(utf8cpp_VERSION      "${utf8cpp_VERSION}")         # this comes from the utf8cpp project
get_target_property(_utf8cpp_inc utf8cpp::utf8cpp INTERFACE_INCLUDE_DIRECTORIES)
set(utf8cpp_INCLUDE_DIR  "${_utf8cpp_inc}")
