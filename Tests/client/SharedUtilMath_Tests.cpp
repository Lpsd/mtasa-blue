/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Tests/client/SharedUtilMath_Tests.cpp
 *  PURPOSE:     Google Test suite for SharedUtil.Math.h utilities
 *
 *****************************************************************************/

#include <gtest/gtest.h>
#include <SharedUtil.Math.h>

static constexpr float kPi = 3.14159265f;

///////////////////////////////////////////////////////////////
//
// Square
//
///////////////////////////////////////////////////////////////

TEST(SharedUtilMath, SquareInt)
{
    EXPECT_EQ(SharedUtil::Square(5), 25);
    EXPECT_EQ(SharedUtil::Square(-3), 9);
    EXPECT_EQ(SharedUtil::Square(0), 0);
}

TEST(SharedUtilMath, SquareFloat)
{
    EXPECT_FLOAT_EQ(SharedUtil::Square(2.5f), 6.25f);
}

///////////////////////////////////////////////////////////////
//
// DegreesToRadians
//
///////////////////////////////////////////////////////////////

TEST(SharedUtilMath, DegreesToRadiansZero)
{
    EXPECT_FLOAT_EQ(SharedUtil::DegreesToRadians(0.0f), 0.0f);
}

TEST(SharedUtilMath, DegreesToRadians180)
{
    EXPECT_NEAR(SharedUtil::DegreesToRadians(180.0f), kPi, 0.001f);
}

TEST(SharedUtilMath, DegreesToRadians90)
{
    EXPECT_NEAR(SharedUtil::DegreesToRadians(90.0f), kPi / 2.0f, 0.001f);
}

TEST(SharedUtilMath, DegreesToRadians360)
{
    EXPECT_NEAR(SharedUtil::DegreesToRadians(360.0f), 2.0f * kPi, 0.01f);
}

///////////////////////////////////////////////////////////////
//
// ShouldUseInt
//
///////////////////////////////////////////////////////////////

TEST(SharedUtilMath, ShouldUseIntExactInteger)
{
    int number = 0;
    EXPECT_TRUE(SharedUtil::ShouldUseInt(42.0, &number));
    EXPECT_EQ(number, 42);
}

TEST(SharedUtilMath, ShouldUseIntFractionalValue)
{
    int number = 0;
    EXPECT_FALSE(SharedUtil::ShouldUseInt(42.5, &number));
}

TEST(SharedUtilMath, ShouldUseIntZero)
{
    int number = -1;
    EXPECT_TRUE(SharedUtil::ShouldUseInt(0.0, &number));
    EXPECT_EQ(number, 0);
}

TEST(SharedUtilMath, ShouldUseIntLargeValue)
{
    // Values in the large-int range (>= 0x1000000) should return true via rounding
    int    number = 0;
    double large = 20000000.0;
    EXPECT_TRUE(SharedUtil::ShouldUseInt(large, &number));
    EXPECT_EQ(number, 20000000);
}

TEST(SharedUtilMath, ShouldUseIntTooLargeForInt)
{
    int    number = 0;
    double huge = 3e10;
    EXPECT_FALSE(SharedUtil::ShouldUseInt(huge, &number));
}

TEST(SharedUtilMath, ShouldUseIntNegative)
{
    int number = 0;
    EXPECT_TRUE(SharedUtil::ShouldUseInt(-100.0, &number));
    EXPECT_EQ(number, -100);
}

///////////////////////////////////////////////////////////////
//
// IsNearlyEqual
//
///////////////////////////////////////////////////////////////

TEST(SharedUtilMath, IsNearlyEqualSameValue)
{
    EXPECT_TRUE(SharedUtil::IsNearlyEqual(1.0f, 1.0f));
}

TEST(SharedUtilMath, IsNearlyEqualWithinEpsilon)
{
    float eps = 0.01f;
    EXPECT_TRUE(SharedUtil::IsNearlyEqual(1.0f, 1.005f, eps));
}

