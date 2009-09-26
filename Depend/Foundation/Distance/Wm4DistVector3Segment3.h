// Geometric Tools, Inc.
// http://www.geometrictools.com
// Copyright (c) 1998-2006.  All Rights Reserved
//
// The Wild Magic Version 4 Foundation Library source code is supplied
// under the terms of the license agreement
//     http://www.geometrictools.com/License/Wm4FoundationLicense.pdf
// and may not be copied or disclosed except in accordance with the terms
// of that agreement.

#ifndef WM4DISTVECTOR3SEGMENT3_H
#define WM4DISTVECTOR3SEGMENT3_H

#include "Wm4FoundationLIB.h"
#include "Wm4Distance.h"
#include "Wm4Segment3.h"

namespace Wm4
{

template <class Real>
class WM4_FOUNDATION_ITEM DistVector3Segment3
    : public Distance<Real,Vector3<Real> >
{
public:
    DistVector3Segment3 (const Vector3<Real>& rkVector,
        const Segment3<Real>& rkSegment);

    // object access
    const Vector3<Real>& GetVector () const;
    const Segment3<Real>& GetSegment () const;

    // static distance queries
    virtual Real Get ();
    virtual Real GetSquared ();

    // function calculations for dynamic distance queries
    virtual Real Get (Real fT, const Vector3<Real>& rkVelocity0,
        const Vector3<Real>& rkVelocity1);
    virtual Real GetSquared (Real fT, const Vector3<Real>& rkVelocity0,
        const Vector3<Real>& rkVelocity1);

    // Information about the closest line point.
    Real GetSegmentParameter () const;

private:
    using Distance<Real,Vector3<Real> >::m_kClosestPoint0;
    using Distance<Real,Vector3<Real> >::m_kClosestPoint1;

    const Vector3<Real>& m_rkVector;
    const Segment3<Real>& m_rkSegment;

    // Information about the closest segment point.
    Real m_fSegmentParameter;  // closest1 = seg.origin+param*seg.direction
};

typedef DistVector3Segment3<float> DistVector3Segment3f;
typedef DistVector3Segment3<double> DistVector3Segment3d;

}

#endif
