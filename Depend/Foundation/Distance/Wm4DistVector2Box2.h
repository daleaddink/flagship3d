// Geometric Tools, Inc.
// http://www.geometrictools.com
// Copyright (c) 1998-2006.  All Rights Reserved
//
// The Wild Magic Version 4 Foundation Library source code is supplied
// under the terms of the license agreement
//     http://www.geometrictools.com/License/Wm4FoundationLicense.pdf
// and may not be copied or disclosed except in accordance with the terms
// of that agreement.

#ifndef WM4DISTVECTOR2BOX2_H
#define WM4DISTVECTOR2BOX2_H

#include "Wm4FoundationLIB.h"
#include "Wm4Distance.h"
#include "Wm4Box2.h"

namespace Wm4
{

template <class Real>
class WM4_FOUNDATION_ITEM DistVector2Box2
    : public Distance<Real,Vector2<Real> >
{
public:
    DistVector2Box2 (const Vector2<Real>& rkVector, const Box2<Real>& rkBox);

    // object access
    const Vector2<Real>& GetVector () const;
    const Box2<Real>& GetBox () const;

    // static distance queries
    virtual Real Get ();
    virtual Real GetSquared ();

    // function calculations for dynamic distance queries
    virtual Real Get (Real fT, const Vector2<Real>& rkVelocity0,
        const Vector2<Real>& rkVelocity1);
    virtual Real GetSquared (Real fT, const Vector2<Real>& rkVelocity0,
        const Vector2<Real>& rkVelocity1);

private:
    using Distance<Real,Vector2<Real> >::m_kClosestPoint0;
    using Distance<Real,Vector2<Real> >::m_kClosestPoint1;

    const Vector2<Real>& m_rkVector;
    const Box2<Real>& m_rkBox;
};

typedef DistVector2Box2<float> DistVector2Box2f;
typedef DistVector2Box2<double> DistVector2Box2d;

}

#endif
