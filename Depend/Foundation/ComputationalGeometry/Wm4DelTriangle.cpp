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
#include "Wm4DelTriangle.h"

namespace Wm4
{
//----------------------------------------------------------------------------
template <class Real>
DelTriangle<Real>::DelTriangle (int iV0, int iV1, int iV2)
{
    V[0] = iV0;
    V[1] = iV1;
    V[2] = iV2;
    A[0] = 0;
    A[1] = 0;
    A[2] = 0;
    Time = -1;
    IsComponent = false;
    OnStack = false;
}
//----------------------------------------------------------------------------
template <class Real>
bool DelTriangle<Real>::IsInsertionComponent (int i, DelTriangle* pkAdj,
    const Query2<Real>* pkQuery, const int* aiSupervertex)
{
    if (i != Time)
    {
        Time = i;

        // Determine the number of vertices in common with the supertriangle.
        // The supertriangle vertices have indices VQ-3, VQ-2, and VQ-1, where
        // VQ is the quantity of input vertices.
        int iCommon = 0, iSVIndex = -1, j;
        for (j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (V[j] == aiSupervertex[k])
                {
                    iCommon++;
                    iSVIndex = j;
                }
            }
        }

        int iRelation;
        if (iCommon == 0)
        {
            // The classic case is that a point is in the mesh formed only by
            // the input vertices, in which case we only test for containment
            // in the circumcircle of the triangle.
            iRelation = pkQuery->ToCircumcircle(i,V[0],V[1],V[2]);
        }
        else
        {
            // The classic problem is that points outside the mesh formed
            // only by the input vertices must be handled from a visibility
            // perspective rather than using circumcircles (compare with
            // convex hull construction).  By not doing this, you can run into
            // the pitfall that has snared many folks--the boundary edges of
            // the final triangulation do not form a convex polygon.
            int iV0, iV1;
            if (iCommon == 1)
            {
                iV0 = V[(iSVIndex+1)%3];
                iV1 = V[(iSVIndex+2)%3];
            }
            else  // iCommon == 2
            {
                for (j = 0; j < 3; j++)
                {
                    if (A[j] != 0 && A[j] != pkAdj)
                    {
                        break;
                    }
                }
                iV0 = V[j];
                iV1 = V[(j+1)%3];
            }
            iRelation = pkQuery->ToLine(i,iV0,iV1);
        }

        IsComponent = (iRelation <= 0 ? true : false);
    }

    return IsComponent;
}
//----------------------------------------------------------------------------
template <class Real>
int DelTriangle<Real>::DetachFrom (int iAdj, DelTriangle* pkAdj)
{
    assert(0 <= iAdj && iAdj < 3 && A[iAdj] == pkAdj);
    A[iAdj] = 0;
    for (int i = 0; i < 3; i++)
    {
        if (pkAdj->A[i] == this)
        {
            pkAdj->A[i] = 0;
            return i;
        }
    }
    return -1;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template WM4_FOUNDATION_ITEM
class DelTriangle<float>;

template WM4_FOUNDATION_ITEM
class DelTriangle<double>;
//----------------------------------------------------------------------------
}
