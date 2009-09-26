#ifndef _DATADEFINE_H_
#define _DATADEFINE_H_

namespace Flagship
{
	template <class T>
	struct SharedData
	{
		T         m_pData[DATACENTER_CACHE];
		T         m_kStore;
        int       m_iIndex;

		SharedData()
		{
			ZeroMemory( m_pData, DATACENTER_CACHE * sizeof(T) );
			m_iIndex = 0;
		}

		void    Write( T& rData )
		{
			int iNewIndex = m_iIndex == DATACENTER_CACHE - 1 ? 0 : m_iIndex + 1;
			m_pData[iNewIndex] = rData;
			m_iIndex = iNewIndex;
		}

		T *    Read()
		{
			return &( m_pData[m_iIndex] );			
		}

		void   Set()
		{
			m_kStore = m_pData[m_iIndex];
		}

		T *    Get()
		{
			return &m_kStore;
		}
	};

	template <class T>
	struct SharedArrayData
	{
		T *         m_pArrayData[DATACENTER_CACHE];
		T *         m_pStore;
		int         m_iIndex;

		SharedArrayData( int iArraySize )
		{
			for ( int i = 0; i < DATACENTER_CACHE; i++ )
			{
				m_pArrayData[i] = new T[iArraySize];
				ZeroMemory( m_pArrayData[i], iArraySize * sizeof(T) );
			}
			m_pStore = new T[iArraySize];

			m_iIndex = 0;
		}

		~SharedArrayData()
		{
			for ( int i = 0; i < DATACENTER_CACHE; i++ )
			{
				delete [] m_pArrayData[i];
			}
		}

		void    Write( T * pData, int iNum )
		{
			int iNewIndex = m_iIndex == DATACENTER_CACHE - 1 ? 0 : m_iIndex + 1;
			for ( int i = 0; i < iNum; i++ )
			{
				m_pArrayData[iNewIndex][i] = pData[i];
			}
			m_iIndex = iNewIndex;
		}

		T *    Read()
		{
			return m_pArrayData[m_iIndex];
		}

		void   Set( int iNum )
		{
			for ( int i = 0; i < iNum; i++ )
			{
				m_pStore[i] = m_pArrayData[m_iIndex][i];
			}
		}

		T *    Get()
		{
			return m_pStore;
		}
	};

	typedef SharedData<int> IntData;
	typedef SharedData<float> FloatData;
	typedef SharedData<Vector4f> VectorData;
	typedef SharedData<Matrix4f> MatrixData;
	typedef SharedArrayData<Matrix4f> MatrixArrayData;

	struct RenderableData 
	{
		enum
		{
			Matrix4_World       = 0,

			Renderable_Max      = 1,
		};

		void *    m_pData[ Renderable_Max ];

		void *    operator new( size_t size )
		{
			void * p = malloc( ( Renderable_Max + 1 ) * sizeof(void *) );
			return p;
		}

		RenderableData()
		{
			m_pData[ Matrix4_World ] = new MatrixData;
			m_pData[ Renderable_Max ] = NULL;
		}

		void    SetData()
		{
			( (MatrixData *) m_pData[ Matrix4_World ] )->Set();
		}
	};

	struct SceneNodeData 
	{
		enum
		{
			Matrix4_World       = 0,

			SceneNode_Max       = 1,
		};

		void *    m_pData[ SceneNode_Max ];

		void *    operator new( size_t size )
		{
			void * p = malloc( ( SceneNode_Max + 1 ) * sizeof(void *) );
			return p;
		}

		SceneNodeData()
		{
			m_pData[ Matrix4_World ] = new MatrixData;
			m_pData[ SceneNode_Max ] = NULL;
		}

		void    SetData()
		{
			( (MatrixData *) m_pData[ Matrix4_World ] )->Set();
		}
    };

	struct SectorNodeData 
	{
		enum
		{
			Matrix4_World       = 0,

			SectorNode_Max      = 1,
		};

		void *    m_pData[ SectorNode_Max ];

		void *    operator new( size_t size )
		{
			void * p = malloc( ( SectorNode_Max + 1 ) * sizeof(void *) );
			return p;
		}

		SectorNodeData()
		{
			m_pData[ Matrix4_World ] = new MatrixData;
			m_pData[ SectorNode_Max ] = NULL;
		}

		void    SetData()
		{
			( (MatrixData *) m_pData[ Matrix4_World ] )->Set();
		}
	};

	struct EntityData 
	{
		enum
		{
			Matrix4_World       = 0,

			Entity_Max          = 1,
		};

		void *    m_pData[ Entity_Max ];

		void *    operator new( size_t size )
		{
			void * p = malloc( ( Entity_Max + 1 ) * sizeof(void *) );
			return p;
		}

		EntityData()
		{
			m_pData[ Matrix4_World ] = new MatrixData;
			m_pData[ Entity_Max ] = NULL;
		}

