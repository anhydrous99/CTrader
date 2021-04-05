//
// Created by constexpr_dog on 4/5/21.
//

#include <gtest/gtest.h>
#include <libCTrader/Auth.h>

namespace {
    using ::testing::TestWithParam;
    using ::testing::Values;

    struct AuthTest : public ::testing::Test {
        libCTrader::Auth auth;
    protected:
        void SetUp() override {
            auth = libCTrader::Auth(
                    "86ba7cc41c63e8703f5204741d3daeee",
                    "kAbDfhW0tNer94bLNKDDS2W21O9xtkn5pRa9LeiGy+TeeP5mgNNNxcALJQ47CHo0U5wbK7HxWCI/z+vHh1o2jw==",
                    "msr63p109q"
                    );
        }
    };

    TEST_F(AuthTest, Sign1) {
        std::string time_stamp = "this is a time stamp";
        std::string signature = auth.Sign(time_stamp, "GET", "/hello");
        EXPECT_EQ(signature, "UDYuZUftU+l8lfDbU+NeD2dVWzEDAyoSNw//8bKt2L8=");
    }

    TEST_F(AuthTest, Sign2) {
        std::string time_stamp = "this is a time stamp";
        std::string signature = auth.Sign(time_stamp, "GET", "/hello", "body");
        EXPECT_EQ(signature, "9XWa2fMexaMGs2XsexPlso3ppkrnk945ITG48v85pXo=");
    }

    TEST(AuthTest_Base64, Base64Encode) {
        std::string expected = "YXNkZg==";
        std::string input = "asdf";
        std::string ret = libCTrader::Auth::b64_encode(input);
        EXPECT_EQ(ret, expected);
    }

    TEST(AuthTest_Base64, Base64Decode) {
        std::string expected = "asdf";
        std::string input = "YXNkZg==";
        std::string ret = libCTrader::Auth::b64_decode(input);
        EXPECT_EQ(ret, expected);
    }
}