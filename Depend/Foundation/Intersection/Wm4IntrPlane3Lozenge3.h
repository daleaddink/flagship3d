// Geometric Tools, Inc.
// http://www.geometrictools.com
// Copyright (c) 1998-2006.  All Rights Reserved
//
// The Wild Magic Version 4 Foundation Library source code is supplied
// under the terms of the license agreement
//     http://www.geometrictools.com/License/Wm4FoundationLicense.pdf
// and may not be copied or disclosed except in accordance with the terms
// of that agreement.

#ifndef WM4INTRPLANE3LOZENGE3_H
#define WM4INTRPLANE3LOZENGE3_H

#include "Wm4FoundationLIB.h"
#include "Wm4Intersector.h"
#include "Wm4Plane3.h"
#include "Wm4Lozenge3.h"

namespace Wm4
{

template <class Real>
class WM4_FOUNDATION_ITEM IntrPlane3Lozenge3
    : public Intersector<Real,Vector3<Real> >
{
public:
    IntrPlane3Lozenge3 (const Plane3<Real>& rkPlane,
        const Lozenge3<Real>& rkLozenge);

    // object access
    const Plane3<Real>& GetPlane () const;
    const Lozenge3<Real>& GetLozenge () const;

    // static intersection query
    virtual bool Test ();

    // Culling support.  The view frustum is assumed to be on the positive
    // side of the plane.  The lozenge is culled if it is on the negative
    // side of the plane.
    bool LozengeIsCulled () const;

protected:
    // the objects to intersect
    const Plane3<Real>& m_rkPlane;
    const Lozenge3<Real>& m_rkLozenge;
};

typedef IntrPlane3Lozenge3<float> IntrPlane3Lozenge3f;
typedef IntrPlane3Lozenge3<double> IntrPlane3Lozenge3d;

}

#endif
