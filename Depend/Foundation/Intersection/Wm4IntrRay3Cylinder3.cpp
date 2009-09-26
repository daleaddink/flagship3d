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
#include "Wm4IntrRay3Cylinder3.h"
#include "Wm4IntrLine3Cylinder3.h"

namespace Wm4
{
//----------------------------------------------------------------------------
template <class Real>
IntrRay3Cylinder3<Real>::IntrRay3Cylinder3 (const Ray3<Real>& rkRay,
    const Cylinder3<Real>& rkCylinder)
    :
    m_rkRay(rkRay),
    m_rkCylinder(rkCylinder)
{
}
//----------------------------------------------------------------------------
template <class Real>
const Ray3<Real>& IntrRay3Cylinder3<Real>::GetRay () const
{
    return m_rkRay;
}
//----------------------------------------------------------------------------
template <class Real>
const Cylinder3<Real>& IntrRay3Cylinder3<Real>::GetCylinder () const
{
    return m_rkCylinder;
}
//----------------------------------------------------------------------------
template <class Real>
bool IntrRay3Cylinder3<Real>::Find ()
{
    Real afT[2];
    int iQuantity = IntrLine3Cylinder3<Real>::Find(m_rkRay.Origin,
        m_rkRay.Direction,m_rkCylinder,afT);

    m_iQuantity = 0;
    for (int i = 0; i < iQuantity; i++)
    {
        if (afT[i] >= (Real)0.0)
        {
            m_akPoint[m_iQuantity++] = m_rkRay.Origin +
                afT[i]*m_rkRay.Direction;
        }
    }

    if (m_iQuantity == 2)
    {
        m_iIntersectionType = IT_SEGMENT;
    }
    else if (m_iQuantity == 1)
    {
        m_iIntersectionType = IT_POINT;
    }
    else
    {
        m_iIntersectionType = IT_EMPTY;
    }

    return m_iIntersectionType != IT_EMPTY;
}
//----------------------------------------------------------------------------
template <class Real>
int IntrRay3Cylinder3<Real>::GetQuantity () const
{
    return m_iQuantity;
}
//----------------------------------------------------------------------------
template <class Real>
const Vector3<Real>& IntrRay3Cylinder3<Real>::GetPoint (int i) const
{
    assert(0 <= i && i < m_iQuantity);
    return m_akPoint[i];
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template WM4_FOUNDATION_ITEM
class IntrRay3Cylinder3<float>;

template WM4_FOUNDATION_ITEM
class IntrRay3Cylinder3<double>;
//----------------------------------------------------------------------------
}
