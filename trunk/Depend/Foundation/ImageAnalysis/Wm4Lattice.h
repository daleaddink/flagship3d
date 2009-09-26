// Geometric Tools, Inc.
// http://www.geometrictools.com
// Copyright (c) 1998-2006.  All Rights Reserved
//
// The Wild Magic Version 4 Foundation Library source code is supplied
// under the terms of the license agreement
//     http://www.geometrictools.com/License/Wm4FoundationLicense.pdf
// and may not be copied or disclosed except in accordance with the terms
// of that agreement.

#ifndef WM4LATTICE_H
#define WM4LATTICE_H

#include "Wm4FoundationLIB.h"
#include "Wm4System.h"

namespace Wm4
{

class WM4_FOUNDATION_ITEM Lattice
{
public:
    // abstract base class for TImage
    virtual ~Lattice ();

    // data access
    int GetDimensions () const;
    const int* GetBounds () const;
    int GetBound (int i) const;
    int GetQuantity () const;
    const int* GetOffsets () const;
    int GetOffset (int i) const;

    // assignment
    Lattice& operator= (const Lattice& rkLattice);

    // comparisons
    bool operator== (const Lattice& rkLattice) const;
    bool operator!= (const Lattice& rkLattice) const;

    // Conversions between n-dim and 1-dim structures.  The coordinate arrays
    // must have the same number of elements as the dimensions of the lattice.
    int GetIndex (const int* aiCoord) const;
    void GetCoordinates (int iIndex, int* aiCoord) const;

    // streaming
    bool Load (FILE* pkIFile);
    bool Save (FILE* pkOFile) const;

    static bool LoadRaw (const char* acFilename, int& riDimensions,
        int*& raiBound, int& riQuantity, int& riRTTI, int& riSizeOf,
        char*& racData);

protected:
    // Construction.  Lattice accepts responsibility for deleting the
    // bound array.
    Lattice (int iDimensions, int* aiBound);
    Lattice (const Lattice& rkLattice);
    Lattice ();

    // For deferred creation of bounds.  Lattice accepts responsibility
    // for deleting the bound array.
    Lattice (int iDimensions);
    void SetBounds (int* aiBound);
    void ComputeQuantityAndOffsets ();

    int m_iDimensions;
    int* m_aiBound;
    int m_iQuantity;
    int* m_aiOffset;

    // streaming
    static const char* ms_acHeader;
};

#include "Wm4Lattice.inl"

}

#endif
