// Geometric Tools, Inc.
// http://www.geometrictools.com
// Copyright (c) 1998-2006.  All Rights Reserved
//
// The Wild Magic Version 4 Foundation Library source code is supplied
// under the terms of the license agreement
//     http://www.geometrictools.com/License/Wm4FoundationLicense.pdf
// and may not be copied or disclosed except in accordance with the terms
// of that agreement.

#ifndef WM4INTPTRILINEAR3_H
#define WM4INTPTRILINEAR3_H

#include "Wm4FoundationLIB.h"
#include "Wm4System.h"

namespace Wm4
{

template <class Real>
class WM4_FOUNDATION_ITEM IntpTrilinear3
{
public:
    // Construction and destruction.  IntpTrilinear3 does not accept
    // responsibility for deleting the input array.  The application must do
    // so.  The interpolator is for uniformly spaced (x,y,z)-values.  The
    // function values are assumed to be organized as f(x,y,z) = F[z][y][x].

    IntpTrilinear3 (int iXBound, int iYBound, int iZBound, Real fXMin,
        Real fXSpacing, Real fYMin, Real fYSpacing, Real fZMin,
        Real fZSpacing, Real*** aaafF);

    int GetXBound () const;
    int GetYBound () const;
    int GetZBound () const;
    int GetQuantity () const;
    Real*** GetF () const;

    Real GetXMin () const;
    Real GetXMax () const;
    Real GetXSpacing () const;
    Real GetYMin () const;
    Real GetYMax () const;
    Real GetYSpacing () const;
    Real GetZMin () const;
    Real GetZMax () const;
    Real GetZSpacing () const;

    // Evaluate the function and its derivatives.  The application is
    // responsible for ensuring that xmin <= x <= xmax, ymin <= y <= ymax,
    // and zmin <= z <= zmax.  If (x,y,z) is outside the extremes, the
    // function returns MAXREAL.  The first operator is for function
    // evaluation.  The second operator is for function or derivative
    // evaluations.  The uiXOrder argument is the order of the x-derivative,
    // the uiYOrder argument is the order of the y-derivative, and the
    // uiZOrder argument is the order of the z-derivative.  All orders are
    // zero to get the function value itself.
    Real operator() (Real fX, Real fY, Real fZ) const;
    Real operator() (int iXOrder, int iYOrder, int iZOrder, Real fX,
        Real fY, Real fZ) const;

private:
    int m_iXBound, m_iYBound, m_iZBound, m_iQuantity;
    Real m_fXMin, m_fXMax, m_fXSpacing, m_fInvXSpacing;
    Real m_fYMin, m_fYMax, m_fYSpacing, m_fInvYSpacing;
    Real m_fZMin, m_fZMax, m_fZSpacing, m_fInvZSpacing;
    Real*** m_aaafF;

    static const Real ms_aafBlend[2][2];
};

typedef IntpTrilinear3<float> IntpTrilinear3f;
typedef IntpTrilinear3<double> IntpTrilinear3d;

}

#endif
