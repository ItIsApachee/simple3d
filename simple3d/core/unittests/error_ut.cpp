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

TEST(TErrorTest, TestThrowOnError)
{
    EXPECT_NO_THROW(TError().ThrowOnError());
    EXPECT_THROW(TError("foo").ThrowOnError(), TErrorException);
}

TEST(TErrorOrValueTest, TestErrorMessage)
{
    EXPECT_EQ(TErrorOr<int>().GetMessage(), "");
    EXPECT_EQ(TErrorOr<int>(TError("vim is cool")).GetMessage(), "vim is cool");
    EXPECT_EQ(
        TErrorOr<int>(TError("square root of {} is undefined under real numbers", -1)).GetMessage(),
        "square root of -1 is undefined under real numbers");
}

TEST(TErrorOrValueTest, TestOKErrors)
{
    EXPECT_TRUE(TErrorOr<int>().IsOk());
    EXPECT_TRUE(TErrorOr<int>(1337).IsOk());
    EXPECT_FALSE(TErrorOr<int>(TError("foo")).IsOk());
}

TEST(TErrorOrValueTest, TestErrorFormatting)
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

TEST(TErrorOrValueTest, TestThrowOnError)
{
    EXPECT_NO_THROW(TErrorOr<int>().ThrowOnError());
    EXPECT_THROW(TErrorOr<int>(TError("foo")).ThrowOnError(), TErrorException);

    EXPECT_NO_THROW(TErrorOr<int>(1337).ValueOrThrow());
    EXPECT_EQ(TErrorOr<int>(1337).ValueOrThrow(), 1337);
    EXPECT_THROW(TErrorOr<int>(TError("foo")).ValueOrThrow(), TErrorException);
}

TEST(TErrorOrValueTest, TestValue)
{
    const std::vector<int> SampleVec{1, 2, 3};

    TErrorOr<std::vector<int>> vecOrError = SampleVec;

    EXPECT_EQ(vecOrError.ValueOrThrow(), SampleVec);
    EXPECT_EQ(vecOrError.Value(), SampleVec);

    // Check that value is still present

    EXPECT_EQ(vecOrError.ValueOrThrow(), SampleVec);
    EXPECT_EQ(vecOrError.Value(), SampleVec);

    // Check moving

    auto movedVec = std::move(vecOrError).Value();
    EXPECT_EQ(movedVec, SampleVec);
    EXPECT_EQ(vecOrError.Value(), std::vector<int>{});
}

}  // namespace NSimple3D
