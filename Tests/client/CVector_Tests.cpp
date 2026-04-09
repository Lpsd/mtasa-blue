/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Tests/client/CVector_Tests.cpp
 *  PURPOSE:     Google Test suite for CVector (3D vector math)
 *
 *****************************************************************************/

#include <gtest/gtest.h>
#include <SharedUtil.Defines.h>
#include <CVector.h>
#include <CVector2D.h>

static constexpr float kEpsilon = 0.001f;

///////////////////////////////////////////////////////////////
//
// Construction
//
///////////////////////////////////////////////////////////////

TEST(CVector, DefaultConstructor)
{
    CVector v;
    EXPECT_FLOAT_EQ(v.fX, 0.0f);
    EXPECT_FLOAT_EQ(v.fY, 0.0f);
    EXPECT_FLOAT_EQ(v.fZ, 0.0f);
}

TEST(CVector, ExplicitConstructor)
{
    CVector v(1.0f, 2.0f, 3.0f);
    EXPECT_FLOAT_EQ(v.fX, 1.0f);
    EXPECT_FLOAT_EQ(v.fY, 2.0f);
    EXPECT_FLOAT_EQ(v.fZ, 3.0f);
}

TEST(CVector, PartialConstructor)
{
    CVector v(5.0f);
    EXPECT_FLOAT_EQ(v.fX, 5.0f);
    EXPECT_FLOAT_EQ(v.fY, 0.0f);
    EXPECT_FLOAT_EQ(v.fZ, 0.0f);
}

TEST(CVector, FromCVector4D)
{
    CVector4D v4(1.0f, 2.0f, 3.0f, 4.0f);
    CVector   v(v4);
    EXPECT_FLOAT_EQ(v.fX, 1.0f);
    EXPECT_FLOAT_EQ(v.fY, 2.0f);
    EXPECT_FLOAT_EQ(v.fZ, 3.0f);
}

///////////////////////////////////////////////////////////////
//
// Length / Normalize
//
///////////////////////////////////////////////////////////////

TEST(CVector, Length)
{
    CVector v(3.0f, 4.0f, 0.0f);
    EXPECT_NEAR(v.Length(), 5.0f, kEpsilon);
}

TEST(CVector, LengthSquared)
{
    CVector v(3.0f, 4.0f, 0.0f);
    EXPECT_NEAR(v.LengthSquared(), 25.0f, kEpsilon);
}

TEST(CVector, Normalize)
{
    CVector v(0.0f, 3.0f, 4.0f);
    float   origLen = v.Normalize();
    EXPECT_NEAR(origLen, 5.0f, kEpsilon);
    EXPECT_NEAR(v.Length(), 1.0f, kEpsilon);
}

TEST(CVector, NormalizeZeroVector)
{
    CVector v(0.0f, 0.0f, 0.0f);
    float   len = v.Normalize();
    EXPECT_FLOAT_EQ(len, 0.0f);
}

///////////////////////////////////////////////////////////////
//
// Dot / Cross product
//
///////////////////////////////////////////////////////////////

TEST(CVector, DotProduct)
{
    CVector a(1.0f, 2.0f, 3.0f);
    CVector b(4.0f, 5.0f, 6.0f);
    EXPECT_NEAR(a.DotProduct(&b), 32.0f, kEpsilon);
}

TEST(CVector, DotProductPerpendicular)
{
    CVector a(1.0f, 0.0f, 0.0f);
    CVector b(0.0f, 1.0f, 0.0f);
    EXPECT_NEAR(a.DotProduct(&b), 0.0f, kEpsilon);
}

TEST(CVector, CrossProduct)
{
    CVector a(1.0f, 0.0f, 0.0f);
    CVector b(0.0f, 1.0f, 0.0f);
    a.CrossProduct(&b);
    EXPECT_NEAR(a.fX, 0.0f, kEpsilon);
    EXPECT_NEAR(a.fY, 0.0f, kEpsilon);
    EXPECT_NEAR(a.fZ, 1.0f, kEpsilon);
}

///////////////////////////////////////////////////////////////
//
// Arithmetic operators
//
///////////////////////////////////////////////////////////////