TEST(SharedUtilMath, IsNearlyEqualOutsideEpsilon)
{
    float eps = 0.001f;
    EXPECT_FALSE(SharedUtil::IsNearlyEqual(1.0f, 1.01f, eps));
}

///////////////////////////////////////////////////////////////
//
// ApproximatelyEqual (relative comparison using max)
//
///////////////////////////////////////////////////////////////

TEST(SharedUtilMath, ApproximatelyEqualSameValue)
{
    EXPECT_TRUE(SharedUtil::ApproximatelyEqual(100.0f, 100.0f));
}

TEST(SharedUtilMath, ApproximatelyEqualLargeEpsilon)
{
    // With epsilon=0.1, 100 and 105 differ by 5, max(100,105)=105, 105*0.1=10.5 > 5
    EXPECT_TRUE(SharedUtil::ApproximatelyEqual(100.0f, 105.0f, 0.1f));
}

TEST(SharedUtilMath, ApproximatelyEqualFailsSmallEpsilon)
{
    EXPECT_FALSE(SharedUtil::ApproximatelyEqual(100.0f, 110.0f, 0.01f));
}

///////////////////////////////////////////////////////////////
//
// EssentiallyEqual (relative comparison using min)
//
///////////////////////////////////////////////////////////////

TEST(SharedUtilMath, EssentiallyEqualSameValue)
{
    EXPECT_TRUE(SharedUtil::EssentiallyEqual(100.0f, 100.0f));
}

TEST(SharedUtilMath, EssentiallyEqualIsStricterThanApproximately)
{
    // With epsilon=0.1, 100 and 105: min(100,105)=100, 100*0.1=10 > 5 => true
    EXPECT_TRUE(SharedUtil::EssentiallyEqual(100.0f, 105.0f, 0.1f));
    // But with a smaller epsilon, EssentiallyEqual should be stricter
    // min(100,110)=100, 100*0.01=1 < 10 => false
    EXPECT_FALSE(SharedUtil::EssentiallyEqual(100.0f, 110.0f, 0.01f));
}

///////////////////////////////////////////////////////////////
//
// DefinitelyGreaterThan / DefinitelyLessThan
//
///////////////////////////////////////////////////////////////

TEST(SharedUtilMath, DefinitelyGreaterThanClearlyGreater)
{
    EXPECT_TRUE(SharedUtil::DefinitelyGreaterThan(200.0f, 100.0f, 0.1f));
}

TEST(SharedUtilMath, DefinitelyGreaterThanNearlyEqual)
{
    // 100.0 vs 100.0 — not definitely greater
    EXPECT_FALSE(SharedUtil::DefinitelyGreaterThan(100.0f, 100.0f, 0.1f));
}

TEST(SharedUtilMath, DefinitelyLessThanClearlyLess)
{
    EXPECT_TRUE(SharedUtil::DefinitelyLessThan(100.0f, 200.0f, 0.1f));
}

TEST(SharedUtilMath, DefinitelyLessThanNearlyEqual)
{
    EXPECT_FALSE(SharedUtil::DefinitelyLessThan(100.0f, 100.0f, 0.1f));
}

///////////////////////////////////////////////////////////////
//
// NumberOfSignificantBits
//
///////////////////////////////////////////////////////////////

TEST(SharedUtilMath, NumberOfSignificantBitsZero)
{
    EXPECT_EQ(SharedUtil::NumberOfSignificantBits<0>::COUNT, 0);
}

TEST(SharedUtilMath, NumberOfSignificantBitsOne)
{
    EXPECT_EQ(SharedUtil::NumberOfSignificantBits<1>::COUNT, 1);
}

TEST(SharedUtilMath, NumberOfSignificantBits255)
{
    EXPECT_EQ(SharedUtil::NumberOfSignificantBits<255>::COUNT, 8);
}

TEST(SharedUtilMath, NumberOfSignificantBits256)
{
    EXPECT_EQ(SharedUtil::NumberOfSignificantBits<256>::COUNT, 9);
}
