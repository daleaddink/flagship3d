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
#include "Wm4IntrSphere3Frustum3.h"
#include "Wm4DistVector3Frustum3.h"

namespace Wm4
{
//----------------------------------------------------------------------------
template <class Real>
IntrSphere3Frustum3<Real>::IntrSphere3Frustum3 (const Sphere3<Real>& rkSphere,
    const Frustum3<Real>& rkFrustum)
    :
    m_rkSphere(rkSphere),
    m_rkFrustum(rkFrustum)
{
}
//----------------------------------------------------------------------------
template <class Real>
const Sphere3<Real>& IntrSphere3Frustum3<Real>::GetSphere () const
{
    return m_rkSphere;
}
//----------------------------------------------------------------------------
template <class Real>
const Frustum3<Real>& IntrSphere3Frustum3<Real>::GetFrustum () const
{
    return m_rkFrustum;
}
//----------------------------------------------------------------------------
template <class Real>
bool IntrSphere3Frustum3<Real>::Test ()
{
    Real fDist = DistVector3Frustum3<Real>(m_rkSphere.Center,
        m_rkFrustum).Get();
    return fDist <= m_rkSphere.Radius;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template WM4_FOUNDATION_ITEM
class IntrSphere3Frustum3<float>;

template WM4_FOUNDATION_ITEM
class IntrSphere3Frustum3<double>;
//----------------------------------------------------------------------------
}
