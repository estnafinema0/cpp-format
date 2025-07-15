#include <gtest/gtest.h>
#include "format.hpp"

TEST(FormatTest, BasicReplacement) {
    auto s = format("{1}+{1}={0}", 2, "one");
    EXPECT_EQ(s, "one+one=2");
}

TEST(FormatTest, ReuseSameIndex) {
    auto s = format("{0}-{0}-{0}", "x");
    EXPECT_EQ(s, "x-x-x");
}

TEST(FormatTest, EmptyPattern) {
    auto s = format("", 1, 2, 3);
    EXPECT_EQ(s, "");
}

TEST(FormatTest, NoPlaceholders) {
    auto s = format("plain text", 1);
    EXPECT_EQ(s, "plain text");
}

TEST(FormatErrorTest, EmptyBraces) {
    EXPECT_THROW(format("Hello {}!", 1), FormatError);
}

TEST(FormatErrorTest, MissingClosingBrace) {
    EXPECT_THROW(format("Value is {0", 10), FormatError);
}

TEST(FormatErrorTest, InvalidPlaceholder) {
    EXPECT_THROW(format("Value {a} here", 5), FormatError);
}

TEST(FormatErrorTest, UnexpectedClosingBrace) {
    EXPECT_THROW(format("Bad } here", 1), FormatError);
}

TEST(FormatErrorTest, IndexOutOfRange) {
    EXPECT_THROW(format("{2}", "only", "two"), FormatError);
}

TEST(FormatErrorTest, LargeIndex) {
    EXPECT_THROW(format("{999}", 0), FormatError);
}
