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
inline int ETNonmanifoldMesh::GetEdgeQuantity () const
{
    return (int)m_kEMap.size();
}
//----------------------------------------------------------------------------
inline const ETNonmanifoldMesh::EMap& ETNonmanifoldMesh::GetEdges () const
{
    return m_kEMap;
}
//----------------------------------------------------------------------------
inline int ETNonmanifoldMesh::GetTriangleQuantity () const
{
    return (int)m_kTMap.size();
}
//----------------------------------------------------------------------------
inline const ETNonmanifoldMesh::TMap& ETNonmanifoldMesh::GetTriangles () const
{
    return m_kTMap;
}
//----------------------------------------------------------------------------
