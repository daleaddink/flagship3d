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
template <class T>
TImage3D<T>::TImage3D (int iXBound, int iYBound, int iZBound, T* atData)
    :
    TImage<T>(3)
{
    int* aiBound = WM4_NEW int[3];
    aiBound[0] = iXBound;
    aiBound[1] = iYBound;
    aiBound[2] = iZBound;
    this->SetBounds(aiBound);
    SetData(atData);
}
//----------------------------------------------------------------------------
template <class T>
TImage3D<T>::TImage3D (const TImage3D& rkImage)
    :
    TImage<T>(rkImage)
{
}
//----------------------------------------------------------------------------
template <class T>
TImage3D<T>::TImage3D (const char* acFilename)
    :
    TImage<T>(acFilename)
{
}
//----------------------------------------------------------------------------
template <class T>
T& TImage3D<T>::operator() (int iX, int iY, int iZ) const
{
    // assert:  x < bound[0] && y < bound[1] && z < bound[2]
    return m_atData[iX + this->m_aiBound[0]*(iY + this->m_aiBound[1]*iZ)];
}
//----------------------------------------------------------------------------
template <class T>
int TImage3D<T>::GetIndex (int iX, int iY, int iZ) const
{
    // assert:  x < bound[0] && y < bound[1] && z < bound[2]
    return iX + this->m_aiBound[0]*(iY + this->m_aiBound[1]*iZ);
}
//----------------------------------------------------------------------------
template <class T>
void TImage3D<T>::GetCoordinates (int iIndex, int& riX, int& riY, int& riZ)
    const
{
    // assert:  iIndex < m_iQuantity
    riX = iIndex % this->m_aiBound[0];
    iIndex /= this->m_aiBound[0];
    riY = iIndex % this->m_aiBound[1];
    riZ = iIndex / this->m_aiBound[1];
}
//----------------------------------------------------------------------------
template <class T>
TImage3D<T>& TImage3D<T>::operator= (const TImage3D& rkImage)
{
    return (TImage3D<T>&) TImage<T>::operator=(rkImage);
}
//----------------------------------------------------------------------------
template <class T>
TImage3D<T>& TImage3D<T>::operator= (T tValue)
{
    return (TImage3D<T>&) TImage<T>::operator=(tValue);
}
//----------------------------------------------------------------------------