TEST(CVector, Addition)
{
    CVector a(1.0f, 2.0f, 3.0f);
    CVector b(4.0f, 5.0f, 6.0f);
    CVector c = a + b;
    EXPECT_FLOAT_EQ(c.fX, 5.0f);
    EXPECT_FLOAT_EQ(c.fY, 7.0f);
    EXPECT_FLOAT_EQ(c.fZ, 9.0f);
}

TEST(CVector, Subtraction)
{
    CVector a(4.0f, 5.0f, 6.0f);
    CVector b(1.0f, 2.0f, 3.0f);
    CVector c = a - b;
    EXPECT_FLOAT_EQ(c.fX, 3.0f);
    EXPECT_FLOAT_EQ(c.fY, 3.0f);
    EXPECT_FLOAT_EQ(c.fZ, 3.0f);
}

TEST(CVector, Negation)
{
    CVector a(1.0f, -2.0f, 3.0f);
    CVector b = -a;
    EXPECT_FLOAT_EQ(b.fX, -1.0f);
    EXPECT_FLOAT_EQ(b.fY, 2.0f);
    EXPECT_FLOAT_EQ(b.fZ, -3.0f);
}

TEST(CVector, ScalarMultiply)
{
    CVector a(1.0f, 2.0f, 3.0f);
    CVector b = a * 2.0f;
    EXPECT_FLOAT_EQ(b.fX, 2.0f);
    EXPECT_FLOAT_EQ(b.fY, 4.0f);
    EXPECT_FLOAT_EQ(b.fZ, 6.0f);
}

TEST(CVector, ScalarDivide)
{
    CVector a(4.0f, 6.0f, 8.0f);
    CVector b = a / 2.0f;
    EXPECT_FLOAT_EQ(b.fX, 2.0f);
    EXPECT_FLOAT_EQ(b.fY, 3.0f);
    EXPECT_FLOAT_EQ(b.fZ, 4.0f);
}

TEST(CVector, CompoundAdd)
{
    CVector a(1.0f, 2.0f, 3.0f);
    a += CVector(10.0f, 20.0f, 30.0f);
    EXPECT_FLOAT_EQ(a.fX, 11.0f);
    EXPECT_FLOAT_EQ(a.fY, 22.0f);
    EXPECT_FLOAT_EQ(a.fZ, 33.0f);
}

TEST(CVector, CompoundSubtract)
{
    CVector a(10.0f, 20.0f, 30.0f);
    a -= CVector(1.0f, 2.0f, 3.0f);
    EXPECT_FLOAT_EQ(a.fX, 9.0f);
    EXPECT_FLOAT_EQ(a.fY, 18.0f);
    EXPECT_FLOAT_EQ(a.fZ, 27.0f);
}

///////////////////////////////////////////////////////////////
//
// Comparison / Validity
//
///////////////////////////////////////////////////////////////

TEST(CVector, Equality)
{
    CVector a(1.0f, 2.0f, 3.0f);
    CVector b(1.0f, 2.0f, 3.0f);
    CVector c(1.0f, 2.0f, 3.1f);
    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a != c);
}

TEST(CVector, IsValid)
{
    CVector valid(1.0f, 2.0f, 3.0f);
    EXPECT_TRUE(valid.IsValid());

    CVector nan(std::numeric_limits<float>::quiet_NaN(), 0.0f, 0.0f);
    EXPECT_FALSE(nan.IsValid());

    CVector inf(0.0f, std::numeric_limits<float>::infinity(), 0.0f);
    EXPECT_FALSE(inf.IsValid());
}

TEST(CVector, IndexOperator)
{
    CVector v(10.0f, 20.0f, 30.0f);
    EXPECT_FLOAT_EQ(v[0], 10.0f);
    EXPECT_FLOAT_EQ(v[1], 20.0f);
    EXPECT_FLOAT_EQ(v[2], 30.0f);
}

///////////////////////////////////////////////////////////////
//
// Geometry helpers
//
///////////////////////////////////////////////////////////////

TEST(CVector, GetOtherAxis)
{
    CVector v(1.0f, 0.0f, 0.0f);
    CVector perp = v.GetOtherAxis();
    // Must be perpendicular (dot ~ 0) and unit length
    EXPECT_NEAR(v.DotProduct(&perp), 0.0f, kEpsilon);
    EXPECT_NEAR(perp.Length(), 1.0f, kEpsilon);
}
