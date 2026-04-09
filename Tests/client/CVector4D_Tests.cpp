/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Tests/client/CVector4D_Tests.cpp
 *  PURPOSE:     Google Test suite for CVector4D (4D vector math)
 *
 *****************************************************************************/

#include <gtest/gtest.h>
#include <SharedUtil.Defines.h>
#include <CVector4D.h>

static constexpr float kEpsilon = 0.001f;

///////////////////////////////////////////////////////////////
//
// Construction
//
///////////////////////////////////////////////////////////////

TEST(CVector4D, DefaultConstructor)
{
    CVector4D v;
    EXPECT_FLOAT_EQ(v.fX, 0.0f);
    EXPECT_FLOAT_EQ(v.fY, 0.0f);
    EXPECT_FLOAT_EQ(v.fZ, 0.0f);
    EXPECT_FLOAT_EQ(v.fW, 0.0f);
}

TEST(CVector4D, ExplicitConstructor)
{
    CVector4D v(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.fX, 1.0f);
    EXPECT_FLOAT_EQ(v.fY, 2.0f);
    EXPECT_FLOAT_EQ(v.fZ, 3.0f);
    EXPECT_FLOAT_EQ(v.fW, 4.0f);
}

TEST(CVector4D, CopyConstructor)
{
    CVector4D a(1.0f, 2.0f, 3.0f, 4.0f);
    CVector4D b(a);
    EXPECT_FLOAT_EQ(b.fX, 1.0f);
    EXPECT_FLOAT_EQ(b.fY, 2.0f);
    EXPECT_FLOAT_EQ(b.fZ, 3.0f);
    EXPECT_FLOAT_EQ(b.fW, 4.0f);
}

///////////////////////////////////////////////////////////////
//
// Math operations
//
///////////////////////////////////////////////////////////////

TEST(CVector4D, Length)
{
    CVector4D v(1.0f, 2.0f, 2.0f, 0.0f);
    // sqrt(1 + 4 + 4 + 0) = 3
    EXPECT_NEAR(v.Length(), 3.0f, kEpsilon);
}

TEST(CVector4D, LengthSquared)
{
    CVector4D v(1.0f, 2.0f, 3.0f, 4.0f);
    // 1 + 4 + 9 + 16 = 30
    EXPECT_FLOAT_EQ(v.LengthSquared(), 30.0f);
}

TEST(CVector4D, DotProduct)
{
    // Note: CVector4D::DotProduct intentionally excludes fW (see source comment)
    CVector4D a(1.0f, 2.0f, 3.0f, 100.0f);
    CVector4D b(4.0f, 5.0f, 6.0f, 200.0f);
    // 1*4 + 2*5 + 3*6 = 32 (fW is not included)
    EXPECT_FLOAT_EQ(a.DotProduct(b), 32.0f);
}

TEST(CVector4D, Normalize)
{
    CVector4D v(2.0f, 0.0f, 0.0f, 0.0f);
    v.Normalize();
    EXPECT_NEAR(v.fX, 1.0f, kEpsilon);
    EXPECT_NEAR(v.Length(), 1.0f, kEpsilon);
}

TEST(CVector4D, NormalizeFourComponents)
{
    CVector4D v(1.0f, 1.0f, 1.0f, 1.0f);
    v.Normalize();
    // Each component should be 1/2 since sqrt(4) = 2
    EXPECT_NEAR(v.fX, 0.5f, kEpsilon);
    EXPECT_NEAR(v.fW, 0.5f, kEpsilon);
    EXPECT_NEAR(v.Length(), 1.0f, kEpsilon);
}

TEST(CVector4D, NormalizeZeroVector)
{
    CVector4D v(0.0f, 0.0f, 0.0f, 0.0f);
    v.Normalize();
    EXPECT_FLOAT_EQ(v.fX, 0.0f);
    EXPECT_FLOAT_EQ(v.fW, 0.0f);
}

///////////////////////////////////////////////////////////////
//
// Operators
//
///////////////////////////////////////////////////////////////

TEST(CVector4D, Addition)
{
    CVector4D a(1.0f, 2.0f, 3.0f, 4.0f);
    CVector4D b(5.0f, 6.0f, 7.0f, 8.0f);
    CVector4D c = a + b;
    EXPECT_FLOAT_EQ(c.fX, 6.0f);
    EXPECT_FLOAT_EQ(c.fY, 8.0f);
    EXPECT_FLOAT_EQ(c.fZ, 10.0f);
    EXPECT_FLOAT_EQ(c.fW, 12.0f);
}

