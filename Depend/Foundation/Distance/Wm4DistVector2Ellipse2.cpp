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
#include "Wm4DistVector2Ellipse2.h"

namespace Wm4
{
//----------------------------------------------------------------------------
template <class Real>
DistVector2Ellipse2<Real>::DistVector2Ellipse2 (const Vector2<Real>& rkVector,
    const Ellipse2<Real>& rkEllipse)
    :
    m_rkVector(rkVector),
    m_rkEllipse(rkEllipse)
{
}
//----------------------------------------------------------------------------
template <class Real>
const Vector2<Real>& DistVector2Ellipse2<Real>::GetVector () const
{
    return m_rkVector;
}
//----------------------------------------------------------------------------
template <class Real>
const Ellipse2<Real>& DistVector2Ellipse2<Real>::GetEllipse () const
{
    return m_rkEllipse;
}
//----------------------------------------------------------------------------
template <class Real>
Real DistVector2Ellipse2<Real>::Get ()
{
    return Math<Real>::Sqrt(GetSquared());
}
//----------------------------------------------------------------------------
template <class Real>
Real DistVector2Ellipse2<Real>::GetSquared ()
{
    // compute coordinates of point in ellipse coordinate system
    Vector2<Real> kDiff = m_rkVector - m_rkEllipse.Center;
    Vector2<Real> kEPoint(kDiff.Dot(m_rkEllipse.Axis[0]),
        kDiff.Dot(m_rkEllipse.Axis[1]));

    const Real* afExtent = m_rkEllipse.Extent;
    Real fA2 = afExtent[0]*afExtent[0];
    Real fB2 = afExtent[1]*afExtent[1];
    Real fU2 = kEPoint.X()*kEPoint.X();
    Real fV2 = kEPoint.Y()*kEPoint.Y();
    Real fA2U2 = fA2*fU2, fB2V2 = fB2*fV2;
    Real fDx, fDy, fXDivA, fYDivB, fSqrDistance;
    Vector2<Real> kClosest;

    // handle points near the coordinate axes
    if (Math<Real>::FAbs(kEPoint.X()) <= Math<Real>::ZERO_TOLERANCE)
    {
        if (afExtent[0] >= afExtent[1] 
        ||  Math<Real>::FAbs(kEPoint.Y()) >= afExtent[1] - fA2/afExtent[1])
        {
            kClosest.X() = (Real)0.0;
            if (kEPoint.Y() >= (Real)0.0)
            {
                kClosest.Y() = afExtent[1];
            }
            else
            {
                kClosest.Y() = -afExtent[1];
            }

            fDy = kClosest.Y() - kEPoint.Y();
            fSqrDistance = fDy*fDy;

            m_kClosestPoint0 = m_rkVector;
            m_kClosestPoint1 = m_rkEllipse.Center +
                kClosest.X()*m_rkEllipse.Axis[0] +
                kClosest.Y()*m_rkEllipse.Axis[1];
            return fSqrDistance;
        }
        else
        {
            kClosest.Y() = fB2*kEPoint.Y()/(fB2-fA2);
            fDy = kClosest.Y() - kEPoint.Y();
            fYDivB = kClosest.Y()/afExtent[1];
            kClosest.X() = afExtent[0]*Math<Real>::Sqrt(
                Math<Real>::FAbs((Real)1.0-fYDivB*fYDivB));

            fSqrDistance = kClosest.X()*kClosest.X() + fDy*fDy;

            m_kClosestPoint0 = m_rkVector;
            m_kClosestPoint1 = m_rkEllipse.Center +
                kClosest.X()*m_rkEllipse.Axis[0] +
                kClosest.Y()*m_rkEllipse.Axis[1];
            return fSqrDistance;
        }
    }

    if (Math<Real>::FAbs(kEPoint.Y()) <= Math<Real>::ZERO_TOLERANCE)
    {
        if (afExtent[1] >= afExtent[0]
        ||  Math<Real>::FAbs(kEPoint.X()) >= afExtent[0] - fB2/afExtent[0])
        {
            kClosest.Y() = (Real)0.0;
            if (kEPoint.X() >= (Real)0.0)
            {
                kClosest.X() = afExtent[0];
            }
            else
            {
                kClosest.X() = -afExtent[0];
            }

            fDx = kClosest.X() - kEPoint.X();
            fSqrDistance = fDx*fDx;

            m_kClosestPoint0 = m_rkVector;
            m_kClosestPoint1 = m_rkEllipse.Center +
                m_rkEllipse.Axis[0]*kClosest.X() +
                m_rkEllipse.Axis[1]*kClosest.Y();
            return Math<Real>::FAbs(fDx);
        }
        else
        {
            kClosest.X() = fA2*kEPoint.X()/(fA2-fB2);
            fDx = kClosest.X() - kEPoint.X();
            fXDivA = kClosest.X()/afExtent[0];
            kClosest.Y() = afExtent[1]*Math<Real>::Sqrt(
                Math<Real>::FAbs((Real)1.0-fXDivA*fXDivA));

            fSqrDistance = kClosest.Y()*kClosest.Y() + fDx*fDx;

            m_kClosestPoint0 = m_rkVector;
            m_kClosestPoint1 = m_rkEllipse.Center +
                kClosest.X()*m_rkEllipse.Axis[0] +
                kClosest.Y()*m_rkEllipse.Axis[1];
            return fSqrDistance;
        }
    }

    // initial guess
    Real fURatio = kEPoint.X()/afExtent[0];
    Real fVRatio = kEPoint.Y()/afExtent[1];
    Real fT;
    if (fURatio*fURatio + fVRatio*fVRatio < (Real)1.0)
    {
        fT = (Real)0.0;
    }
    else
    {
        Real fMax = afExtent[0];
        if (afExtent[1] > fMax)
        {
            fMax = afExtent[1];
        }

        fT = fMax*kEPoint.Length();
    }

    // Newton's method
    const int iMaxIteration = 64;
    Real fP, fQ;
    for (int i = 0; i < iMaxIteration; i++)
    {
        fP = fT+fA2;
        fQ = fT+fB2;
        Real fP2 = fP*fP;
        Real fQ2 = fQ*fQ;
        Real fR = fP2*fQ2-fA2U2*fQ2-fB2V2*fP2;
        if (Math<Real>::FAbs(fR) < Math<Real>::ZERO_TOLERANCE)
        {
            break;
        }

        Real fDR = ((Real)2.0)*(fP*fQ*(fP+fQ)-fA2U2*fQ-fB2V2*fP);
        fT -= fR/fDR;
    }

    kClosest.X() = fA2*kEPoint.X()/fP;
    kClosest.Y() = fB2*kEPoint.Y()/fQ;
    kDiff = kClosest - kEPoint;
    fSqrDistance = kDiff.SquaredLength();

    m_kClosestPoint0 = m_rkVector;
    m_kClosestPoint1 = m_rkEllipse.Center +
        kClosest.X()*m_rkEllipse.Axis[0] +
        kClosest.Y()*m_rkEllipse.Axis[1];

    return fSqrDistance;
}
//----------------------------------------------------------------------------
template <class Real>
Real DistVector2Ellipse2<Real>::Get (Real fT,
    const Vector2<Real>& rkVelocity0, const Vector2<Real>& rkVelocity1)
{
    Vector2<Real> kMVector = m_rkVector + fT*rkVelocity0;
    Vector2<Real> kMCenter = m_rkEllipse.Center + fT*rkVelocity1;
    Ellipse2<Real> kMEllipse(kMCenter,m_rkEllipse.Axis,m_rkEllipse.Extent);
    return DistVector2Ellipse2<Real>(kMVector,kMEllipse).Get();
}
//----------------------------------------------------------------------------
template <class Real>
Real DistVector2Ellipse2<Real>::GetSquared (Real fT,
    const Vector2<Real>& rkVelocity0, const Vector2<Real>& rkVelocity1)
{
    Vector2<Real> kMVector = m_rkVector + fT*rkVelocity0;
    Vector2<Real> kMCenter = m_rkEllipse.Center + fT*rkVelocity1;
    Ellipse2<Real> kMEllipse(kMCenter,m_rkEllipse.Axis,m_rkEllipse.Extent);
    return DistVector2Ellipse2<Real>(kMVector,kMEllipse).GetSquared();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template WM4_FOUNDATION_ITEM
class DistVector2Ellipse2<float>;

template WM4_FOUNDATION_ITEM
class DistVector2Ellipse2<double>;
//----------------------------------------------------------------------------
}
