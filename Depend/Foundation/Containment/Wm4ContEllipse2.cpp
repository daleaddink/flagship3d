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
#include "Wm4ContEllipse2.h"
#include "Wm4ApprGaussPointsFit2.h"

namespace Wm4
{
//----------------------------------------------------------------------------
template <class Real>
Ellipse2<Real> ContEllipse (int iQuantity, const Vector2<Real>* akPoint)
{
    // Fit the points with a Gaussian distribution.  The covariance matrix
    // is M = D[0]*U[0]*U[0]^T+D[1]*U[1]*U[1]^T, where D[0] and D[1] are the
    // eigenvalues and U[0] and U[1] are corresponding unit-length
    // eigenvectors.
    Box2<Real> kBox = GaussPointsFit2<Real>(iQuantity,akPoint);
    Real* afD = kBox.Extent;
    Vector2<Real>* akAxis = kBox.Axis;

    // If either eigenvalue is nonpositive, adjust the D[] values so that
    // we actually build an ellipse.
    int i;
    for (i = 0; i < 2; i++)
    {
        if (afD[i] < (Real)0.0)
        {
            afD[i] = -afD[i];
        }
        if (afD[i] < Math<Real>::ZERO_TOLERANCE)
        {
            afD[i] = Math<Real>::ZERO_TOLERANCE;
        }
    }

    // Grow the ellipse, while retaining its shape determined by the
    // covariance matrix, to enclose all the input points.  The quadratic form
    // that is used for the ellipse construction is
    //
    //   Q(X) = (X-C)^T*M*(X-C)
    //        = (X-C)^T*(D[0]*U[0]*U[0]^T+D[1]*U[1]*U[1]^T)*(X-C)
    //        = D[0]*Dot(U[0],X-C)^2 + D[1]*Dot(U[1],X-C)^2
    //
    // If the maximum value of Q(X[i]) for all input points is V^2, then a
    // bounding ellipse is Q(X) = V^2 since Q(X[i]) <= V^2 for all i.

    Real fMaxValue = (Real)0.0;
    for (i = 0; i < iQuantity; i++)
    {
        Vector2<Real> kDiff = akPoint[i] - kBox.Center;
        Real afDot[2] =
        {
            akAxis[0].Dot(kDiff),
            akAxis[1].Dot(kDiff)
        };

        Real fValue = afD[0]*afDot[0]*afDot[0] + afD[1]*afDot[1]*afDot[1];
        if (fValue > fMaxValue)
        {
            fMaxValue = fValue;
        }
    }

    // arrange for quadratic to satisfy Q(X) <= 1
    Real fInv = ((Real)1.0)/fMaxValue;
    for (i = 0; i < 2; i++)
    {
        afD[i] *= fInv;
    }

    Ellipse2<Real> kEllipse;
    kEllipse.Center = kBox.Center;
    for (i = 0; i < 2; i++)
    {
        kEllipse.Axis[i] = akAxis[i];
        kEllipse.Extent[i] = Math<Real>::InvSqrt(afD[i]);
    }
    return kEllipse;
}
//----------------------------------------------------------------------------
template <class Real>
void ProjectEllipse (const Ellipse2<Real>& rkEllipse,
   const Line2<Real>& rkLine, Real& rfMin, Real& rfMax)
{
    // center of projection interval
    Real fCenter = rkLine.Direction.Dot(rkEllipse.Center - rkLine.Origin);

    // radius of projection interval
    Real afTmp[2] =
    {
        rkEllipse.Extent[0]*(rkLine.Direction.Dot(rkEllipse.Axis[0])),
        rkEllipse.Extent[1]*(rkLine.Direction.Dot(rkEllipse.Axis[1]))
    };
    Real fRSqr = afTmp[0]*afTmp[0] + afTmp[1]*afTmp[1];
    Real fRadius = Math<Real>::Sqrt(fRSqr);

    rfMin = fCenter - fRadius;
    rfMax = fCenter + fRadius;
}
//----------------------------------------------------------------------------
template <class Real>
const Ellipse2<Real> MergeEllipses (const Ellipse2<Real>& rkEllipse0,
    const Ellipse2<Real>& rkEllipse1)
{
    Ellipse2<Real> kMerge;

    // compute the average of the input centers
    kMerge.Center = ((Real)0.5)*(rkEllipse0.Center + rkEllipse1.Center);

    // bounding ellipse orientation is average of input orientations
    if (rkEllipse0.Axis[0].Dot(rkEllipse1.Axis[0]) >= (Real)0.0)
    {
        kMerge.Axis[0] = ((Real)0.5)*(rkEllipse0.Axis[0]+rkEllipse1.Axis[0]);
        kMerge.Axis[0].Normalize();
    }
    else
    {
        kMerge.Axis[0] = ((Real)0.5)*(rkEllipse0.Axis[0]-rkEllipse1.Axis[0]);
        kMerge.Axis[0].Normalize();
    }
    kMerge.Axis[1] = -kMerge.Axis[0].Perp();

    // Project the input ellipses onto the axes obtained by the average
    // of the orientations and that go through the center obtained by the
    // average of the centers.
    for (int i = 0; i < 2; i++)
    {
        // projection axis
        Line2<Real> kLine(kMerge.Center,kMerge.Axis[i]);

        // project ellipsoids onto the axis
        Real fMin0, fMax0, fMin1, fMax1;
        ProjectEllipse(rkEllipse0,kLine,fMin0,fMax0);
        ProjectEllipse(rkEllipse1,kLine,fMin1,fMax1);

        // Determine the smallest interval containing the projected
        // intervals.
        Real fMax = (fMax0 >= fMax1 ? fMax0 : fMax1);
        Real fMin = (fMin0 <= fMin1 ? fMin0 : fMin1);

        // Update the average center to be the center of the bounding box
        // defined by the projected intervals.
        kMerge.Center += kLine.Direction*(((Real)0.5)*(fMin+fMax));

        // Compute the extents of the box based on the new center.
        kMerge.Extent[i] = ((Real)0.5)*(fMax-fMin);
    }

    return kMerge;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template WM4_FOUNDATION_ITEM
Ellipse2<float> ContEllipse<float> (int, const Vector2<float>*);

template WM4_FOUNDATION_ITEM
void ProjectEllipse<float> (const Ellipse2<float>&, const Line2<float>&,
    float&, float&);

template WM4_FOUNDATION_ITEM
const Ellipse2<float> MergeEllipses<float> (const Ellipse2<float>&,
    const Ellipse2<float>&);

template WM4_FOUNDATION_ITEM
Ellipse2<double> ContEllipse<double> (int, const Vector2<double>*);

template WM4_FOUNDATION_ITEM
void ProjectEllipse<double> (const Ellipse2<double>&, const Line2<double>&,
    double&, double&);

template WM4_FOUNDATION_ITEM
const Ellipse2<double> MergeEllipses<double> (const Ellipse2<double>&,
    const Ellipse2<double>&);
//----------------------------------------------------------------------------
}
