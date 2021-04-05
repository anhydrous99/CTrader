CPMAddPackage(
        NAME googletest
        GITHUB_REPOSITORY google/googletest
        GIT_TAG release-1.8.1
        VERSION 1.8.1
        OPTIONS
        "INSTALL_GTEST OFF"
        "gtest_force_shared_crt ON"
)