// Geometric Tools, Inc.
// http://www.geometrictools.com
// Copyright (c) 1998-2006.  All Rights Reserved
//
// The Wild Magic Version 4 Foundation Library source code is supplied
// under the terms of the license agreement
//     http://www.geometrictools.com/License/Wm4FoundationLicense.pdf
// and may not be copied or disclosed except in accordance with the terms
// of that agreement.

#ifndef WM4CIRCLE3_H
#define WM4CIRCLE3_H

#include "Wm4FoundationLIB.h"
#include "Wm4Vector3.h"

namespace Wm4
{

template <class Real>
class Circle3
{
public:
    // Plane containing circle is Dot(N,X-C) = 0 where X is any point in the
    // plane.  Vectors U, V, and N form an orthonormal right-handed set
    // (matrix [U V N] is orthonormal and has determinant 1).  Circle within
    // the plane is parameterized by X = C + R*(cos(A)*U + sin(A)*V) where
    // A is an angle in [0,2*pi).

    // construction
    Circle3 ();  // uninitialized
    Circle3 (const Vector3<Real>& rkCenter, const Vector3<Real>& rkU,
        const Vector3<Real>& rkV, const Vector3<Real>& rkN, Real fRadius);

    Vector3<Real> Center, U, V, N;
    Real Radius;
};

#include "Wm4Circle3.inl"

typedef Circle3<float> Circle3f;
typedef Circle3<double> Circle3d;

}

#endif
