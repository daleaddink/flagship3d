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
#include "Wm4IntpBSplineUniform3.h"
#include "Wm4Math.h"

namespace Wm4
{
//----------------------------------------------------------------------------
template <class Real>
IntpBSplineUniform3<Real>::IntpBSplineUniform3 (int iDegree, const int* aiDim,
    Real* afData)
    :
    IntpBSplineUniform<Real>(3,iDegree,aiDim,afData)
{
}
//----------------------------------------------------------------------------
template <class Real>
int IntpBSplineUniform3<Real>::Index (int iX, int iY, int iZ) const
{
    return iX + m_aiDim[0]*(iY + m_aiDim[1]*iZ);
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpBSplineUniform3<Real>::operator() (Real* afX)
{
    return (*this)(afX[0],afX[1],afX[2]);
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpBSplineUniform3<Real>::operator() (int* aiDx, Real* afX)
{
    return (*this)(aiDx[0],aiDx[1],aiDx[2],afX[0],afX[1],afX[2]);
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpBSplineUniform3<Real>::operator() (Real fX, Real fY, Real fZ)
{
    m_aiBase[0] = (int)Math<Real>::Floor(fX);
    m_aiBase[1] = (int)Math<Real>::Floor(fY);
    m_aiBase[2] = (int)Math<Real>::Floor(fZ);
    for (int iDim = 0; iDim < 3; iDim++)
    {
        if (m_aiOldBase[iDim] != m_aiBase[iDim])
        {
            // switch to new local grid
            for (int k = 0; k < 3; k++)
            {
                m_aiOldBase[k] = m_aiBase[k];
                m_aiGridMin[k] = m_aiBase[k] - 1;
                m_aiGridMax[k] = m_aiGridMin[k] + m_iDegree;
            }

            // fill in missing grid data if necessary
            if (m_oEvaluateCallback)
            {
                EvaluateUnknownData();
            }

            ComputeIntermediate();
            break;
        }
    }

    SetPolynomial(0,fX-m_aiBase[0],m_aafPoly[0]);
    SetPolynomial(0,fY-m_aiBase[1],m_aafPoly[1]);
    SetPolynomial(0,fZ-m_aiBase[2],m_aafPoly[2]);

    int aiI[3] = { 0, 0, 0 };
    Real fResult = (Real)0.0;
    for (int k = 0; k < m_iDp1ToN; k++)
    {
        fResult += m_aafPoly[0][aiI[0]]*m_aafPoly[1][aiI[1]]*
            m_aafPoly[2][aiI[2]]*m_afInter[k];

        if (++aiI[0] <= m_iDegree)
        {
            continue;
        }
        aiI[0] = 0;

        if (++aiI[1] <= m_iDegree)
        {
            continue;
        }
        aiI[1] = 0;

        aiI[2]++;
    }
    return fResult;
}
//----------------------------------------------------------------------------
template <class Real>
Real IntpBSplineUniform3<Real>::operator() (int iDx, int iDy, int iDz,
    Real fX, Real fY, Real fZ)
{
    m_aiBase[0] = (int)Math<Real>::Floor(fX);
    m_aiBase[1] = (int)Math<Real>::Floor(fY);
    m_aiBase[2] = (int)Math<Real>::Floor(fZ);
    for (int d = 0; d < 3; d++)
    {
        if (m_aiOldBase[d] != m_aiBase[d])
        {
            // switch to new local grid
            for (int k = 0; k < 3; k++)
            {
                m_aiOldBase[k] = m_aiBase[k];
                m_aiGridMin[k] = m_aiBase[k] - 1;
                m_aiGridMax[k] = m_aiGridMin[k] + m_iDegree;
            }

            // fill in missing grid data if necessary
            if (m_oEvaluateCallback)
            {
                EvaluateUnknownData();
            }

            ComputeIntermediate();
            break;
        }
    }

    SetPolynomial(iDx,fX-m_aiBase[0],m_aafPoly[0]);
    SetPolynomial(iDy,fY-m_aiBase[1],m_aafPoly[1]);
    SetPolynomial(iDz,fZ-m_aiBase[2],m_aafPoly[2]);

    int aiI[3] = { iDx, iDy, iDz };
    int aiDelta[2] = { iDx, m_iDp1*iDy };
    Real fResult = (Real)0.0;
    for (int k = aiI[0]+m_iDp1*(aiI[1]+m_iDp1*aiI[2]); k < m_iDp1ToN; k++)
    {
        fResult +=  m_aafPoly[0][aiI[0]]*m_aafPoly[1][aiI[1]]*
            m_aafPoly[2][aiI[2]]*m_afInter[k];

        if (++aiI[0] <= m_iDegree)
        {
            continue;
        }
        aiI[0] = iDx;
        k += aiDelta[0];

        if (++aiI[1] <= m_iDegree)
        {
            continue;
        }
        aiI[1] = iDy;
        k += aiDelta[1];

        aiI[2]++;
    }
    return fResult;
}
//----------------------------------------------------------------------------
template <class Real>
void IntpBSplineUniform3<Real>::EvaluateUnknownData ()
{
    for (int k2 = m_aiGridMin[2]; k2 <= m_aiGridMax[2]; k2++)
    {
        for (int k1 = m_aiGridMin[1]; k1 <= m_aiGridMax[1]; k1++)
        {
            for (int k0 = m_aiGridMin[0]; k0 <= m_aiGridMax[0]; k0++)
            {
                int iIndex = Index(k0,k1,k2);
                if (m_afData[iIndex] == Math<Real>::MAX_REAL)
                {
                    m_afData[iIndex] = m_oEvaluateCallback(iIndex);
                }
            }
        }
    }
}
//----------------------------------------------------------------------------
template <class Real>
void IntpBSplineUniform3<Real>::ComputeIntermediate ()
{
    // fetch subblock of data to cache
    int aiDelta[2] = { m_aiDim[0]-m_iDp1, m_aiDim[0]*(m_aiDim[1]-m_iDp1) };
    int aiLoop[3];
    for (int iDim = 0; iDim < 3; iDim++)
    {
        aiLoop[iDim] = m_aiGridMin[iDim];
    }
    int iIndex = Index(aiLoop[0],aiLoop[1],aiLoop[2]);
    int k;
    for (k = 0; k < m_iDp1ToN; k++, iIndex++)
    {
        m_afCache[k] = m_afData[iIndex];

        if (++aiLoop[0] <= m_aiGridMax[0])
        {
            continue;
        }
        aiLoop[0] = m_aiGridMin[0];
        iIndex += aiDelta[0];

        if (++aiLoop[1] <= m_aiGridMax[1])
        {
            continue;
        }
        aiLoop[1] = m_aiGridMin[1];
        iIndex += aiDelta[1];

        aiLoop[2]++;
    }

    // compute and save the intermediate product
    for (int i = 0, j = 0; i < m_iDp1ToN; i++)
    {
        Real fSum = (Real)0.0;
        for (k = 0; k < m_iDp1ToN; k += m_aiSkip[j], j += m_aiSkip[j])
        {
            fSum += m_afProduct[j]*m_afCache[k];
        }
        m_afInter[i] = fSum;
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template WM4_FOUNDATION_ITEM
class IntpBSplineUniform3<float>;

template WM4_FOUNDATION_ITEM
class IntpBSplineUniform3<double>;
//----------------------------------------------------------------------------
}
