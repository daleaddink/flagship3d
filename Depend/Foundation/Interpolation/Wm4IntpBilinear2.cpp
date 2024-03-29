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
#include "Wm4IntpBilinear2.h"
#include "Wm4Math.h"

namespace Wm4
{
//----------------------------------------------------------------------------
template <class Real>
IntpBilinear2<Real>::IntpBilinear2 (int iXBound, int iYBound, Real fXMin,
    Real fXSpacing, Real fYMin, Real fYSpacing, Real** aafF)
{
    // At least a 2x2 block of data points are needed.
    assert(iXBound >= 2 && iYBound >= 2 && aafF);
    assert(fXSpacing > (Real)0.0 && fYSpacing > (Real)0.0);

    m_iXBound = iXBound;
    m_iYBound = iYBound;
    m_iQuantity = iXBound*iYBound;

    m_fXMin = fXMin;
    m_fXSpacing = fXSpacing;
    m_fInvXSpacing = ((Real)1.0)/fXSpacing;
    m_fYMin = fYMin;
    m_fYSpacing = fYSpacing;
    m_fInvYSpacing = ((Real)1.0)/fYSpacing;
    m_fXMax = fXMin + fXSpacing*(iXBound-1);
    m_fYMax = fYMin + fYSpacing*(iYBound-1);

    m_aafF = aafF;
}
//----------------------------------------------------------------------------
template <class Real>
int IntpBilinear2<Real>::GetXBound () const
{
    return m_iXBound;
}
//----------------------------------------------------------------------------
template <class Real>
int IntpBilinear2<Real>::GetYBound () const
{
    return m_iYBound;
}
//----------------------------------------------------------------------------
template <class Real>
int IntpBilinear2<Real>::GetQuantity () const
{
    return m_iQuantity;
}
//----------------------------------------------------------------------------
template <class Real>
Real** IntpBilinear2<Real>::GetF () const
{
    return m_aafF;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpBilinear2<Real>::GetXMin () const
{
    return m_fXMin;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpBilinear2<Real>::GetXMax () const
{
    return m_fXMax;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpBilinear2<Real>::GetXSpacing () const
{
    return m_fXSpacing;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpBilinear2<Real>::GetYMin () const
{
    return m_fYMin;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpBilinear2<Real>::GetYMax () const
{
    return m_fYMax;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpBilinear2<Real>::GetYSpacing () const
{
    return m_fYSpacing;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpBilinear2<Real>::operator() (Real fX, Real fY) const
{
    // check for inputs not in the domain of the function
    if (fX < m_fXMin || fX > m_fXMax || fY < m_fYMin || fY > m_fYMax)
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

    Real afU[2];
    afU[0] = (Real)1.0;
    afU[1] = fXIndex - iX;

    Real afV[2];
    afV[0] = (Real)1.0;
    afV[1] = fYIndex - iY;

    // compute P = M*U and Q = M*V
    Real afP[2], afQ[2];
    int iRow, iCol;
    for (iRow = 0; iRow < 2; iRow++)
    {
        afP[iRow] = (Real)0.0;
        afQ[iRow] = (Real)0.0;
        for (iCol = 0; iCol < 2; iCol++)
        {
            afP[iRow] += ms_aafBlend[iRow][iCol]*afU[iCol];
            afQ[iRow] += ms_aafBlend[iRow][iCol]*afV[iCol];
        }
    }

    // compute (M*U)^t D (M*V) where D is the 2x2 subimage containing (x,y)
    Real fResult = (Real)0.0;
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

            fResult += afQ[iRow]*m_aafF[iYClamp][iXClamp]*afP[iCol];
        }
    }

    return fResult;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpBilinear2<Real>::operator() (int iXOrder, int iYOrder, Real fX,
    Real fY) const
{
    // check for inputs not in the domain of the function
    if (fX < m_fXMin || fX > m_fXMax || fY < m_fYMin || fY > m_fYMax)
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

    // compute P = M*U and Q = M*V
    Real afP[2], afQ[2];
    int iRow, iCol;
    for (iRow = 0; iRow < 2; iRow++)
    {
        afP[iRow] = (Real)0.0;
        afQ[iRow] = (Real)0.0;
        for (iCol = 0; iCol < 2; iCol++)
        {
            afP[iRow] += ms_aafBlend[iRow][iCol]*afU[iCol];
            afQ[iRow] += ms_aafBlend[iRow][iCol]*afV[iCol];
        }
    }

    // compute (M*U)^t D (M*V) where D is the 2x2 subimage containing (x,y)
    Real fResult = (Real)0.0;
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

            fResult += afQ[iRow]*m_aafF[iYClamp][iXClamp]*afP[iCol];
        }
    }
    fResult *= fXMult*fYMult;

    return fResult;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template WM4_FOUNDATION_ITEM
class IntpBilinear2<float>;

template<>
const float IntpBilinear2<float>::ms_aafBlend[2][2] =
{
    { 1.0f, -1.0f },
    { 0.0f,  1.0f }
};

template WM4_FOUNDATION_ITEM
class IntpBilinear2<double>;

template<>
const double IntpBilinear2<double>::ms_aafBlend[2][2] =
{
    { 1.0, -1.0 },
    { 0.0,  1.0 }
};
//----------------------------------------------------------------------------
}
