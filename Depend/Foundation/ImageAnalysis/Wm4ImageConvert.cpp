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
#include "Wm4Element.h"
#include "Wm4ImageConvert.h"
using namespace Wm4;

//----------------------------------------------------------------------------
#define CONVERTER(SrcType,TrgType) \
static void SrcType##To##TrgType (int iQuantity, void* pvSrc, void* pvTrg) \
{ \
    SrcType* ptSrc = (SrcType*)pvSrc; \
    TrgType* ptTrg = (TrgType*)pvTrg; \
    for (int i = 0; i < iQuantity; i++, ptSrc++, ptTrg++) \
    { \
        *ptTrg = (TrgType)(*ptSrc); \
    } \
}
//----------------------------------------------------------------------------
CONVERTER(char,uchar)
CONVERTER(char,short)
CONVERTER(char,ushort)
CONVERTER(char,int)
CONVERTER(char,uint)
CONVERTER(char,long)
CONVERTER(char,ulong)
CONVERTER(char,float)
CONVERTER(char,double)
CONVERTER(char,rgb5)
CONVERTER(char,rgb8)

CONVERTER(uchar,char)
CONVERTER(uchar,short)
CONVERTER(uchar,ushort)
CONVERTER(uchar,int)
CONVERTER(uchar,uint)
CONVERTER(uchar,long)
CONVERTER(uchar,ulong)
CONVERTER(uchar,float)
CONVERTER(uchar,double)
CONVERTER(uchar,rgb5)
CONVERTER(uchar,rgb8)

CONVERTER(short,char)
CONVERTER(short,uchar)
CONVERTER(short,ushort)
CONVERTER(short,int)
CONVERTER(short,uint)
CONVERTER(short,long)
CONVERTER(short,ulong)
CONVERTER(short,float)
CONVERTER(short,double)
CONVERTER(short,rgb5)
CONVERTER(short,rgb8)

CONVERTER(ushort,char)
CONVERTER(ushort,uchar)
CONVERTER(ushort,short)
CONVERTER(ushort,int)
CONVERTER(ushort,uint)
CONVERTER(ushort,long)
CONVERTER(ushort,ulong)
CONVERTER(ushort,float)
CONVERTER(ushort,double)
CONVERTER(ushort,rgb5)
CONVERTER(ushort,rgb8)

CONVERTER(int,char)
CONVERTER(int,uchar)
CONVERTER(int,short)
CONVERTER(int,ushort)
CONVERTER(int,uint)
CONVERTER(int,long)
CONVERTER(int,ulong)
CONVERTER(int,float)
CONVERTER(int,double)
CONVERTER(int,rgb5)
CONVERTER(int,rgb8)

CONVERTER(uint,char)
CONVERTER(uint,uchar)
CONVERTER(uint,short)
CONVERTER(uint,ushort)
CONVERTER(uint,int)
CONVERTER(uint,long)
CONVERTER(uint,ulong)
CONVERTER(uint,float)
CONVERTER(uint,double)
CONVERTER(uint,rgb5)
CONVERTER(uint,rgb8)

CONVERTER(long,char)
CONVERTER(long,uchar)
CONVERTER(long,short)
CONVERTER(long,ushort)
CONVERTER(long,int)
CONVERTER(long,uint)
CONVERTER(long,ulong)
CONVERTER(long,float)
CONVERTER(long,double)
CONVERTER(long,rgb5)
CONVERTER(long,rgb8)

CONVERTER(ulong,char)
CONVERTER(ulong,uchar)
CONVERTER(ulong,short)
CONVERTER(ulong,ushort)
CONVERTER(ulong,int)
CONVERTER(ulong,uint)
CONVERTER(ulong,long)
CONVERTER(ulong,float)
CONVERTER(ulong,double)
CONVERTER(ulong,rgb5)
CONVERTER(ulong,rgb8)

CONVERTER(float,char)
CONVERTER(float,uchar)
CONVERTER(float,short)
CONVERTER(float,ushort)
CONVERTER(float,int)
CONVERTER(float,uint)
CONVERTER(float,long)
CONVERTER(float,ulong)
CONVERTER(float,double)
CONVERTER(float,rgb5)
CONVERTER(float,rgb8)

CONVERTER(double,char)
CONVERTER(double,uchar)
CONVERTER(double,short)
CONVERTER(double,ushort)
CONVERTER(double,int)
CONVERTER(double,uint)
CONVERTER(double,long)
CONVERTER(double,ulong)
CONVERTER(double,float)
CONVERTER(double,rgb5)
CONVERTER(double,rgb8)

CONVERTER(rgb5,char)
CONVERTER(rgb5,uchar)
CONVERTER(rgb5,short)
CONVERTER(rgb5,ushort)
CONVERTER(rgb5,int)
CONVERTER(rgb5,uint)
CONVERTER(rgb5,long)
CONVERTER(rgb5,ulong)
CONVERTER(rgb5,float)
CONVERTER(rgb5,double)
CONVERTER(rgb5,rgb8)

