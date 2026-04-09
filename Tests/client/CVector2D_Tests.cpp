/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Tests/client/CVector2D_Tests.cpp
 *  PURPOSE:     Google Test suite for CVector2D (2D vector math)
 *
 *****************************************************************************/

#include <gtest/gtest.h>
#include <SharedUtil.Defines.h>
#include <CVector2D.h>

static constexpr float kEpsilon = 0.001f;

///////////////////////////////////////////////////////////////
//
// Construction
//
///////////////////////////////////////////////////////////////

TEST(CVector2D, DefaultConstructor)
{
    CVector2D v;
    EXPECT_FLOAT_EQ(v.fX, 0.0f);
    EXPECT_FLOAT_EQ(v.fY, 0.0f);
}

TEST(CVector2D, ExplicitConstructor)
{
    CVector2D v(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.fX, 3.0f);
    EXPECT_FLOAT_EQ(v.fY, 4.0f);
}

TEST(CVector2D, ConstructFromCVector)
{
    CVector   v3(1.0f, 2.0f, 3.0f);
    CVector2D v2(v3);
    EXPECT_FLOAT_EQ(v2.fX, 1.0f);
    EXPECT_FLOAT_EQ(v2.fY, 2.0f);
}

TEST(CVector2D, ConstructFromCVector4D)
{
    CVector4D v4(5.0f, 6.0f, 7.0f, 8.0f);
    CVector2D v2(v4);
    EXPECT_FLOAT_EQ(v2.fX, 5.0f);
    EXPECT_FLOAT_EQ(v2.fY, 6.0f);
}

///////////////////////////////////////////////////////////////
//
// Math operations
//
///////////////////////////////////////////////////////////////

TEST(CVector2D, Length)
{
    CVector2D v(3.0f, 4.0f);
    EXPECT_NEAR(v.Length(), 5.0f, kEpsilon);
}

TEST(CVector2D, LengthSquared)
{
    CVector2D v(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.LengthSquared(), 25.0f);
}

TEST(CVector2D, DotProduct)
{
    CVector2D a(1.0f, 0.0f);
    CVector2D b(0.0f, 1.0f);
    EXPECT_FLOAT_EQ(a.DotProduct(b), 0.0f);

    CVector2D c(2.0f, 3.0f);
    CVector2D d(4.0f, 5.0f);
    EXPECT_FLOAT_EQ(c.DotProduct(d), 23.0f);
}

TEST(CVector2D, Normalize)
{
    CVector2D v(3.0f, 4.0f);
    v.Normalize();
    EXPECT_NEAR(v.Length(), 1.0f, kEpsilon);
    EXPECT_NEAR(v.fX, 0.6f, kEpsilon);
    EXPECT_NEAR(v.fY, 0.8f, kEpsilon);
}

TEST(CVector2D, NormalizeZeroVector)
{
    CVector2D v(0.0f, 0.0f);
    v.Normalize();
    EXPECT_FLOAT_EQ(v.fX, 0.0f);
    EXPECT_FLOAT_EQ(v.fY, 0.0f);
}

///////////////////////////////////////////////////////////////
//
// Operators
//
///////////////////////////////////////////////////////////////

TEST(CVector2D, ScalarMultiply)
{
    CVector2D v(2.0f, 3.0f);
    CVector2D result = v * 4.0f;
    EXPECT_FLOAT_EQ(result.fX, 8.0f);
    EXPECT_FLOAT_EQ(result.fY, 12.0f);
}

TEST(CVector2D, ScalarDivide)
{
    CVector2D v(10.0f, 20.0f);
    CVector2D result = v / 2.0f;
    EXPECT_FLOAT_EQ(result.fX, 5.0f);
    EXPECT_FLOAT_EQ(result.fY, 10.0f);
}

TEST(CVector2D, VectorAddition)
{
    CVector2D a(1.0f, 2.0f);
    CVector2D b(3.0f, 4.0f);
    CVector2D result = a + b;
    EXPECT_FLOAT_EQ(result.fX, 4.0f);
    EXPECT_FLOAT_EQ(result.fY, 6.0f);
}

TEST(CVector2D, VectorSubtraction)
{
    CVector2D a(5.0f, 7.0f);
    CVector2D b(2.0f, 3.0f);
    CVector2D result = a - b;
    EXPECT_FLOAT_EQ(result.fX, 3.0f);
    EXPECT_FLOAT_EQ(result.fY, 4.0f);
}

TEST(CVector2D, VectorMultiply)
{
    CVector2D a(2.0f, 3.0f);
    CVector2D b(4.0f, 5.0f);
    CVector2D result = a * b;
    EXPECT_FLOAT_EQ(result.fX, 8.0f);
    EXPECT_FLOAT_EQ(result.fY, 15.0f);
}

TEST(CVector2D, VectorDivide)
{
    CVector2D a(10.0f, 20.0f);
    CVector2D b(2.0f, 5.0f);
    CVector2D result = a / b;
    EXPECT_FLOAT_EQ(result.fX, 5.0f);
    EXPECT_FLOAT_EQ(result.fY, 4.0f);
}

TEST(CVector2D, CompoundAddScalar)
{
    CVector2D v(1.0f, 2.0f);
    v += 5.0f;
    EXPECT_FLOAT_EQ(v.fX, 6.0f);
    EXPECT_FLOAT_EQ(v.fY, 7.0f);
}

TEST(CVector2D, CompoundSubVector)
{
    CVector2D v(10.0f, 20.0f);
    v -= CVector2D(3.0f, 5.0f);
    EXPECT_FLOAT_EQ(v.fX, 7.0f);
    EXPECT_FLOAT_EQ(v.fY, 15.0f);
}

TEST(CVector2D, CompoundMulScalar)
{
    CVector2D v(2.0f, 3.0f);
    v *= 4.0f;
    EXPECT_FLOAT_EQ(v.fX, 8.0f);
    EXPECT_FLOAT_EQ(v.fY, 12.0f);
}

TEST(CVector2D, CompoundDivScalar)
{
    CVector2D v(10.0f, 20.0f);
    v /= 5.0f;
    EXPECT_FLOAT_EQ(v.fX, 2.0f);
    EXPECT_FLOAT_EQ(v.fY, 4.0f);
}

///////////////////////////////////////////////////////////////
//
// Equality
//
///////////////////////////////////////////////////////////////

TEST(CVector2D, Equality)
{
    CVector2D a(1.0f, 2.0f);
    CVector2D b(1.0f, 2.0f);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(CVector2D, Inequality)
{
    CVector2D a(1.0f, 2.0f);
    CVector2D b(1.0f, 3.0f);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}

TEST(CVector2D, AssignFromCVector)
{
    CVector   v3(7.0f, 8.0f, 9.0f);
    CVector2D v2;
    v2 = v3;
    EXPECT_FLOAT_EQ(v2.fX, 7.0f);
    EXPECT_FLOAT_EQ(v2.fY, 8.0f);
}
