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
Query2Int64<Real>::Query2Int64 (int iVQuantity, const Vector2<Real>* akVertex)
    :
    Query2<Real>(iVQuantity,akVertex)
{
}
//----------------------------------------------------------------------------
template <class Real>
Query::Type Query2Int64<Real>::GetType () const
{
    return Query::QT_INT64;
}
//----------------------------------------------------------------------------
template <class Real>
int Query2Int64<Real>::ToLine (const Vector2<Real>& rkP, int iV0, int iV1)
    const
{
    const Vector2<Real>& rkV0 = m_akVertex[iV0];
    const Vector2<Real>& rkV1 = m_akVertex[iV1];

    Integer64 iX0 = (Integer64)rkP[0] - (Integer64)rkV0[0];
    Integer64 iY0 = (Integer64)rkP[1] - (Integer64)rkV0[1];
    Integer64 iX1 = (Integer64)rkV1[0] - (Integer64)rkV0[0];
    Integer64 iY1 = (Integer64)rkV1[1] - (Integer64)rkV0[1];

    Integer64 iDet2 = Det2(iX0,iY0,iX1,iY1);
    return (iDet2 > 0 ? +1 : (iDet2 < 0 ? -1 : 0));
}
//----------------------------------------------------------------------------
template <class Real>
int Query2Int64<Real>::ToCircumcircle (const Vector2<Real>& rkP, int iV0,
    int iV1, int iV2) const
{
    const Vector2<Real>& rkV0 = m_akVertex[iV0];
    const Vector2<Real>& rkV1 = m_akVertex[iV1];
    const Vector2<Real>& rkV2 = m_akVertex[iV2];

    Integer64 aiP[2] = { (Integer64)rkP[0], (Integer64)rkP[1] };
    Integer64 aiV0[2] = { (Integer64)rkV0[0], (Integer64)rkV0[1] };
    Integer64 aiV1[2] = { (Integer64)rkV1[0], (Integer64)rkV1[1] };
    Integer64 aiV2[2] = { (Integer64)rkV2[0], (Integer64)rkV2[1] };

    Integer64 iS0x = aiV0[0] + aiP[0];
    Integer64 iD0x = aiV0[0] - aiP[0];
    Integer64 iS0y = aiV0[1] + aiP[1];
    Integer64 iD0y = aiV0[1] - aiP[1];
    Integer64 iS1x = aiV1[0] + aiP[0];
    Integer64 iD1x = aiV1[0] - aiP[0];
    Integer64 iS1y = aiV1[1] + aiP[1];
    Integer64 iD1y = aiV1[1] - aiP[1];
    Integer64 iS2x = aiV2[0] + aiP[0];
    Integer64 iD2x = aiV2[0] - aiP[0];
    Integer64 iS2y = aiV2[1] + aiP[1];
    Integer64 iD2y = aiV2[1] - aiP[1];
    Integer64 iZ0 = iS0x*iD0x + iS0y*iD0y;
    Integer64 iZ1 = iS1x*iD1x + iS1y*iD1y;
    Integer64 iZ2 = iS2x*iD2x + iS2y*iD2y;
    Integer64 iDet3 = Det3(iD0x,iD0y,iZ0,iD1x,iD1y,iZ1,iD2x,iD2y,iZ2);
    return (iDet3 < 0 ? 1 : (iDet3 > 0 ? -1 : 0));
}
//----------------------------------------------------------------------------
template <class Real>
Integer64 Query2Int64<Real>::Dot (Integer64 iX0, Integer64 iY0, Integer64 iX1,
    Integer64 iY1)
{
    return iX0*iX1 + iY0*iY1;
}
//----------------------------------------------------------------------------
template <class Real>
Integer64 Query2Int64<Real>::Det2 (Integer64 iX0, Integer64 iY0,
    Integer64 iX1, Integer64 iY1)
{
    return iX0*iY1 - iX1*iY0;
}
//----------------------------------------------------------------------------
template <class Real>
Integer64 Query2Int64<Real>::Det3 (Integer64 iX0, Integer64 iY0,
    Integer64 iZ0, Integer64 iX1, Integer64 iY1, Integer64 iZ1, Integer64 iX2,
    Integer64 iY2, Integer64 iZ2)
{
    Integer64 iC00 = iY1*iZ2 - iY2*iZ1;
    Integer64 iC01 = iY2*iZ0 - iY0*iZ2;
    Integer64 iC02 = iY0*iZ1 - iY1*iZ0;
    return iX0*iC00 + iX1*iC01 + iX2*iC02;
}
//----------------------------------------------------------------------------
