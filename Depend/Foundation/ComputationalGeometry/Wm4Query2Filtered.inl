// Geometric Tools, Inc.
// http://www.geometrictools.com
// Copyright (c) 1998-2006.  All Rights Reserved
//
// The Wild Magic Version 4 Foundation Library source code is supplied
// under the terms of the license agreement
//     http://www.geometrictools.com/License/Wm4FoundationLicense.pdf
// and may not be copied or disclosed except in accordance with the terms
// of that agreement.

//----------------------------------------------------------------------------
template <class Real>
Query2Filtered<Real>::Query2Filtered (int iVQuantity,
    const Vector2<Real>* akVertex, Real fUncertainty)
    :
    Query2<Real>(iVQuantity,akVertex),
    m_kRQuery(iVQuantity,akVertex)
{
    assert((Real)0.0 <= fUncertainty && fUncertainty <= (Real)1.0);
    m_fUncertainty = fUncertainty;
}
//----------------------------------------------------------------------------
template <class Real>
Query2Filtered<Real>::~Query2Filtered ()
{
}
//----------------------------------------------------------------------------
template <class Real>
Query::Type Query2Filtered<Real>::GetType () const
{
    return Query::QT_FILTERED;
}
//----------------------------------------------------------------------------
template <class Real>
int Query2Filtered<Real>::ToLine (const Vector2<Real>& rkP, int iV0, int iV1)
    const
{
    const Vector2<Real>& rkV0 = m_akVertex[iV0];
    const Vector2<Real>& rkV1 = m_akVertex[iV1];

    Real fX0 = rkP[0] - rkV0[0];
    Real fY0 = rkP[1] - rkV0[1];
    Real fX1 = rkV1[0] - rkV0[0];
    Real fY1 = rkV1[1] - rkV0[1];

    Real fLen0 = Math<Real>::Sqrt(fX0*fX0 + fY0*fY0);
    Real fLen1 = Math<Real>::Sqrt(fX1*fX1 + fY1*fY1);
    Real fScaledUncertainty = m_fUncertainty*fLen0*fLen1;

    Real fDet2 = Det2(fX0,fY0,fX1,fY1);
    if (Math<Real>::FAbs(fDet2) >= fScaledUncertainty)
    {
        return (fDet2 > (Real)0.0 ? +1 : (fDet2 < (Real)0.0 ? -1 : 0));
    }

    return m_kRQuery.ToLine(rkP,iV0,iV1);
}
//----------------------------------------------------------------------------
template <class Real>
int Query2Filtered<Real>::ToCircumcircle (const Vector2<Real>& rkP, int iV0,
    int iV1, int iV2) const
{
    const Vector2<Real>& rkV0 = m_akVertex[iV0];
    const Vector2<Real>& rkV1 = m_akVertex[iV1];
    const Vector2<Real>& rkV2 = m_akVertex[iV2];

    Real fS0x = rkV0[0] + rkP[0];
    Real fD0x = rkV0[0] - rkP[0];
    Real fS0y = rkV0[1] + rkP[1];
    Real fD0y = rkV0[1] - rkP[1];
    Real fS1x = rkV1[0] + rkP[0];
    Real fD1x = rkV1[0] - rkP[0];
    Real fS1y = rkV1[1] + rkP[1];
    Real fD1y = rkV1[1] - rkP[1];
    Real fS2x = rkV2[0] + rkP[0];
    Real fD2x = rkV2[0] - rkP[0];
    Real fS2y = rkV2[1] + rkP[1];
    Real fD2y = rkV2[1] - rkP[1];
    Real fZ0 = fS0x*fD0x + fS0y*fD0y;
    Real fZ1 = fS1x*fD1x + fS1y*fD1y;
    Real fZ2 = fS2x*fD2x + fS2y*fD2y;

    Real fLen0 = Math<Real>::Sqrt(fD0x*fD0x + fD0y*fD0y + fZ0*fZ0);
    Real fLen1 = Math<Real>::Sqrt(fD1x*fD1x + fD1y*fD1y + fZ1*fZ1);
    Real fLen2 = Math<Real>::Sqrt(fD2x*fD2x + fD2y*fD2y + fZ2*fZ2);
    Real fScaledUncertainty = m_fUncertainty*fLen0*fLen1*fLen2;

    Real fDet3 = Det3(fD0x,fD0y,fZ0,fD1x,fD1y,fZ1,fD2x,fD2y,fZ2);
    if (Math<Real>::FAbs(fDet3) >= fScaledUncertainty)
    {
        return (fDet3 < (Real)0.0 ? 1 : (fDet3 > (Real)0.0 ? -1 : 0));
    }

    return m_kRQuery.ToCircumcircle(rkP,iV0,iV1,iV2);
}
//----------------------------------------------------------------------------
