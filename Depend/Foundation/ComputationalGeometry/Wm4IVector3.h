// Geometric Tools, Inc.
// http://www.geometrictools.com
// Copyright (c) 1998-2005.  All Rights Reserved
//
// The Wild Magic Library (WM) source code is supplied under the terms of
// the license agreement
//     http://www.geometrictools.com/License/WildMagic4License.pdf
// and may not be copied or disclosed except in accordance with the terms
// of that agreement.

#ifndef WM4IVECTOR3_H
#define WM4IVECTOR3_H

#include "Wm4FoundationLIB.h"
#include "Wm4TIVector.h"

namespace Wm4
{

class IVector3 : public TIVector<3>
{
public:
    // construction
    IVector3 ();
    IVector3 (const IVector3& rkV);
    IVector3 (const TIVector<3>& rkV);
    IVector3 (const Integer64& riX, const Integer64& riY,
        const Integer64& riZ);

    // member access
    Integer64 X () const;
    Integer64& X ();
    Integer64 Y () const;
    Integer64& Y ();
    Integer64 Z () const;
    Integer64& Z ();

    // assignment
    IVector3& operator= (const IVector3& rkV);
    IVector3& operator= (const TIVector<3>& rkV);

    // returns Dot(this,V)
    Integer64 Dot (const IVector3& rkV) const;

    // returns Cross(this,V)
    IVector3 Cross (const IVector3& rkV) const;

    // returns Dot(this,Cross(U,V))
    Integer64 TripleScalar (const IVector3& rkU, const IVector3& rkV) const;

protected:
    using TIVector<3>::m_aiTuple;
};

#include "Wm4IVector3.inl"

}

#endif
