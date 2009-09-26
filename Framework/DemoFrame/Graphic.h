#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include <Common/Prerequisites.h>
#include <Common/LogManager.h>

//--------Main--------

// Basic
#include "../../Graphic/Main/BoundingBox.h"
#include "../../Graphic/Main/BoundingSphere.h"
#include "../../Graphic/Main/Camera.h"
#include "../../Graphic/Main/Entity.h"
#include "../../Graphic/Main/Material.h"
#include "../../Graphic/Main/SceneCuller.h"
#include "../../Graphic/Main/LightCuller.h"
#include "../../Graphic/Main/RenderTargetQueue.h"
#include "../../Graphic/Main/PostProcessQueue.h"
#include "../../Graphic/Main/RenderTexture.h"
#include "../../Graphic/Main/RenderWindow.h"
#include "../../Graphic/Main/Resource.h"
#include "../../Graphic/Main/SceneManager.h"
#include "../../Graphic/Main/SceneNode.h"
#include "../../Graphic/Main/Buffer.h"
#include "../../Graphic/Main/Renderer.h"

// Entity
#include "../../Graphic/Main/Light.h"
#include "../../Graphic/Main/AnimEntity.h"

// Resource
#include "../../Graphic/Main/Effect.h"
#include "../../Graphic/Main/Mesh.h"
#include "../../Graphic/Main/Texture.h"
#include "../../Graphic/Main/Skeleton.h"
#include "../../Graphic/Main/Animation.h"
#include "../../Graphic/Main/TerrainTile.h"

//--------Material--------

// Direct3D9
#include "../../Graphic/Material/D3D9Basic.h"
#include "../../Graphic/Material/D3D9SkyBox.h"
#include "../../Graphic/Material/D3D9Metal.h"
#include "../../Graphic/Material/D3D9Glass.h"
#include "../../Graphic/Material/D3D9Satin.h"
#include "../../Graphic/Material/D3D9ThinFilm.h"
#include "../../Graphic/Material/D3D9Terrain.h"
#include "../../Graphic/Material/D3D9HDR.h"

#ifdef FLAGSHIP_DX10
// Direct3D10
#include "../../Graphic/Material/D3D10SkyBox.h"
#include "../../Graphic/Material/D3D10Metal.h"
#include "../../Graphic/Material/D3D10Glass.h"
#include "../../Graphic/Material/D3D10Satin.h"
#include "../../Graphic/Material/D3D10ThinFilm.h"
#include "../../Graphic/Material/D3D10HDR.h"
#endif

//--------Renderer--------

// Direct3D9
#include "../../Graphic/Renderer/D3D9Prerequisites.h"
#include "../../Graphic/Renderer/D3D9Effect.h"
#include "../../Graphic/Renderer/D3D9XMesh.h"
#include "../../Graphic/Renderer/D3D9PMFMesh.h"
#include "../../Graphic/Renderer/D3D9TerrainTile.h"
#include "../../Graphic/Renderer/D3D9Renderer.h"
#include "../../Graphic/Renderer/D3D9RenderTexture.h"
#include "../../Graphic/Renderer/D3D9RenderWindow.h"
#include "../../Graphic/Renderer/D3D9Texture.h"
#include "../../Graphic/Renderer/D3D9CubeTexture.h"

#ifdef FLAGSHIP_DX10
// Direct3D10
#include "../../Graphic/Renderer/D3D10Prerequisites.h"
#include "../../Graphic/Renderer/D3D10Effect.h"
#include "../../Graphic/Renderer/D3D10PMFMesh.h"
#include "../../Graphic/Renderer/D3D10SDKMesh.h"
#include "../../Graphic/Renderer/D3D10Renderer.h"
#include "../../Graphic/Renderer/D3D10RenderTexture.h"
#include "../../Graphic/Renderer/D3D10RenderWindow.h"
#include "../../Graphic/Renderer/D3D10Texture.h"
#include "../../Graphic/Renderer/D3D10CubeTexture.h"
#endif

#endif