		void    SetData()
		{
			( (MatrixData *) m_pData[ Matrix4_World ] )->Set();
		}
	};

	struct AnimEntityData 
	{
		enum
		{
			Matrix4_World       = 0,
			Matrix4_Skinned     = 1,
			Int_BoneNum         = 2,

			AnimEntity_Max      = 3,
		};

		void *    m_pData[ AnimEntity_Max ];

		void *    operator new( size_t size )
		{
			void * p = malloc( ( AnimEntity_Max + 1 ) * sizeof(void *) );
			return p;
		}

		AnimEntityData()
		{
			m_pData[ Matrix4_World ] = new MatrixData;
			m_pData[ Matrix4_Skinned ] = new MatrixArrayData(256);
			m_pData[ Int_BoneNum ] = new IntData;
			m_pData[ AnimEntity_Max ] = NULL;
		}

		void    SetData()
		{
			( (MatrixData *) m_pData[ Matrix4_World ] )->Set();
			( (MatrixArrayData *) m_pData[ Matrix4_Skinned ] )->Set( 256 );
			( (IntData *) m_pData[ Int_BoneNum ] )->Set();
		}
	};

	struct LightData 
	{
		enum
		{
			Matrix4_World           = 0,
			Vector4_Color           = 1,
			Vector4_Position        = 2,
			Vector4_Direction       = 3,
			Vector4_Attenuation     = 4,
			Float_Phi               = 5,

			Light_Max               = 6,
		};

		void *    m_pData[ Light_Max ];

		void *    operator new( size_t size )
		{
			void * p = malloc( ( Light_Max + 1 ) * sizeof(void *) );
			return p;
		}

		LightData()
		{
			m_pData[ Matrix4_World ] = new MatrixData;
			m_pData[ Vector4_Color ] = new VectorData;
			m_pData[ Vector4_Position ] = new VectorData;
			m_pData[ Vector4_Direction ] = new VectorData;
			m_pData[ Vector4_Attenuation ] = new VectorData;
			m_pData[ Float_Phi ] = new FloatData;
			m_pData[ Light_Max ] = NULL;
		}

		void    SetData()
		{
			( (MatrixData *) m_pData[ Matrix4_World ] )->Set();
			( (VectorData *) m_pData[ Vector4_Color ] )->Set();
			( (VectorData *) m_pData[ Vector4_Position ] )->Set();
			( (VectorData *) m_pData[ Vector4_Direction ] )->Set();
			( (VectorData *) m_pData[ Vector4_Attenuation ] )->Set();
			( (FloatData *) m_pData[ Float_Phi ] )->Set();
		}
	};

	struct PortalData 
	{
		enum
		{
			Matrix4_World       = 0,

			Portal_Max          = 1,
		};

		void *    m_pData[ Portal_Max ];

		void *    operator new( size_t size )
		{
			void * p = malloc( ( Portal_Max + 1 ) * sizeof(void *) );
			return p;
		}

		PortalData()
		{
			m_pData[ Matrix4_World ] = new MatrixData;
			m_pData[ Portal_Max ] = NULL;
		}

		void    SetData()
		{
			( (MatrixData *) m_pData[ Matrix4_World ] )->Set();
		}
	};

	struct CameraData
	{
		enum
		{
			Vector4_Position    = 0,
			Vector4_Direction   = 1,
			Vector4_Up          = 2,
			Vector4_Right       = 3,
			Vector4_Param       = 4,
			Matrix4_View        = 5,
			Matrix4_Proj        = 6,

			Camera_Max          = 7,
		};

		void *    m_pData[ Camera_Max ];

		void *    operator new( size_t size )
		{
			void * p = malloc( ( Camera_Max + 1 ) * sizeof(void *) );
			return p;
		}

		CameraData()
		{
			m_pData[ Vector4_Position ] = new VectorData;
			m_pData[ Vector4_Direction ] = new VectorData;
			m_pData[ Vector4_Up ] = new VectorData;
			m_pData[ Vector4_Right ] = new VectorData;
			m_pData[ Vector4_Param ] = new VectorData;
			m_pData[ Matrix4_View ] = new MatrixData;
			m_pData[ Matrix4_Proj ] = new MatrixData;
			m_pData[ Camera_Max ] = NULL;
		}

		void    SetData()
		{
			( (VectorData *) m_pData[ Vector4_Position ] )->Set();
			( (VectorData *) m_pData[ Vector4_Direction ] )->Set();
			( (VectorData *) m_pData[ Vector4_Up ] )->Set();
			( (VectorData *) m_pData[ Vector4_Right ] )->Set();
			( (VectorData *) m_pData[ Vector4_Param ] )->Set();
			( (MatrixData *) m_pData[ Matrix4_View ] )->Set();
			( (MatrixData *) m_pData[ Matrix4_Proj ] )->Set();
		}
	};
}

#endif