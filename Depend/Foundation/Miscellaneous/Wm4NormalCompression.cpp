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
#include "Wm4NormalCompression.h"
#include "Wm4Math.h"
using namespace Wm4;

static int gs_iN = 127;  // N(N+1)/2 = 8128 < 2^{13}
static double gs_dB = 2*gs_iN+1;
static double gs_dB2 = gs_dB*gs_dB;
static double gs_dFactor = (gs_iN-1)*Mathd::Sqrt(0.5);
static double gs_dInvFactor = 1.0/gs_dFactor;

//----------------------------------------------------------------------------
void Wm4::CompressNormal (double dX, double dY, double dZ,
    unsigned short& rusIndex)
{
    // assert:  x*x + y*y + z*z = 1

    // determine octant
    rusIndex = 0;
    if (dX < 0.0)
    {
        rusIndex |= 0x8000;
        dX = -dX;
    }
    if (dY < 0.0)
    {
        rusIndex |= 0x4000;
        dY = -dY;
    }
    if (dZ < 0.0)
    {
        rusIndex |= 0x2000;
        dZ = -dZ;
    }

    // determine mantissa
    unsigned short usX = (unsigned short) Mathd::Floor(gs_dFactor*dX);
    unsigned short usY = (unsigned short) Mathd::Floor(gs_dFactor*dY);
    unsigned short usMantissa = usX + ((usY*(255-usY)) >> 1);
    rusIndex |= usMantissa;
}
//----------------------------------------------------------------------------
void Wm4::UncompressNormal (unsigned short usIndex, double& rdX, double& rdY,
    double& rdZ)
{
    unsigned short usMantissa = usIndex & 0x1FFF;

    // extract triangular indices
    double dTemp = gs_dB2 - 8*usMantissa;
    unsigned short usY = (unsigned short) Mathd::Floor(0.5*(gs_dB -
        Mathd::Sqrt(fabs(dTemp))));
    unsigned short usX = usMantissa - ((usY*(255-usY)) >> 1);

    // build approximate normal
    rdX = usX*gs_dInvFactor;
    rdY = usY*gs_dInvFactor;
    dTemp = 1.0 - rdX*rdX - rdY*rdY;
    rdZ = Mathd::Sqrt(Mathd::FAbs(dTemp));

    // determine octant
    if (usIndex & 0x8000)
    {
        rdX = -rdX;
    }
    if (usIndex & 0x4000)
    {
        rdY = -rdY;
    }
    if (usIndex & 0x2000)
    {
        rdZ = -rdZ;
    }
}
//----------------------------------------------------------------------------
