// Geometric Tools, Inc.
// http://www.geometrictools.com
// Copyright (c) 1998-2006.  All Rights Reserved
//
// The Wild Magic Version 4 Foundation Library source code is supplied
// under the terms of the license agreement
//     http://www.geometrictools.com/License/Wm4FoundationLicense.pdf
// and may not be copied or disclosed except in accordance with the terms
// of that agreement.

#ifndef WM4QUERY3FILTERED_H
#define WM4QUERY3FILTERED_H

#include "Wm4FoundationLIB.h"
#include "Wm4Query3TRational.h"
#include "Wm4Vector3.h"

namespace Wm4
{

template <class Real>
class Query3Filtered : public Query3<Real>
{
public:
    // The base class handles floating-point queries.  Each query involves
    // comparing a determinant to zero.  If the determinant is sufficiently
    // close to zero, numerical round-off errors may cause the determinant
    // sign to be misclassified.  To avoid this, the query is repeated with
    // exact rational arithmetic.  You specify the closeness to zero for the
    // switch to rational arithmetic via fUncertainty, a value in the
    // interval [0,1].  The uncertainty of 0 causes the class to behave
    // as if it were Query3.  The uncertainty of 1 causes the class to
    // behave as if it were Query3TRational.
    Query3Filtered (int iVQuantity, const Vector3<Real>* akVertex,
        Real fUncertainty);
    virtual ~Query3Filtered ();

    // run-time type information
    virtual Query::Type GetType () const;

    // Queries about the relation of a point to various geometric objects.

    virtual int ToPlane (const Vector3<Real>& rkP, int iV0, int iV1, int iV2)
        const;

    virtual int ToCircumsphere (const Vector3<Real>& rkP, int iV0, int iV1,
        int iV2, int iV3) const;

private:
    using Query3<Real>::m_akVertex;

    Query3TRational<Real> m_kRQuery;
    Real m_fUncertainty;
};

#include "Wm4Query3Filtered.inl"

typedef Query3Filtered<float> Query3Filteredf;
typedef Query3Filtered<double> Query3Filteredd;

}

#endif
