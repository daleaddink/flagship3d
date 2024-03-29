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
#include "Wm4IntrRay3Box3.h"

namespace Wm4
{
//----------------------------------------------------------------------------
template <class Real>
IntrRay3Box3<Real>::IntrRay3Box3 (const Ray3<Real>& rkRay,
    const Box3<Real>& rkBox)
    :
    m_rkRay(rkRay),
    m_rkBox(rkBox)
{
}
//----------------------------------------------------------------------------
template <class Real>
const Ray3<Real>& IntrRay3Box3<Real>::GetRay () const
{
    return m_rkRay;
}
//----------------------------------------------------------------------------
template <class Real>
const Box3<Real>& IntrRay3Box3<Real>::GetBox () const
{
    return m_rkBox;
}
//----------------------------------------------------------------------------
template <class Real>
bool IntrRay3Box3<Real>::Test ()
{
    Real afWdU[3], afAWdU[3], afDdU[3], afADdU[3], afAWxDdU[3], fRhs;

    Vector3<Real> kDiff = m_rkRay.Origin - m_rkBox.Center;

    afWdU[0] = m_rkRay.Direction.Dot(m_rkBox.Axis[0]);
    afAWdU[0] = Math<Real>::FAbs(afWdU[0]);
    afDdU[0] = kDiff.Dot(m_rkBox.Axis[0]);
    afADdU[0] = Math<Real>::FAbs(afDdU[0]);
    if (afADdU[0] > m_rkBox.Extent[0] && afDdU[0]*afWdU[0] >= (Real)0.0)
    {
        return false;
    }

    afWdU[1] = m_rkRay.Direction.Dot(m_rkBox.Axis[1]);
    afAWdU[1] = Math<Real>::FAbs(afWdU[1]);
    afDdU[1] = kDiff.Dot(m_rkBox.Axis[1]);
    afADdU[1] = Math<Real>::FAbs(afDdU[1]);
    if (afADdU[1] > m_rkBox.Extent[1] && afDdU[1]*afWdU[1] >= (Real)0.0)
    {
        return false;
    }

    afWdU[2] = m_rkRay.Direction.Dot(m_rkBox.Axis[2]);
    afAWdU[2] = Math<Real>::FAbs(afWdU[2]);
    afDdU[2] = kDiff.Dot(m_rkBox.Axis[2]);
    afADdU[2] = Math<Real>::FAbs(afDdU[2]);
    if (afADdU[2] > m_rkBox.Extent[2] && afDdU[2]*afWdU[2] >= (Real)0.0)
    {
        return false;
    }

    Vector3<Real> kWxD = m_rkRay.Direction.Cross(kDiff);

    afAWxDdU[0] = Math<Real>::FAbs(kWxD.Dot(m_rkBox.Axis[0]));
    fRhs = m_rkBox.Extent[1]*afAWdU[2] + m_rkBox.Extent[2]*afAWdU[1];
    if (afAWxDdU[0] > fRhs)
    {
        return false;
    }

    afAWxDdU[1] = Math<Real>::FAbs(kWxD.Dot(m_rkBox.Axis[1]));
    fRhs = m_rkBox.Extent[0]*afAWdU[2] + m_rkBox.Extent[2]*afAWdU[0];
    if (afAWxDdU[1] > fRhs)
    {
        return false;
    }

    afAWxDdU[2] = Math<Real>::FAbs(kWxD.Dot(m_rkBox.Axis[2]));
    fRhs = m_rkBox.Extent[0]*afAWdU[1] + m_rkBox.Extent[1]*afAWdU[0];
    if (afAWxDdU[2] > fRhs)
    {
        return false;
    }

    return true;
}
//----------------------------------------------------------------------------
template <class Real>
bool IntrRay3Box3<Real>::Find ()
{
    Real fT0 = (Real)0.0, fT1 = Math<Real>::MAX_REAL;
    return IntrLine3Box3<Real>::DoClipping(fT0,fT1,m_rkRay.Origin,
        m_rkRay.Direction,m_rkBox,true,m_iQuantity,m_akPoint,
        m_iIntersectionType);
}
//----------------------------------------------------------------------------
template <class Real>
int IntrRay3Box3<Real>::GetQuantity () const
{
    return m_iQuantity;
}
//----------------------------------------------------------------------------
template <class Real>
const Vector3<Real>& IntrRay3Box3<Real>::GetPoint (int i) const
{
    assert(0 <= i && i < m_iQuantity);
    return m_akPoint[i];
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template WM4_FOUNDATION_ITEM
class IntrRay3Box3<float>;

template WM4_FOUNDATION_ITEM
class IntrRay3Box3<double>;
//----------------------------------------------------------------------------
}
