// Geometric Tools, Inc.
// http://www.geometrictools.com
// Copyright (c) 1998-2006.  All Rights Reserved
//
// The Wild Magic Version 4 Foundation Library source code is supplied
// under the terms of the license agreement
//     http://www.geometrictools.com/License/Wm4FoundationLicense.pdf
// and may not be copied or disclosed except in accordance with the terms
// of that agreement.

#ifndef WM4DISTVECTOR3RECTANGLE3_H
#define WM4DISTVECTOR3RECTANGLE3_H

#include "Wm4FoundationLIB.h"
#include "Wm4Distance.h"
#include "Wm4Rectangle3.h"

namespace Wm4
{

template <class Real>
class WM4_FOUNDATION_ITEM DistVector3Rectangle3
    : public Distance<Real,Vector3<Real> >
{
public:
    DistVector3Rectangle3 (const Vector3<Real>& rkVector,
        const Rectangle3<Real>& rkRectangle);

    // object access
    const Vector3<Real>& GetVector () const;
    const Rectangle3<Real>& GetRectangle () const;

    // static distance queries
    virtual Real Get ();
    virtual Real GetSquared ();

    // function calculations for dynamic distance queries
    virtual Real Get (Real fT, const Vector3<Real>& rkVelocity0,
        const Vector3<Real>& rkVelocity1);
    virtual Real GetSquared (Real fT, const Vector3<Real>& rkVelocity0,
        const Vector3<Real>& rkVelocity1);

    // Information about the closest rectangle point.
    Real GetRectangleCoordinate (int i) const;

private:
    using Distance<Real,Vector3<Real> >::m_kClosestPoint0;
    using Distance<Real,Vector3<Real> >::m_kClosestPoint1;

    const Vector3<Real>& m_rkVector;
    const Rectangle3<Real>& m_rkRectangle;

    // Information about the closest rectangle point.
    Real m_afRectCoord[2];  // closest1 = rect.center+param0*rect.dir0+
                            //            param1*rect.dir1
};

typedef DistVector3Rectangle3<float> DistVector3Rectangle3f;
typedef DistVector3Rectangle3<double> DistVector3Rectangle3d;

}

#endif
