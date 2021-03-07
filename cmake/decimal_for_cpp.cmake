CPMAddPackage(
        NAME decimal_for_cpp
        VERSION 1.17
        GITHUB_REPOSITORY vpiotr/decimal_for_cpp
        GIT_TAG 9df9e9824b
        DOWNLOAD_ONLY
)

if (decimal_for_cpp_ADDED)
    add_library(decimal_for_cpp INTERFACE IMPORTED)
    target_include_directories(decimal_for_cpp INTERFACE ${decimal_for_cpp_SOURCE_DIR}/include)
endif()
