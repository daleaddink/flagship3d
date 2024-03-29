// Geometric Tools, Inc.
// http://www.geometrictools.com
// Copyright (c) 1998-2006.  All Rights Reserved
//
// The Wild Magic Version 4 Foundation Library source code is supplied
// under the terms of the license agreement
//     http://www.geometrictools.com/License/Wm4FoundationLicense.pdf
// and may not be copied or disclosed except in accordance with the terms
// of that agreement.

#include "Wm4FoundationPCH.h"
#include "Wm4IntrRay3Ellipsoid3.h"

namespace Wm4
{
//----------------------------------------------------------------------------
template <class Real>
IntrRay3Ellipsoid3<Real>::IntrRay3Ellipsoid3 (const Ray3<Real>& rkRay,
    const Ellipsoid3<Real>& rkEllipsoid)
    :
    m_rkRay(rkRay),
    m_rkEllipsoid(rkEllipsoid)
{
}
//----------------------------------------------------------------------------
template <class Real>
const Ray3<Real>& IntrRay3Ellipsoid3<Real>::GetRay () const
{
    return m_rkRay;
}
//----------------------------------------------------------------------------
template <class Real>
const Ellipsoid3<Real>& IntrRay3Ellipsoid3<Real>::GetEllipsoid () const
{
    return m_rkEllipsoid;
}
//----------------------------------------------------------------------------
template <class Real>
bool IntrRay3Ellipsoid3<Real>::Test ()
{
    // The ellipsoid is (X-K)^T*M*(X-K)-1 = 0 and the ray is X = P+t*D.
    // Substitute the ray equation into the ellipsoid equation to obtain
    // a quadratic equation
    //   Q(t) = a2*t^2 + 2*a1*t + a0 = 0
    // where a2 = D^T*M*D, a1 = D^T*M*(P-K), and a0 = (P-K)^T*M*(P-K)-1.

    Matrix3<Real> kM;
    m_rkEllipsoid.GetM(kM);

    Vector3<Real> kDiff = m_rkRay.Origin - m_rkEllipsoid.Center;
    Vector3<Real> kMatDir = kM*m_rkRay.Direction;
    Vector3<Real> kMatDiff = kM*kDiff;
    Real fA2 = m_rkRay.Direction.Dot(kMatDir);
    Real fA1 = m_rkRay.Direction.Dot(kMatDiff);
    Real fA0 = kDiff.Dot(kMatDiff) - (Real)1.0;

    // no intersection if Q(t) has no real roots
    Real fDiscr = fA1*fA1 - fA0*fA2;
    if (fDiscr < (Real)0.0)
    {
        return false;
    }

    // test if ray origin is inside ellipsoid
    if (fA0 <= (Real)0.0)
    {
        return true;
    }

    // At this point, Q(0) = fA0 > 0 and Q(t) has real roots.  It is also
    // the case that fA2 > 0, since M is positive definite, implying that
    // D^T*M*D > 0 for any nonzero vector D.  Thus, an intersection occurs
    // only when Q'(0) < 0.
    return fA1 < (Real)0.0;
}
//----------------------------------------------------------------------------
template <class Real>
bool IntrRay3Ellipsoid3<Real>::Find ()
{
    // The ellipsoid is (X-K)^T*M*(X-K)-1 = 0 and the line is X = P+t*D.
    // Substitute the line equation into the ellipsoid equation to obtain
    // a quadratic equation
    //   Q(t) = a2*t^2 + 2*a1*t + a0 = 0
    // where a2 = D^T*M*D, a1 = D^T*M*(P-K), and a0 = (P-K)^T*M*(P-K)-1.

    Matrix3<Real> kM;
    m_rkEllipsoid.GetM(kM);

    Vector3<Real> kDiff = m_rkRay.Origin - m_rkEllipsoid.Center;
    Vector3<Real> kMatDir = kM*m_rkRay.Direction;
    Vector3<Real> kMatDiff = kM*kDiff;
    Real fA2 = m_rkRay.Direction.Dot(kMatDir);
    Real fA1 = m_rkRay.Direction.Dot(kMatDiff);
    Real fA0 = kDiff.Dot(kMatDiff) - (Real)1.0;

    // intersection occurs if Q(t) has real roots with t >= 0
    Real fDiscr = fA1*fA1 - fA0*fA2;
    Real afT[2];
    if (fDiscr < (Real)0.0)
    {
        m_iIntersectionType = IT_EMPTY;
        m_iQuantity = 0;
    }
    else if (fDiscr > (Real)0.0)
    {
        Real fRoot = Math<Real>::Sqrt(fDiscr);
        Real fInv = ((Real)1.0)/fA2;
        afT[0] = (-fA1 - fRoot)*fInv;
        afT[1] = (-fA1 + fRoot)*fInv;

        if (afT[0] >= (Real)0.0)
        {
            m_iIntersectionType = IT_SEGMENT;
            m_iQuantity = 2;
            m_akPoint[0] = m_rkRay.Origin + afT[0]*m_rkRay.Direction;
            m_akPoint[1] = m_rkRay.Origin + afT[1]*m_rkRay.Direction;
        }
        else if (afT[1] >= (Real)0.0)
        {
            m_iIntersectionType = IT_POINT;
            m_iQuantity = 1;
            m_akPoint[0] = m_rkRay.Origin + afT[1]*m_rkRay.Direction;
        }
        else
        {
            m_iIntersectionType = IT_EMPTY;
            m_iQuantity = 0;
        }
    }
    else
    {
        afT[0] = -fA1/fA2;
        if (afT[0] >= (Real)0.0)
        {
            m_iIntersectionType = IT_POINT;
            m_iQuantity = 1;
            m_akPoint[0] = m_rkRay.Origin + afT[0]*m_rkRay.Direction;
        }
        else
        {
            m_iIntersectionType = IT_EMPTY;
            m_iQuantity = 0;
        }
    }

    return m_iIntersectionType != IT_EMPTY;
}
//----------------------------------------------------------------------------
template <class Real>
int IntrRay3Ellipsoid3<Real>::GetQuantity () const
{
    return m_iQuantity;
}
//----------------------------------------------------------------------------
template <class Real>
const Vector3<Real>& IntrRay3Ellipsoid3<Real>::GetPoint (int i) const
{
    assert(0 <= i && i < m_iQuantity);
    return m_akPoint[i];
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template WM4_FOUNDATION_ITEM
class IntrRay3Ellipsoid3<float>;

template WM4_FOUNDATION_ITEM
class IntrRay3Ellipsoid3<double>;
//----------------------------------------------------------------------------
}