CONVERTER(rgb8,char)
CONVERTER(rgb8,uchar)
CONVERTER(rgb8,short)
CONVERTER(rgb8,ushort)
CONVERTER(rgb8,int)
CONVERTER(rgb8,uint)
CONVERTER(rgb8,long)
CONVERTER(rgb8,ulong)
CONVERTER(rgb8,float)
CONVERTER(rgb8,double)
CONVERTER(rgb8,rgb5)
//----------------------------------------------------------------------------
#define ENTRY(SrcType,TrgType) SrcType##To##TrgType
//----------------------------------------------------------------------------
typedef void (*Converter)(int,void*,void*);
static Converter gs_aaoConvert[WM4_ELEMENT_QUANTITY][WM4_ELEMENT_QUANTITY] =
{
    { 0,
      ENTRY(char,uchar),
      ENTRY(char,short),
      ENTRY(char,ushort),
      ENTRY(char,int),
      ENTRY(char,uint),
      ENTRY(char,long),
      ENTRY(char,ulong),
      ENTRY(char,float),
      ENTRY(char,double),
      ENTRY(char,rgb5),
      ENTRY(char,rgb8) },

    { ENTRY(uchar,char),
      0,
      ENTRY(uchar,short),
      ENTRY(uchar,ushort),
      ENTRY(uchar,int),
      ENTRY(uchar,uint),
      ENTRY(uchar,long),
      ENTRY(uchar,ulong),
      ENTRY(uchar,float),
      ENTRY(uchar,double),
      ENTRY(uchar,rgb5),
      ENTRY(uchar,rgb8) },

    { ENTRY(short,char),
      ENTRY(short,uchar),
      0,
      ENTRY(short,ushort),
      ENTRY(short,int),
      ENTRY(short,uint),
      ENTRY(short,long),
      ENTRY(short,ulong),
      ENTRY(short,float),
      ENTRY(short,double),
      ENTRY(short,rgb5),
      ENTRY(short,rgb8) },

    { ENTRY(ushort,char),
      ENTRY(ushort,uchar),
      ENTRY(ushort,short),
      0,
      ENTRY(ushort,int),
      ENTRY(ushort,uint),
      ENTRY(ushort,long),
      ENTRY(ushort,ulong),
      ENTRY(ushort,float),
      ENTRY(ushort,double),
      ENTRY(ushort,rgb5),
      ENTRY(ushort,rgb8) },

    { ENTRY(int,char),
      ENTRY(int,uchar),
      ENTRY(int,short),
      ENTRY(int,ushort),
      0,
      ENTRY(int,uint),
      ENTRY(int,long),
      ENTRY(int,ulong),
      ENTRY(int,float),
      ENTRY(int,double),
      ENTRY(int,rgb5),
      ENTRY(int,rgb8) },

    { ENTRY(uint,char),
      ENTRY(uint,uchar),
      ENTRY(uint,short),
      ENTRY(uint,ushort),
      ENTRY(uint,int),
      0,
      ENTRY(uint,long),
      ENTRY(uint,ulong),
      ENTRY(uint,float),
      ENTRY(uint,double),
      ENTRY(uint,rgb5),
      ENTRY(uint,rgb8) },

    { ENTRY(long,char),
      ENTRY(long,uchar),
      ENTRY(long,short),
      ENTRY(long,ushort),
      ENTRY(long,int),
      ENTRY(long,uint),
      0,
      ENTRY(long,ulong),
      ENTRY(long,float),
      ENTRY(long,double),
      ENTRY(long,rgb5),
      ENTRY(long,rgb8) },

    { ENTRY(ulong,char),
      ENTRY(ulong,uchar),
      ENTRY(ulong,short),
      ENTRY(ulong,ushort),
      ENTRY(ulong,int),
      ENTRY(ulong,uint),
      ENTRY(ulong,long),
      0,
      ENTRY(ulong,float),
      ENTRY(ulong,double),
      ENTRY(ulong,rgb5),
      ENTRY(ulong,rgb8) },

    { ENTRY(float,char),
      ENTRY(float,uchar),
      ENTRY(float,short),
      ENTRY(float,ushort),
      ENTRY(float,int),
      ENTRY(float,uint),
      ENTRY(float,long),
      ENTRY(float,ulong),
      0,
      ENTRY(float,double),
      ENTRY(float,rgb5),
      ENTRY(float,rgb8) },

    { ENTRY(double,char),
      ENTRY(double,uchar),
      ENTRY(double,short),
      ENTRY(double,ushort),
      ENTRY(double,int),
      ENTRY(double,uint),
      ENTRY(double,long),
      ENTRY(double,ulong),
      ENTRY(double,float),
      0,
      ENTRY(double,rgb5),
      ENTRY(double,rgb8) },

    { ENTRY(rgb5,char),
      ENTRY(rgb5,uchar),
      ENTRY(rgb5,short),
      ENTRY(rgb5,ushort),
      ENTRY(rgb5,int),
      ENTRY(rgb5,uint),
      ENTRY(rgb5,long),
      ENTRY(rgb5,ulong),
      ENTRY(rgb5,float),
      ENTRY(rgb5,double),
      0,
      ENTRY(rgb5,rgb8) },

    { ENTRY(rgb8,char),
      ENTRY(rgb8,uchar),
      ENTRY(rgb8,short),
      ENTRY(rgb8,ushort),
      ENTRY(rgb8,int),
      ENTRY(rgb8,uint),
      ENTRY(rgb8,long),
      ENTRY(rgb8,ulong),
      ENTRY(rgb8,float),
      ENTRY(rgb8,double),
      ENTRY(rgb8,rgb5),
      0 }
};
//----------------------------------------------------------------------------
void Wm4::ImageConvert (int iQuantity, int iSrcRTTI, void* pvSrcData,
    int iTrgRTTI, void* pvTrgData)
{
    assert(iSrcRTTI < WM4_ELEMENT_QUANTITY
        && iTrgRTTI < WM4_ELEMENT_QUANTITY);

    Converter oConverter = gs_aaoConvert[iSrcRTTI][iTrgRTTI];
    oConverter(iQuantity,pvSrcData,pvTrgData);
}
//----------------------------------------------------------------------------
