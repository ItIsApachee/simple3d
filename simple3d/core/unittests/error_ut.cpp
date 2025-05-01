#include <gtest/gtest.h>

#include <simple3d/core/error.h>

namespace NSimple3D {

using namespace std::literals::string_literals;

TEST(TErrorTest, TestErrorMessage)
{
    EXPECT_EQ(TError().GetMessage(), "");
    EXPECT_EQ(TError("vim is cool").GetMessage(), "vim is cool");
    EXPECT_EQ(
        TError("square root of {} is undefined under real numbers", -1).GetMessage(),
        "square root of -1 is undefined under real numbers");
}

TEST(TErrorTest, TestOKErrors)
{
    EXPECT_TRUE(TError().IsOk());
    EXPECT_FALSE(TError("foo").IsOk());
}

TEST(TErrorTest, TestErrorFormatting)
{
    auto formatError = [] (TError error) {
        return std::format("{}", error);
    };

    EXPECT_EQ(formatError(TError()), "OK");
    EXPECT_EQ(formatError(TError("vim is cool")), "vim is cool");
    EXPECT_EQ(
        formatError(TError("square root of {} is undefined under real numbers", -1)),
        "square root of -1 is undefined under real numbers");
}

}  // namespace NSimple3D
