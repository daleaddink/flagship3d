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
#include "Wm4DistVector3Box3.h"

namespace Wm4
{
//----------------------------------------------------------------------------
template <class Real>
DistVector3Box3<Real>::DistVector3Box3 (const Vector3<Real>& rkVector,
    const Box3<Real>& rkBox)
    :
    m_rkVector(rkVector),
    m_rkBox(rkBox)
{
}
//----------------------------------------------------------------------------
template <class Real>
const Vector3<Real>& DistVector3Box3<Real>::GetVector () const
{
    return m_rkVector;
}
//----------------------------------------------------------------------------
template <class Real>
const Box3<Real>& DistVector3Box3<Real>::GetBox () const
{
    return m_rkBox;
}
//----------------------------------------------------------------------------
template <class Real>
Real DistVector3Box3<Real>::Get ()
{
    return Math<Real>::Sqrt(GetSquared());
}
//----------------------------------------------------------------------------
template <class Real>
Real DistVector3Box3<Real>::GetSquared ()
{
    // work in the box's coordinate system
    Vector3<Real> kDiff = m_rkVector - m_rkBox.Center;

    // compute squared distance and closest point on box
    Real fSqrDistance = (Real)0.0, fDelta;
    Vector3<Real> kClosest;
    int i;
    for (i = 0; i < 3; i++)
    {
        kClosest[i] = kDiff.Dot(m_rkBox.Axis[i]);
        if (kClosest[i] < -m_rkBox.Extent[i])
        {
            fDelta = kClosest[i] + m_rkBox.Extent[i];
            fSqrDistance += fDelta*fDelta;
            kClosest[i] = -m_rkBox.Extent[i];
        }
        else if (kClosest[i] > m_rkBox.Extent[i])
        {
            fDelta = kClosest[i] - m_rkBox.Extent[i];
            fSqrDistance += fDelta*fDelta;
            kClosest[i] = m_rkBox.Extent[i];
        }
    }

    m_kClosestPoint0 = m_rkVector;
    m_kClosestPoint1 = m_rkBox.Center;
    for (i = 0; i < 3; i++)
    {
        m_kClosestPoint1 += kClosest[i]*m_rkBox.Axis[i];
    }

    return fSqrDistance;
}
//----------------------------------------------------------------------------
template <class Real>
Real DistVector3Box3<Real>::Get (Real fT, const Vector3<Real>& rkVelocity0,
    const Vector3<Real>& rkVelocity1)
{
    Vector3<Real> kMVector = m_rkVector + fT*rkVelocity0;
    Vector3<Real> kMCenter = m_rkBox.Center + fT*rkVelocity1;
    Box3<Real> kMBox(kMCenter,m_rkBox.Axis,m_rkBox.Extent);
    return DistVector3Box3<Real>(kMVector,kMBox).Get();
}
//----------------------------------------------------------------------------
template <class Real>
Real DistVector3Box3<Real>::GetSquared (Real fT,
    const Vector3<Real>& rkVelocity0, const Vector3<Real>& rkVelocity1)
{
    Vector3<Real> kMVector = m_rkVector + fT*rkVelocity0;
    Vector3<Real> kMCenter = m_rkBox.Center + fT*rkVelocity1;
    Box3<Real> kMBox(kMCenter,m_rkBox.Axis,m_rkBox.Extent);
    return DistVector3Box3<Real>(kMVector,kMBox).GetSquared();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template WM4_FOUNDATION_ITEM
class DistVector3Box3<float>;

template WM4_FOUNDATION_ITEM
class DistVector3Box3<double>;
//----------------------------------------------------------------------------
}