TEST(CVector4D, Subtraction)
{
    CVector4D a(5.0f, 6.0f, 7.0f, 8.0f);
    CVector4D b(1.0f, 2.0f, 3.0f, 4.0f);
    CVector4D c = a - b;
    EXPECT_FLOAT_EQ(c.fX, 4.0f);
    EXPECT_FLOAT_EQ(c.fY, 4.0f);
    EXPECT_FLOAT_EQ(c.fZ, 4.0f);
    EXPECT_FLOAT_EQ(c.fW, 4.0f);
}

TEST(CVector4D, VectorMultiply)
{
    CVector4D a(2.0f, 3.0f, 4.0f, 5.0f);
    CVector4D b(6.0f, 7.0f, 8.0f, 9.0f);
    CVector4D c = a * b;
    EXPECT_FLOAT_EQ(c.fX, 12.0f);
    EXPECT_FLOAT_EQ(c.fY, 21.0f);
    EXPECT_FLOAT_EQ(c.fZ, 32.0f);
    EXPECT_FLOAT_EQ(c.fW, 45.0f);
}

TEST(CVector4D, ScalarMultiply)
{
    CVector4D v(1.0f, 2.0f, 3.0f, 4.0f);
    CVector4D result = v * 3.0f;
    EXPECT_FLOAT_EQ(result.fX, 3.0f);
    EXPECT_FLOAT_EQ(result.fY, 6.0f);
    EXPECT_FLOAT_EQ(result.fZ, 9.0f);
    EXPECT_FLOAT_EQ(result.fW, 12.0f);
}

TEST(CVector4D, ScalarDivide)
{
    CVector4D v(10.0f, 20.0f, 30.0f, 40.0f);
    CVector4D result = v / 10.0f;
    EXPECT_FLOAT_EQ(result.fX, 1.0f);
    EXPECT_FLOAT_EQ(result.fY, 2.0f);
    EXPECT_FLOAT_EQ(result.fZ, 3.0f);
    EXPECT_FLOAT_EQ(result.fW, 4.0f);
}

TEST(CVector4D, VectorDivide)
{
    CVector4D a(10.0f, 20.0f, 30.0f, 40.0f);
    CVector4D b(2.0f, 4.0f, 5.0f, 8.0f);
    CVector4D c = a / b;
    EXPECT_FLOAT_EQ(c.fX, 5.0f);
    EXPECT_FLOAT_EQ(c.fY, 5.0f);
    EXPECT_FLOAT_EQ(c.fZ, 6.0f);
    EXPECT_FLOAT_EQ(c.fW, 5.0f);
}

TEST(CVector4D, CompoundAddScalar)
{
    CVector4D v(1.0f, 2.0f, 3.0f, 4.0f);
    v += 10.0f;
    EXPECT_FLOAT_EQ(v.fX, 11.0f);
    EXPECT_FLOAT_EQ(v.fW, 14.0f);
}

TEST(CVector4D, CompoundSubVector)
{
    CVector4D v(10.0f, 20.0f, 30.0f, 40.0f);
    v -= CVector4D(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.fX, 9.0f);
    EXPECT_FLOAT_EQ(v.fW, 36.0f);
}

TEST(CVector4D, CompoundMulScalar)
{
    CVector4D v(2.0f, 3.0f, 4.0f, 5.0f);
    v *= 3.0f;
    EXPECT_FLOAT_EQ(v.fX, 6.0f);
    EXPECT_FLOAT_EQ(v.fW, 15.0f);
}

TEST(CVector4D, CompoundDivScalar)
{
    CVector4D v(10.0f, 20.0f, 30.0f, 40.0f);
    v /= 10.0f;
    EXPECT_FLOAT_EQ(v.fX, 1.0f);
    EXPECT_FLOAT_EQ(v.fW, 4.0f);
}

///////////////////////////////////////////////////////////////
//
// Equality
//
///////////////////////////////////////////////////////////////

TEST(CVector4D, Equality)
{
    CVector4D a(1.0f, 2.0f, 3.0f, 4.0f);
    CVector4D b(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(CVector4D, Inequality)
{
    CVector4D a(1.0f, 2.0f, 3.0f, 4.0f);
    CVector4D b(1.0f, 2.0f, 3.0f, 5.0f);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}

TEST(CVector4D, CopyAssignment)
{
    CVector4D a(1.0f, 2.0f, 3.0f, 4.0f);
    CVector4D b;
    b = a;
    EXPECT_TRUE(a == b);
}
