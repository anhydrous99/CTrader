CPMAddPackage(
        NAME CURL
        GITHUB_REPOSITORY curl/curl
        GIT_TAG curl-7_75_0
        OPTIONS
        "BUILD_CURL_EXE OFF"
        "HTTP_ONLY ON"
)