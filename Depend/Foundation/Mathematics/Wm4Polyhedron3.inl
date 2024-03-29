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
Polyhedron3<Real>::Polyhedron3 (int iVQuantity, Vector3<Real>* akVertex,
    int iTQuantity, int* aiIndex, bool bOwner)
{
    // The polyhedron must be at least a tetrahedron.
    assert(iVQuantity >= 4 && akVertex);
    assert(iTQuantity >= 4 && aiIndex);

    m_iVQuantity = iVQuantity;
    m_akVertex = akVertex;
    m_iTQuantity = iTQuantity;
    m_aiIndex = aiIndex;
    m_bOwner = bOwner;
}
//----------------------------------------------------------------------------
template <class Real>
Polyhedron3<Real>::Polyhedron3 (const Polyhedron3& rkPoly)
{
    m_akVertex = 0;
    m_aiIndex = 0;
    m_bOwner = false;
    *this = rkPoly;
}
//----------------------------------------------------------------------------
template <class Real>
Polyhedron3<Real>::~Polyhedron3 ()
{
    if (m_bOwner)
    {
        WM4_DELETE[] m_akVertex;
        WM4_DELETE[] m_aiIndex;
    }
}
//----------------------------------------------------------------------------
template <class Real>
Polyhedron3<Real>& Polyhedron3<Real>::operator= (const Polyhedron3& rkPoly)
{
    if (m_bOwner)
    {
        WM4_DELETE[] m_akVertex;
        WM4_DELETE[] m_aiIndex;
    }

    m_iVQuantity = rkPoly.m_iVQuantity;
    m_iTQuantity = rkPoly.m_iTQuantity;
    m_bOwner = rkPoly.m_bOwner;

    if (m_bOwner)
    {
        m_akVertex = WM4_NEW Vector3<Real>[m_iVQuantity];
        size_t uiSize = m_iVQuantity*sizeof(Vector3<Real>);
        System::Memcpy(m_akVertex,uiSize,rkPoly.m_akVertex,uiSize);
        m_aiIndex = WM4_NEW int[3*m_iTQuantity];
        uiSize = 3*m_iTQuantity*sizeof(int);
        System::Memcpy(m_aiIndex,uiSize,rkPoly.m_aiIndex,uiSize);
    }
    else
    {
        m_akVertex = rkPoly.m_akVertex;
        m_aiIndex = rkPoly.m_aiIndex;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
int Polyhedron3<Real>::GetVQuantity () const
{
    return m_iVQuantity;
}
//----------------------------------------------------------------------------
template <class Real>
const Vector3<Real>* Polyhedron3<Real>::GetVertices () const
{
    return m_akVertex;
}
//----------------------------------------------------------------------------
template <class Real>
const Vector3<Real>& Polyhedron3<Real>::GetVertex (int i) const
{
    assert(0 <= i && i < m_iVQuantity);
    return m_akVertex[i];
}
//----------------------------------------------------------------------------
template <class Real>
int Polyhedron3<Real>::GetTQuantity () const
{
    return m_iTQuantity;
}
//----------------------------------------------------------------------------
template <class Real>
const int* Polyhedron3<Real>::GetIndices () const
{
    return m_aiIndex;
}
//----------------------------------------------------------------------------
template <class Real>
const int* Polyhedron3<Real>::GetTriangle (int i) const
{
    assert(0 <= i && i < m_iTQuantity);
    return &m_aiIndex[3*i];
}
//----------------------------------------------------------------------------
template <class Real>
void Polyhedron3<Real>::SetVertex (int i, const Vector3<Real>& rkV)
{
    assert(0 <= i && i < m_iVQuantity);
    m_akVertex[i] = rkV;
}
//----------------------------------------------------------------------------
template <class Real>
Vector3<Real>* Polyhedron3<Real>::GetVertices ()
{
    return m_akVertex;
}
//----------------------------------------------------------------------------
template <class Real>
Vector3<Real> Polyhedron3<Real>::ComputeVertexAverage () const
{
    Vector3<Real> kAverage = m_akVertex[0];
    for (int i = 1; i < m_iVQuantity; i++)
    {
        kAverage += m_akVertex[i];
    }

    kAverage /= (Real)m_iVQuantity;
    return kAverage;
}
//----------------------------------------------------------------------------
template <class Real>
Real Polyhedron3<Real>::ComputeSurfaceArea () const
{
    Real fSurfaceArea = (Real)0.0;

    const int* piIndex = m_aiIndex;
    for (int i = 0; i < m_iTQuantity; i++)
    {
        int iV0 = *piIndex++;
        int iV1 = *piIndex++;
        int iV2 = *piIndex++;
        Vector3<Real> kEdge0 = m_akVertex[iV1] - m_akVertex[iV0];
        Vector3<Real> kEdge1 = m_akVertex[iV2] - m_akVertex[iV0];
        Vector3<Real> kCross = kEdge0.Cross(kEdge1);
        fSurfaceArea += kCross.Length();
    }

    fSurfaceArea *= (Real)0.5;
    return fSurfaceArea;
}
//----------------------------------------------------------------------------
template <class Real>
Real Polyhedron3<Real>::ComputeVolume () const
{
    Real fVolume = (Real)0.0;

    const int* piIndex = m_aiIndex;
    for (int i = 0; i < m_iTQuantity; i++)
    {
        int iV0 = *piIndex++;
        int iV1 = *piIndex++;
        int iV2 = *piIndex++;
        fVolume +=
            m_akVertex[iV0].Dot(m_akVertex[iV1].Cross(m_akVertex[iV2]));
    }

    fVolume /= (Real)6.0;
    return fVolume;
}
//----------------------------------------------------------------------------
