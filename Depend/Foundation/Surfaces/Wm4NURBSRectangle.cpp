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
#include "Wm4NURBSRectangle.h"

namespace Wm4
{
//----------------------------------------------------------------------------
template <class Real>
NURBSRectangle<Real>::NURBSRectangle (int iNumUCtrlPoints,
    int iNumVCtrlPoints, Vector3<Real>** aakCtrlPoint, Real** aafCtrlWeight,
    int iUDegree, int iVDegree, bool bULoop, bool bVLoop, bool bUOpen,
    bool bVOpen)
    :
    ParametricSurface<Real>((Real)0.0,(Real)1.0,(Real)0.0,(Real)1.0,true)
{
    assert(iNumUCtrlPoints >= 2);
    assert(1 <= iUDegree && iUDegree <= iNumUCtrlPoints-1);
    assert(iNumVCtrlPoints >= 2);
    assert(1 <= iVDegree && iVDegree <= iNumVCtrlPoints-1);

    m_abLoop[0] = bULoop;
    m_abLoop[1] = bVLoop;

    m_iNumUCtrlPoints = iNumUCtrlPoints;
    m_iNumVCtrlPoints = iNumVCtrlPoints;
    m_iUReplicate = (bULoop ? (bUOpen ? 1 : iUDegree) : 0);
    m_iVReplicate = (bVLoop ? (bVOpen ? 1 : iVDegree) : 0);
    CreateControl(aakCtrlPoint,aafCtrlWeight);

    m_akBasis[0].Create(m_iNumUCtrlPoints+m_iUReplicate,iUDegree,bUOpen);
    m_akBasis[1].Create(m_iNumVCtrlPoints+m_iVReplicate,iVDegree,bVOpen);
}
//----------------------------------------------------------------------------
template <class Real>
NURBSRectangle<Real>::NURBSRectangle (int iNumUCtrlPoints,
    int iNumVCtrlPoints, Vector3<Real>** aakCtrlPoint, Real** aafCtrlWeight,
    int iUDegree, int iVDegree, bool bULoop, bool bVLoop, bool bUOpen,
    Real* afVKnot)
    :
    ParametricSurface<Real>((Real)0.0,(Real)1.0,(Real)0.0,(Real)1.0,true)
{
    assert(iNumUCtrlPoints >= 2);
    assert(1 <= iUDegree && iUDegree <= iNumUCtrlPoints-1);
    assert(iNumVCtrlPoints >= 2);
    assert(1 <= iVDegree && iVDegree <= iNumVCtrlPoints-1);

    m_abLoop[0] = bULoop;
    m_abLoop[1] = bVLoop;

    m_iNumUCtrlPoints = iNumUCtrlPoints;
    m_iNumVCtrlPoints = iNumVCtrlPoints;
    m_iUReplicate = (bULoop ? (bUOpen ? 1 : iUDegree) : 0);
    m_iVReplicate = (bVLoop ? 1 : 0);
    CreateControl(aakCtrlPoint,aafCtrlWeight);

    m_akBasis[0].Create(m_iNumUCtrlPoints+m_iUReplicate,iUDegree,bUOpen);
    m_akBasis[1].Create(m_iNumVCtrlPoints+m_iVReplicate,iVDegree,afVKnot);
}
//----------------------------------------------------------------------------
template <class Real>
NURBSRectangle<Real>::NURBSRectangle (int iNumUCtrlPoints,
    int iNumVCtrlPoints, Vector3<Real>** aakCtrlPoint, Real** aafCtrlWeight,
    int iUDegree, int iVDegree, bool bULoop, bool bVLoop, Real* afUKnot,
    bool bVOpen)
    :
    ParametricSurface<Real>((Real)0.0,(Real)1.0,(Real)0.0,(Real)1.0,true)
{
    assert(iNumUCtrlPoints >= 2);
    assert(1 <= iUDegree && iUDegree <= iNumUCtrlPoints-1);
    assert(iNumVCtrlPoints >= 2);
    assert(1 <= iVDegree && iVDegree <= iNumVCtrlPoints-1);

    m_abLoop[0] = bULoop;
    m_abLoop[1] = bVLoop;

    m_iNumUCtrlPoints = iNumUCtrlPoints;
    m_iNumVCtrlPoints = iNumVCtrlPoints;
    m_iUReplicate = (bULoop ? 1 : 0);
    m_iVReplicate = (bVLoop ? (bVOpen ? 1 : iVDegree) : 0);
    CreateControl(aakCtrlPoint,aafCtrlWeight);

    m_akBasis[0].Create(m_iNumUCtrlPoints+m_iUReplicate,iUDegree,afUKnot);
    m_akBasis[1].Create(m_iNumVCtrlPoints+m_iVReplicate,iVDegree,bVOpen);
}
//----------------------------------------------------------------------------
template <class Real>
NURBSRectangle<Real>::NURBSRectangle (int iNumUCtrlPoints,
    int iNumVCtrlPoints, Vector3<Real>** aakCtrlPoint, Real** aafCtrlWeight,
    int iUDegree, int iVDegree, bool bULoop, bool bVLoop, Real* afUKnot,
    Real* afVKnot)
    :
    ParametricSurface<Real>((Real)0.0,(Real)1.0,(Real)0.0,(Real)1.0,true)
{
    assert(iNumUCtrlPoints >= 2);
    assert(1 <= iUDegree && iUDegree <= iNumUCtrlPoints-1);
    assert(iNumVCtrlPoints >= 2);
    assert(1 <= iVDegree && iVDegree <= iNumVCtrlPoints-1);

    m_abLoop[0] = bULoop;
    m_abLoop[1] = bVLoop;

    m_iNumUCtrlPoints = iNumUCtrlPoints;
    m_iNumVCtrlPoints = iNumVCtrlPoints;
    m_iUReplicate = (bULoop ? 1 : 0);
    m_iVReplicate = (bVLoop ? 1 : 0);
    CreateControl(aakCtrlPoint,aafCtrlWeight);

    m_akBasis[0].Create(m_iNumUCtrlPoints+m_iUReplicate,iUDegree,afUKnot);
    m_akBasis[1].Create(m_iNumVCtrlPoints+m_iVReplicate,iVDegree,afVKnot);
}
//----------------------------------------------------------------------------
template <class Real>
NURBSRectangle<Real>::~NURBSRectangle ()
{
    WM4_DELETE[] m_aakCtrlPoint[0];
    WM4_DELETE[] m_aakCtrlPoint;
    WM4_DELETE[] m_aafCtrlWeight[0];
    WM4_DELETE[] m_aafCtrlWeight;
}
//----------------------------------------------------------------------------
template <class Real>
void NURBSRectangle<Real>::CreateControl (Vector3<Real>** aakCtrlPoint,
    Real** aafCtrlWeight)
{
    int iNewNumUCtrlPoints = m_iNumUCtrlPoints + m_iUReplicate;
    int iNewNumVCtrlPoints = m_iNumVCtrlPoints + m_iVReplicate;
    int iQuantity = iNewNumUCtrlPoints*iNewNumVCtrlPoints;
    m_aakCtrlPoint = WM4_NEW Vector3<Real>*[iNewNumUCtrlPoints];
    m_aakCtrlPoint[0] = WM4_NEW Vector3<Real>[iQuantity];
    m_aafCtrlWeight = WM4_NEW Real*[iNewNumUCtrlPoints];
    m_aafCtrlWeight[0] = WM4_NEW Real[iQuantity];
    for (int i = 1; i < iNewNumUCtrlPoints; i++)
    {
        m_aakCtrlPoint[i] = &m_aakCtrlPoint[0][i*iNewNumVCtrlPoints];
        m_aafCtrlWeight[i] = &m_aafCtrlWeight[0][i*iNewNumVCtrlPoints];
    }

    for (int iU = 0; iU < iNewNumUCtrlPoints; iU++)
    {
        int iUOld = iU % m_iNumUCtrlPoints;
        for (int iV = 0; iV < iNewNumVCtrlPoints; iV++)
        {
            int iVOld = iV % m_iNumVCtrlPoints;
            m_aakCtrlPoint[iU][iV] = aakCtrlPoint[iUOld][iVOld];
            m_aafCtrlWeight[iU][iV] = aafCtrlWeight[iUOld][iVOld];
        }
    }
}
//----------------------------------------------------------------------------
template <class Real>
int NURBSRectangle<Real>::GetNumCtrlPoints (int iDim) const
{
    assert(0 <= iDim && iDim <= 1);
    return m_akBasis[iDim].GetNumCtrlPoints();
}
//----------------------------------------------------------------------------
template <class Real>
int NURBSRectangle<Real>::GetDegree (int iDim) const
{
    assert(0 <= iDim && iDim <= 1);
    return m_akBasis[iDim].GetDegree();
}
//----------------------------------------------------------------------------
template <class Real>
bool NURBSRectangle<Real>::IsOpen (int iDim) const
{
    assert(0 <= iDim && iDim <= 1);
    return m_akBasis[iDim].IsOpen();
}
//----------------------------------------------------------------------------
template <class Real>
bool NURBSRectangle<Real>::IsUniform (int iDim) const
{
    assert(0 <= iDim && iDim <= 1);
    return m_akBasis[iDim].IsUniform();
}
//----------------------------------------------------------------------------
template <class Real>
bool NURBSRectangle<Real>::IsLoop (int iDim) const
{
    assert(0 <= iDim && iDim <= 1);
    return m_abLoop[iDim];
}
//----------------------------------------------------------------------------
template <class Real>
void NURBSRectangle<Real>::SetControlPoint (int iUIndex, int iVIndex,
    const Vector3<Real>& rkCtrl)
{
    if (0 <= iUIndex && iUIndex < m_iNumUCtrlPoints
    &&  0 <= iVIndex && iVIndex < m_iNumVCtrlPoints)
    {
        // set the control point
        m_aakCtrlPoint[iUIndex][iVIndex] = rkCtrl;

        // set the replicated control point
        bool bDoUReplicate = (iUIndex < m_iUReplicate);
        bool bDoVReplicate = (iVIndex < m_iVReplicate);
        int iUExt = 0, iVExt = 0;

        if (bDoUReplicate)
        {
            iUExt = m_iNumUCtrlPoints + iUIndex;
            m_aakCtrlPoint[iUExt][iVIndex] = rkCtrl;
        }
        if (bDoVReplicate)
        {
            iVExt = m_iNumVCtrlPoints + iVIndex;
            m_aakCtrlPoint[iUIndex][iVExt] = rkCtrl;
        }
        if (bDoUReplicate && bDoVReplicate)
        {
            m_aakCtrlPoint[iUExt][iVExt] = rkCtrl;
        }
    }
}
//----------------------------------------------------------------------------
template <class Real>
Vector3<Real> NURBSRectangle<Real>::GetControlPoint (int iUIndex,
    int iVIndex) const
{
    if (0 <= iUIndex && iUIndex < m_iNumUCtrlPoints
    &&  0 <= iVIndex && iVIndex < m_iNumVCtrlPoints)
    {
        return m_aakCtrlPoint[iUIndex][iVIndex];
    }

    return Vector3<Real>(Math<Real>::MAX_REAL,Math<Real>::MAX_REAL,
        Math<Real>::MAX_REAL);
}
//----------------------------------------------------------------------------
template <class Real>
void NURBSRectangle<Real>::SetControlWeight (int iUIndex, int iVIndex,
    Real fWeight)
{
    if (0 <= iUIndex && iUIndex < m_iNumUCtrlPoints
    &&  0 <= iVIndex && iVIndex < m_iNumVCtrlPoints)
    {
        // set the control weight
        m_aafCtrlWeight[iUIndex][iVIndex] = fWeight;

        // set the replicated control point
        bool bDoUReplicate = (iUIndex < m_iUReplicate );
        bool bDoVReplicate = (iVIndex < m_iVReplicate);
        int iUExt = 0, iVExt = 0;

        if (bDoUReplicate)
        {
            iUExt = m_iNumUCtrlPoints + iUIndex;
            m_aafCtrlWeight[iUExt][iVIndex] = fWeight;
        }
        if (bDoVReplicate)
        {
            iVExt = m_iNumVCtrlPoints + iVIndex;
            m_aafCtrlWeight[iUIndex][iVExt] = fWeight;
        }
        if (bDoUReplicate && bDoVReplicate)
        {
            m_aafCtrlWeight[iUExt][iVExt] = fWeight;
        }
    }
}
//----------------------------------------------------------------------------
template <class Real>
Real NURBSRectangle<Real>::GetControlWeight (int iUIndex, int iVIndex) const
{
    if (0 <= iUIndex && iUIndex < m_iNumUCtrlPoints
    &&  0 <= iVIndex && iVIndex < m_iNumVCtrlPoints)
    {
        return m_aafCtrlWeight[iUIndex][iVIndex];
    }

    return Math<Real>::MAX_REAL;
}
//----------------------------------------------------------------------------
template <class Real>
void NURBSRectangle<Real>::SetKnot (int iDim, int i, Real fKnot)
{
    if (0 <= iDim && iDim <= 1)
    {
        m_akBasis[iDim].SetKnot(i,fKnot);
    }
}
//----------------------------------------------------------------------------
template <class Real>
Real NURBSRectangle<Real>::GetKnot (int iDim, int i) const
{
    if (0 <= iDim && iDim <= 1)
    {
        return m_akBasis[iDim].GetKnot(i);
    }

    return Math<Real>::MAX_REAL;
}
//----------------------------------------------------------------------------
template <class Real>
void NURBSRectangle<Real>::Get (Real fU, Real fV, Vector3<Real>* pkPos,
    Vector3<Real>* pkDerU, Vector3<Real>* pkDerV, Vector3<Real>* pkDerUU,
    Vector3<Real>* pkDerUV, Vector3<Real>* pkDerVV) const
{
    int iU, iUMin, iUMax;
    if (pkDerUU)
    {
        m_akBasis[0].Compute(fU,0,iUMin,iUMax);
        m_akBasis[0].Compute(fU,1,iUMin,iUMax);
        m_akBasis[0].Compute(fU,2,iUMin,iUMax);
    }
    else if (pkDerUV || pkDerU)
    {
        m_akBasis[0].Compute(fU,0,iUMin,iUMax);
        m_akBasis[0].Compute(fU,1,iUMin,iUMax);
    }
    else
    {
        m_akBasis[0].Compute(fU,0,iUMin,iUMax);
    }

    int iV, iVMin, iVMax;
    if (pkDerVV)
    {
        m_akBasis[1].Compute(fV,0,iVMin,iVMax);
        m_akBasis[1].Compute(fV,1,iVMin,iVMax);
        m_akBasis[1].Compute(fV,2,iVMin,iVMax);
    }
    else if (pkDerUV || pkDerV)
    {
        m_akBasis[1].Compute(fV,0,iVMin,iVMax);
        m_akBasis[1].Compute(fV,1,iVMin,iVMax);
    }
    else
    {
        m_akBasis[1].Compute(fV,0,iVMin,iVMax);
    }

    Real fTmp;

    Vector3<Real> kX = Vector3<Real>::ZERO;
    Real fW = 0.0f;
    for (iU = iUMin; iU <= iUMax; iU++)
    {
        for (iV = iVMin; iV <= iVMax; iV++)
        {
            fTmp = m_akBasis[0].GetD0(iU)*m_akBasis[1].GetD0(iV)*
                m_aafCtrlWeight[iU][iV];
            kX += fTmp*m_aakCtrlPoint[iU][iV];
            fW += fTmp;
        }
    }
    Real fInvW = 1.0f/fW;
    Vector3<Real> kP = fInvW*kX;
    if (pkPos)
    {
        *pkPos = kP;
    }

    if (!pkDerU && !pkDerV && !pkDerUU && !pkDerUV && !pkDerVV)
    {
        return;
    }

    Vector3<Real> kXDerU = Vector3<Real>::ZERO;
    Real fWDerU = 0.0f;
    for (iU = iUMin; iU <= iUMax; iU++)
    {
        for (iV = iVMin; iV <= iVMax; iV++)
        {
            fTmp = m_akBasis[0].GetD1(iU)*m_akBasis[1].GetD0(iV)*
                m_aafCtrlWeight[iU][iV];
            kXDerU += fTmp*m_aakCtrlPoint[iU][iV];
            fWDerU += fTmp;
        }
    }
    Vector3<Real> kPDerU = fInvW*(kXDerU - fWDerU*kP);
    if (pkDerU)
    {
        *pkDerU = kPDerU;
    }

    Vector3<Real> kXDerV = Vector3<Real>::ZERO;
    Real fWDerV = 0.0f;
    for (iU = iUMin; iU <= iUMax; iU++)
    {
        for (iV = iVMin; iV <= iVMax; iV++)
        {
            fTmp = m_akBasis[0].GetD0(iU)*m_akBasis[1].GetD1(iV)*
                m_aafCtrlWeight[iU][iV];
            kXDerV += fTmp*m_aakCtrlPoint[iU][iV];
            fWDerV += fTmp;
        }
    }
    Vector3<Real> kPDerV = fInvW*(kXDerV - fWDerV*kP);
    if (pkDerV)
    {
        *pkDerV = kPDerV;
    }

    if (!pkDerUU && !pkDerUV && !pkDerVV)
    {
        return;
    }

    Vector3<Real> kXDerUU = Vector3<Real>::ZERO;
    Real fWDerUU = 0.0f;
    for (iU = iUMin; iU <= iUMax; iU++)
    {
        for (iV = iVMin; iV <= iVMax; iV++)
        {
            fTmp = m_akBasis[0].GetD2(iU)*m_akBasis[1].GetD0(iV)*
                m_aafCtrlWeight[iU][iV];
            kXDerUU += fTmp*m_aakCtrlPoint[iU][iV];
            fWDerUU += fTmp;
        }
    }
    Vector3<Real> kPDerUU = fInvW*(kXDerUU-2.0f*fWDerU*kPDerU-fWDerUU*kP);
    if (pkDerUU)
    {
        *pkDerUU = kPDerUU;
    }

    Vector3<Real> kXDerUV = Vector3<Real>::ZERO;
    Real fWDerUV = 0.0f;
    for (iU = iUMin; iU <= iUMax; iU++)
    {
        for (iV = iVMin; iV <= iVMax; iV++)
        {
            fTmp = m_akBasis[0].GetD1(iU)*m_akBasis[1].GetD1(iV)*
                m_aafCtrlWeight[iU][iV];
            kXDerUV += fTmp*m_aakCtrlPoint[iU][iV];
            fWDerUV += fTmp;
        }
    }
    Vector3<Real> kPDerUV = fInvW*(kXDerUV - fWDerU*kPDerV - fWDerV*kPDerU -
        fWDerUV*kP);
    if (pkDerUV)
    {
        *pkDerUV = kPDerUV;
    }

    Vector3<Real> kXDerVV = Vector3<Real>::ZERO;
    Real fWDerVV = 0.0f;
    for (iU = iUMin; iU <= iUMax; iU++)
    {
        for (iV = iVMin; iV <= iVMax; iV++)
        {
            fTmp = m_akBasis[0].GetD0(iU)*m_akBasis[1].GetD2(iV)*
                m_aafCtrlWeight[iU][iV];
            kXDerVV += fTmp*m_aakCtrlPoint[iU][iV];
            fWDerVV += fTmp;
        }
    }
    Vector3<Real> kPDerVV = fInvW*(kXDerVV-2.0f*fWDerV*kPDerV-fWDerVV*kP);
    if (pkDerVV)
    {
        *pkDerVV = kPDerVV;
    }
}
//----------------------------------------------------------------------------
template <class Real>
Vector3<Real> NURBSRectangle<Real>::P (Real fU, Real fV) const
{
    Vector3<Real> kP;
    Get(fU,fV,&kP,0,0,0,0,0);
    return kP;
}
//----------------------------------------------------------------------------
template <class Real>
Vector3<Real> NURBSRectangle<Real>::PU (Real fU, Real fV) const
{
    Vector3<Real> kPU;
    Get(fU,fV,0,&kPU,0,0,0,0);
    return kPU;
}
//----------------------------------------------------------------------------
template <class Real>
Vector3<Real> NURBSRectangle<Real>::PV (Real fU, Real fV) const
{
    Vector3<Real> kPV;
    Get(fU,fV,0,0,&kPV,0,0,0);
    return kPV;
}
//----------------------------------------------------------------------------
template <class Real>
Vector3<Real> NURBSRectangle<Real>::PUU (Real fU, Real fV) const
{
    Vector3<Real> kPUU;
    Get(fU,fV,0,0,0,&kPUU,0,0);
    return kPUU;
}
//----------------------------------------------------------------------------
template <class Real>
Vector3<Real> NURBSRectangle<Real>::PUV (Real fU, Real fV) const
{
    Vector3<Real> kPUV;
    Get(fU,fV,0,0,0,0,&kPUV,0);
    return kPUV;
}
//----------------------------------------------------------------------------
template <class Real>
Vector3<Real> NURBSRectangle<Real>::PVV (Real fU, Real fV) const
{
    Vector3<Real> kPVV;
    Get(fU,fV,0,0,0,0,0,&kPVV);
    return kPVV;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template WM4_FOUNDATION_ITEM
class NURBSRectangle<float>;

template WM4_FOUNDATION_ITEM
class NURBSRectangle<double>;
//----------------------------------------------------------------------------
}
