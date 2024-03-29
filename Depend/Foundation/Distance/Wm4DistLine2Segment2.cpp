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
#include "Wm4DistLine2Segment2.h"

namespace Wm4
{
//----------------------------------------------------------------------------
template <class Real>
DistLine2Segment2<Real>::DistLine2Segment2 (const Line2<Real>& rkLine,
    const Segment2<Real>& rkSegment)
    :
    m_rkLine(rkLine),
    m_rkSegment(rkSegment)
{
}
//----------------------------------------------------------------------------
template <class Real>
const Line2<Real>& DistLine2Segment2<Real>::GetLine () const
{
    return m_rkLine;
}
//----------------------------------------------------------------------------
template <class Real>
const Segment2<Real>& DistLine2Segment2<Real>::GetSegment () const
{
    return m_rkSegment;
}
//----------------------------------------------------------------------------
template <class Real>
Real DistLine2Segment2<Real>::Get ()
{
    Real fSqrDist = GetSquared();
    return Math<Real>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
template <class Real>
Real DistLine2Segment2<Real>::GetSquared ()
{
    Vector2<Real> kDiff = m_rkLine.Origin - m_rkSegment.Origin;
    Real fA01 = -m_rkLine.Direction.Dot(m_rkSegment.Direction);
    Real fB0 = kDiff.Dot(m_rkLine.Direction);
    Real fC = kDiff.SquaredLength();
    Real fDet = Math<Real>::FAbs((Real)1.0 - fA01*fA01);
    Real fB1, fS0, fS1, fSqrDist, fExtDet;

    if (fDet >= Math<Real>::ZERO_TOLERANCE)
    {
        // The line and segment are not parallel.
        fB1 = -kDiff.Dot(m_rkSegment.Direction);
        fS1 = fA01*fB0-fB1;
        fExtDet = m_rkSegment.Extent*fDet;

        if (fS1 >= -fExtDet)
        {
            if (fS1 <= fExtDet)
            {
                // Two interior points are closest, one on the line and one
                // on the segment.
                Real fInvDet = ((Real)1.0)/fDet;
                fS0 = (fA01*fB1-fB0)*fInvDet;
                fS1 *= fInvDet;
                fSqrDist = (Real)0.0;
            }
            else
            {
                // The end point e1 of the segment and an interior point of
                // the line are closest.
                fS1 = m_rkSegment.Extent;
                fS0 = -(fA01*fS1+fB0);
                fSqrDist = -fS0*fS0+fS1*(fS1+((Real)2.0)*fB1)+fC;
            }
        }
        else
        {
            // The end point e0 of the segment and an interior point of the
            // line are closest.
            fS1 = -m_rkSegment.Extent;
            fS0 = -(fA01*fS1+fB0);
            fSqrDist = -fS0*fS0+fS1*(fS1+((Real)2.0)*fB1)+fC;
        }
    }
    else
    {
        // The line and segment are parallel.  Choose the closest pair so that
        // one point is at segment origin.
        fS1 = (Real)0.0;
        fS0 = -fB0;
        fSqrDist = fB0*fS0+fC;
    }

    m_kClosestPoint0 = m_rkLine.Origin + fS0*m_rkLine.Direction;
    m_kClosestPoint1 = m_rkSegment.Origin + fS1*m_rkSegment.Direction;
    return Math<Real>::FAbs(fSqrDist);
}
//----------------------------------------------------------------------------
template <class Real>
Real DistLine2Segment2<Real>::Get (Real fT, const Vector2<Real>& rkVelocity0,
    const Vector2<Real>& rkVelocity1)
{
    Vector2<Real> kMOrigin0 = m_rkLine.Origin + fT*rkVelocity0;
    Vector2<Real> kMOrigin1 = m_rkSegment.Origin + fT*rkVelocity1;
    Line2<Real> kMLine(kMOrigin0,m_rkLine.Direction);
    Segment2<Real> kMSegment(kMOrigin1,m_rkSegment.Direction,
        m_rkSegment.Extent);
    return DistLine2Segment2<Real>(kMLine,kMSegment).Get();
}
//----------------------------------------------------------------------------
template <class Real>
Real DistLine2Segment2<Real>::GetSquared (Real fT,
    const Vector2<Real>& rkVelocity0, const Vector2<Real>& rkVelocity1)
{
    Vector2<Real> kMOrigin0 = m_rkLine.Origin + fT*rkVelocity0;
    Vector2<Real> kMOrigin1 = m_rkSegment.Origin + fT*rkVelocity1;
    Line2<Real> kMLine(kMOrigin0,m_rkLine.Direction);
    Segment2<Real> kMSegment(kMOrigin1,m_rkSegment.Direction,
        m_rkSegment.Extent);
    return DistLine2Segment2<Real>(kMLine,kMSegment).GetSquared();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template WM4_FOUNDATION_ITEM
class DistLine2Segment2<float>;

template WM4_FOUNDATION_ITEM
class DistLine2Segment2<double>;
//----------------------------------------------------------------------------
}
