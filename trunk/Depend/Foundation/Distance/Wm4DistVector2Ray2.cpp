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
#include "Wm4DistVector2Ray2.h"

namespace Wm4
{
//----------------------------------------------------------------------------
template <class Real>
DistVector2Ray2<Real>::DistVector2Ray2 (const Vector2<Real>& rkVector,
    const Ray2<Real>& rkRay)
    :
    m_rkVector(rkVector),
    m_rkRay(rkRay)
{
}
//----------------------------------------------------------------------------
template <class Real>
const Vector2<Real>& DistVector2Ray2<Real>::GetVector () const
{
    return m_rkVector;
}
//----------------------------------------------------------------------------
template <class Real>
const Ray2<Real>& DistVector2Ray2<Real>::GetRay () const
{
    return m_rkRay;
}
//----------------------------------------------------------------------------
template <class Real>
Real DistVector2Ray2<Real>::Get ()
{
    Real fSqrDist = GetSquared();
    return Math<Real>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
template <class Real>
Real DistVector2Ray2<Real>::GetSquared ()
{
    Vector2<Real> kDiff = m_rkVector - m_rkRay.Origin;
    Real fParam = m_rkRay.Direction.Dot(kDiff);
    if (fParam > (Real)0.0)
    {
        m_kClosestPoint1 = m_rkRay.Origin + fParam*m_rkRay.Direction;
    }
    else
    {
        m_kClosestPoint1 = m_rkRay.Origin;
    }

    m_kClosestPoint0 = m_rkVector;
    kDiff = m_kClosestPoint1 - m_kClosestPoint0;
    return kDiff.SquaredLength();
}
//----------------------------------------------------------------------------
template <class Real>
Real DistVector2Ray2<Real>::Get (Real fT, const Vector2<Real>& rkVelocity0,
    const Vector2<Real>& rkVelocity1)
{
    Vector2<Real> kMVector = m_rkVector + fT*rkVelocity0;
    Vector2<Real> kMOrigin = m_rkRay.Origin + fT*rkVelocity1;
    Ray2<Real> kMRay(kMOrigin,m_rkRay.Direction);
    return DistVector2Ray2<Real>(kMVector,kMRay).Get();
}
//----------------------------------------------------------------------------
template <class Real>
Real DistVector2Ray2<Real>::GetSquared (Real fT,
    const Vector2<Real>& rkVelocity0, const Vector2<Real>& rkVelocity1)
{
    Vector2<Real> kMVector = m_rkVector + fT*rkVelocity0;
    Vector2<Real> kMOrigin = m_rkRay.Origin + fT*rkVelocity1;
    Ray2<Real> kMRay(kMOrigin,m_rkRay.Direction);
    return DistVector2Ray2<Real>(kMVector,kMRay).GetSquared();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template WM4_FOUNDATION_ITEM
class DistVector2Ray2<float>;

template WM4_FOUNDATION_ITEM
class DistVector2Ray2<double>;
//----------------------------------------------------------------------------
}
