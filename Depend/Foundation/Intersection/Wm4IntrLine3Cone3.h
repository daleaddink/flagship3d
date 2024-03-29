// Geometric Tools, Inc.
// http://www.geometrictools.com
// Copyright (c) 1998-2006.  All Rights Reserved
//
// The Wild Magic Version 4 Foundation Library source code is supplied
// under the terms of the license agreement
//     http://www.geometrictools.com/License/Wm4FoundationLicense.pdf
// and may not be copied or disclosed except in accordance with the terms
// of that agreement.

#ifndef WM4INTRLINE3CONE3_H
#define WM4INTRLINE3CONE3_H

#include "Wm4FoundationLIB.h"
#include "Wm4Intersector.h"
#include "Wm4Line3.h"
#include "Wm4Cone3.h"

namespace Wm4
{

template <class Real>
class WM4_FOUNDATION_ITEM IntrLine3Cone3
    : public Intersector<Real,Vector3<Real> >
{
public:
    IntrLine3Cone3 (const Line3<Real>& rkLine, const Cone3<Real>& rkCone);

    // object access
    const Line3<Real>& GetLine () const;
    const Cone3<Real>& GetCone () const;

    // static intersection query
    virtual bool Find ();

    // The intersection set.  The possible intersection types are
    //   IT_EMTPY:   no intersection
    //   IT_POINT:   point[0] is the intersection point
    //   IT_SEGMENT: <point[0],point[1]> is the intersection segment
    //   IT_RAY:     point[0]+t*point[1] is the intersection ray
    int GetQuantity () const;
    const Vector3<Real>& GetPoint (int i) const;

private:
    using Intersector<Real,Vector3<Real> >::IT_EMPTY;
    using Intersector<Real,Vector3<Real> >::IT_POINT;
    using Intersector<Real,Vector3<Real> >::IT_SEGMENT;
    using Intersector<Real,Vector3<Real> >::IT_RAY;
    using Intersector<Real,Vector3<Real> >::m_iIntersectionType;

    // the objects to intersect
    const Line3<Real>& m_rkLine;
    const Cone3<Real>& m_rkCone;

    // information about the intersection set
    int m_iQuantity;
    Vector3<Real> m_akPoint[2];
};

typedef IntrLine3Cone3<float> IntrLine3Cone3f;
typedef IntrLine3Cone3<double> IntrLine3Cone3d;

}

#endif
