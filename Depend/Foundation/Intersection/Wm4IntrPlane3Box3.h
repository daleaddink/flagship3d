// Geometric Tools, Inc.
// http://www.geometrictools.com
// Copyright (c) 1998-2006.  All Rights Reserved
//
// The Wild Magic Version 4 Foundation Library source code is supplied
// under the terms of the license agreement
//     http://www.geometrictools.com/License/Wm4FoundationLicense.pdf
// and may not be copied or disclosed except in accordance with the terms
// of that agreement.

#ifndef WM4INTRPLANE3BOX3_H
#define WM4INTRPLANE3BOX3_H

#include "Wm4FoundationLIB.h"
#include "Wm4Intersector.h"
#include "Wm4Plane3.h"
#include "Wm4Box3.h"

namespace Wm4
{

template <class Real>
class WM4_FOUNDATION_ITEM IntrPlane3Box3
    : public Intersector<Real,Vector3<Real> >
{
public:
    IntrPlane3Box3 (const Plane3<Real>& rkPlane,
        const Box3<Real>& rkBox);

    // object access
    const Plane3<Real>& GetPlane () const;
    const Box3<Real>& GetBox () const;

    // static intersection query
    virtual bool Test ();

    // Culling support.  The view frustum is assumed to be on the positive
    // side of the plane.  The box is culled if it is on the negative side
    // of the plane.
    bool BoxIsCulled () const;

protected:
    // the objects to intersect
    const Plane3<Real>& m_rkPlane;
    const Box3<Real>& m_rkBox;
};

typedef IntrPlane3Box3<float> IntrPlane3Box3f;
typedef IntrPlane3Box3<double> IntrPlane3Box3d;

}

#endif
