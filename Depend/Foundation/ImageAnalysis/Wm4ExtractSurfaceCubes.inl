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
inline bool ExtractSurfaceCubes::VETable::IsValidVertex (int i) const
{
    assert(0 <= i && i < 18);
    return m_akVertex[i].Valid;
}
//----------------------------------------------------------------------------
inline float ExtractSurfaceCubes::VETable::GetX (int i) const
{
    assert(0 <= i && i < 18);
    return m_akVertex[i].P.X();
}
//----------------------------------------------------------------------------
inline float ExtractSurfaceCubes::VETable::GetY (int i) const
{
    assert(0 <= i && i < 18);
    return m_akVertex[i].P.Y();
}
//----------------------------------------------------------------------------
inline float ExtractSurfaceCubes::VETable::GetZ (int i) const
{
    assert(0 <= i && i < 18);
    return m_akVertex[i].P.Z();
}
//----------------------------------------------------------------------------
inline ExtractSurfaceCubes::VETable::Vertex::Vertex ()
{
    AdjQuantity = 0;
    Valid = false;
}
//----------------------------------------------------------------------------
