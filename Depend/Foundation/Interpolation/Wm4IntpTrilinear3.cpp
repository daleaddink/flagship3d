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
#include "Wm4IntpTrilinear3.h"
#include "Wm4Math.h"

namespace Wm4
{
//----------------------------------------------------------------------------
template <class Real>
IntpTrilinear3<Real>::IntpTrilinear3 (int iXBound, int iYBound, int iZBound,
    Real fXMin, Real fXSpacing, Real fYMin, Real fYSpacing, Real fZMin,
    Real fZSpacing, Real*** aaafF)
{
    // At least a 2x2x2 block of data points are needed to construct the
    // trilinear interpolation.
    assert(iXBound >= 2 && iYBound >= 2 && iZBound >= 2 && aaafF);
    assert(fXSpacing > (Real)0.0 && fYSpacing > (Real)0.0
        && fZSpacing > (Real)0.0);

    m_iXBound = iXBound;
    m_iYBound = iYBound;
    m_iZBound = iZBound;
    m_iQuantity = iXBound*iYBound*iZBound;

    m_fXMin = fXMin;
    m_fXSpacing = fXSpacing;
    m_fInvXSpacing = ((Real)1.0)/fXSpacing;
    m_fXMax = fXMin + fXSpacing*(iXBound-1);

    m_fYMin = fYMin;
    m_fYSpacing = fYSpacing;
    m_fInvYSpacing = ((Real)1.0)/fYSpacing;
    m_fYMax = fYMin + fYSpacing*(iYBound-1);

    m_fZMin = fZMin;
    m_fZSpacing = fZSpacing;
    m_fInvZSpacing = ((Real)1.0)/fZSpacing;
    m_fZMax = fYMin + fZSpacing*(iZBound-1);

    m_aaafF = aaafF;
}
//----------------------------------------------------------------------------
template <class Real>
int IntpTrilinear3<Real>::GetXBound () const
{
    return m_iXBound;
}
//----------------------------------------------------------------------------
template <class Real>
int IntpTrilinear3<Real>::GetYBound () const
{
    return m_iYBound;
}
//----------------------------------------------------------------------------
template <class Real>
int IntpTrilinear3<Real>::GetZBound () const
{
    return m_iZBound;
}
//----------------------------------------------------------------------------
template <class Real>
int IntpTrilinear3<Real>::GetQuantity () const
{
    return m_iQuantity;
}
//----------------------------------------------------------------------------
template <class Real>
Real*** IntpTrilinear3<Real>::GetF () const
{
    return m_aaafF;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpTrilinear3<Real>::GetXMin () const
{
    return m_fXMin;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpTrilinear3<Real>::GetXMax () const
{
    return m_fXMax;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpTrilinear3<Real>::GetXSpacing () const
{
    return m_fXSpacing;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpTrilinear3<Real>::GetYMin () const
{
    return m_fYMin;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpTrilinear3<Real>::GetYMax () const
{
    return m_fYMax;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpTrilinear3<Real>::GetYSpacing () const
{
    return m_fYSpacing;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpTrilinear3<Real>::GetZMin () const
{
    return m_fZMin;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpTrilinear3<Real>::GetZMax () const
{
    return m_fZMax;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpTrilinear3<Real>::GetZSpacing () const
{
    return m_fZSpacing;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpTrilinear3<Real>::operator() (Real fX, Real fY, Real fZ) const
{
    // check for inputs not in the domain of the function
    if (fX < m_fXMin || fX > m_fXMax
    ||  fY < m_fYMin || fY > m_fYMax
    ||  fZ < m_fZMin || fY > m_fZMax)
    {
        return Math<Real>::MAX_REAL;
    }

    // compute x-index and clamp to image
    Real fXIndex = (fX - m_fXMin)*m_fInvXSpacing;
    int iX = (int)fXIndex;
    if (iX < 0)
    {
        iX = 0;
    }
    else if (iX >= m_iXBound)
    {
        iX = m_iXBound-1;
    }

    // compute y-index and clamp to image
    Real fYIndex = (fY - m_fYMin)*m_fInvYSpacing;
    int iY = (int)fYIndex;
    if (iY < 0)
    {
        iY = 0;
    }
    else if (iY >= m_iYBound)
    {
        iY = m_iYBound-1;
    }

    // compute z-index and clamp to image
    Real fZIndex = (fZ - m_fZMin)*m_fInvZSpacing;
    int iZ = (int)fZIndex;
    if (iZ < 0)
    {
        iZ = 0;
    }
    else if (iZ >= m_iZBound)
    {
        iZ = m_iZBound-1;
    }

    Real afU[2];
    afU[0] = (Real)1.0;
    afU[1] = fXIndex - iX;

    Real afV[2];
    afV[0] = (Real)1.0;
    afV[1] = fYIndex - iY;

    Real afW[2];
    afW[0] = (Real)1.0;
    afW[1] = fZIndex - iZ;

    // compute P = M*U, Q = M*V, R = M*W
    Real afP[2], afQ[2], afR[2];
    int iRow, iCol;
    for (iRow = 0; iRow < 2; iRow++)
    {
        afP[iRow] = (Real)0.0;
        afQ[iRow] = (Real)0.0;
        afR[iRow] = (Real)0.0;
        for (iCol = 0; iCol < 2; iCol++)
        {
            afP[iRow] += ms_aafBlend[iRow][iCol]*afU[iCol];
            afQ[iRow] += ms_aafBlend[iRow][iCol]*afV[iCol];
            afR[iRow] += ms_aafBlend[iRow][iCol]*afW[iCol];
        }
    }

    // compute the tensor product (M*U)(M*V)(M*W)*D where D is the 2x2x2
    // subimage containing (x,y,z)
    Real fResult = (Real)0.0;
    for (int iSlice = 0; iSlice < 2; iSlice++)
    {
        int iZClamp = iZ + iSlice;
        if (iZClamp >= m_iZBound)
        {
            iZClamp = m_iZBound - 1;
        }

        for (iRow = 0; iRow < 2; iRow++)
        {
            int iYClamp = iY + iRow;
            if (iYClamp >= m_iYBound)
            {
                iYClamp = m_iYBound - 1;
            }

            for (iCol = 0; iCol < 2; iCol++)
            {
                int iXClamp = iX + iCol;
                if (iXClamp >= m_iXBound)
                {
                    iXClamp = m_iXBound - 1;
                }

                fResult += afP[iCol]*afQ[iRow]*afR[iSlice]*
                    m_aaafF[iZClamp][iYClamp][iXClamp];
            }
        }
    }

    return fResult;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpTrilinear3<Real>::operator() (int iXOrder, int iYOrder, int iZOrder,
    Real fX, Real fY, Real fZ) const
{
    // check for inputs not in the domain of the function
    if (fX < m_fXMin || fX > m_fXMax
    ||  fY < m_fYMin || fY > m_fYMax
    ||  fZ < m_fZMin || fY > m_fZMax)
    {
        return Math<Real>::MAX_REAL;
    }

    // compute x-index and clamp to image
    Real fXIndex = (fX - m_fXMin)*m_fInvXSpacing;
    int iX = (int)fXIndex;
    if (iX < 0)
    {
        iX = 0;
    }
    else if (iX >= m_iXBound)
    {
        iX = m_iXBound-1;
    }

    // compute y-index and clamp to image
    Real fYIndex = (fY - m_fYMin)*m_fInvYSpacing;
    int iY = (int)fYIndex;
    if (iY < 0)
    {
        iY = 0;
    }
    else if (iY >= m_iYBound)
    {
        iY = m_iYBound-1;
    }

    // compute z-index and clamp to image
    Real fZIndex = (fZ - m_fZMin)*m_fInvZSpacing;
    int iZ = (int)fZIndex;
    if (iZ < 0)
    {
        iZ = 0;
    }
    else if (iZ >= m_iZBound)
    {
        iZ = m_iZBound-1;
    }

    Real afU[2], fDX, fXMult;
    switch (iXOrder)
    {
    case 0:
        fDX = fXIndex - iX;
        afU[0] = (Real)1.0;
        afU[1] = fDX;
        fXMult = (Real)1.0;
        break;
    case 1:
        fDX = fXIndex - iX;
        afU[0] = (Real)0.0;
        afU[1] = (Real)1.0;
        fXMult = m_fInvXSpacing;
        break;
    default:
        return (Real)0.0;
    }

    Real afV[2], fDY, fYMult;
    switch (iYOrder)
    {
    case 0:
        fDY = fYIndex - iY;
        afV[0] = (Real)1.0;
        afV[1] = fDY;
        fYMult = (Real)1.0;
        break;
    case 1:
        fDY = fYIndex - iY;
        afV[0] = (Real)0.0;
        afV[1] = (Real)1.0;
        fYMult = m_fInvYSpacing;
        break;
    default:
        return (Real)0.0;
    }

    Real afW[2], fDZ, fZMult;
    switch (iZOrder)
    {
    case 0:
        fDZ = fZIndex - iZ;
        afW[0] = (Real)1.0;
        afW[1] = fDZ;
        fZMult = (Real)1.0;
        break;
    case 1:
        fDZ = fZIndex - iZ;
        afW[0] = (Real)0.0;
        afW[1] = (Real)1.0;
        fZMult = m_fInvZSpacing;
        break;
    default:
        return (Real)0.0;
    }

    // compute P = M*U, Q = M*V, and R = M*W
    Real afP[2], afQ[2], afR[2];
    int iRow, iCol;
    for (iRow = 0; iRow < 2; iRow++)
    {
        afP[iRow] = (Real)0.0;
        afQ[iRow] = (Real)0.0;
        afR[iRow] = (Real)0.0;
        for (iCol = 0; iCol < 2; iCol++)
        {
            afP[iRow] += ms_aafBlend[iRow][iCol]*afU[iCol];
            afQ[iRow] += ms_aafBlend[iRow][iCol]*afV[iCol];
            afR[iRow] += ms_aafBlend[iRow][iCol]*afW[iCol];
        }
    }

    // compute the tensor product (M*U)(M*V)(M*W)*D where D is the 2x2x2
    // subimage containing (x,y,z)
    Real fResult = (Real)0.0;
    for (int iSlice = 0; iSlice < 2; iSlice++)
    {
        int iZClamp = iZ + iSlice;
        if (iZClamp >= m_iZBound)
        {
            iZClamp = m_iZBound - 1;
        }

        for (iRow = 0; iRow < 2; iRow++)
        {
            int iYClamp = iY + iRow;
            if (iYClamp >= m_iYBound)
            {
                iYClamp = m_iYBound - 1;
            }

            for (iCol = 0; iCol < 2; iCol++)
            {
                int iXClamp = iX + iCol;
                if (iXClamp >= m_iXBound)
                {
                    iXClamp = m_iXBound - 1;
                }

                fResult += afP[iCol]*afQ[iRow]*afR[iSlice]*
                    m_aaafF[iZClamp][iYClamp][iXClamp];
            }
        }
    }
    fResult *= fXMult*fYMult*fZMult;

    return fResult;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template WM4_FOUNDATION_ITEM
class IntpTrilinear3<float>;

template<>
const float IntpTrilinear3<float>::ms_aafBlend[2][2] =
{
    { 1.0f, -1.0f },
    { 0.0f,  1.0f }
};

template WM4_FOUNDATION_ITEM
class IntpTrilinear3<double>;

template<>
const double IntpTrilinear3<double>::ms_aafBlend[2][2] =
{
    { 1.0, -1.0 },
    { 0.0,  1.0 }
};
//----------------------------------------------------------------------------
}
