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
#include "Wm4Matrix4.h"
using namespace Wm4;

template<> const Matrix4<float> Matrix4<float>::ZERO(
    0.0f,0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,0.0f);
template<> const Matrix4<float> Matrix4<float>::IDENTITY(
    1.0f,0.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,
    0.0f,0.0f,0.0f,1.0f);

template<> const Matrix4<double> Matrix4<double>::ZERO(
    0.0,0.0,0.0,0.0,
    0.0,0.0,0.0,0.0,
    0.0,0.0,0.0,0.0,
    0.0,0.0,0.0,0.0);
template<> const Matrix4<double> Matrix4<double>::IDENTITY(
    1.0,0.0,0.0,0.0,
    0.0,1.0,0.0,0.0,
    0.0,0.0,1.0,0.0,
    0.0,0.0,0.0,1.0);
