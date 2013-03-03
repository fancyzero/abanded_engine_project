#pragma once

#ifdef FOR_SWIG
#define MANIA_ENGINE_API
#else
	#ifdef ENGINE_DLL_EXPORT
		#define MANIA_ENGINE_API __declspec(dllexport)
	#else
		#define MANIA_ENGINE_API __declspec(dllimport)
	#endif
#endif


#include "..\..\basis\src\basis.h"
#include "plugins\plugin.h"
#include "plugins\renderdevice\renderdevice_plugin.h"
#include "plugins\inputdevice\inputdevice_plugin.h"
#include "core.h"
#include "renderingthread.h"
#include "renderingtask.h"
#include "mesh.h"
#include "staticmesh.h"
#include "skeletalmesh.h"
#include "sg\scenegraph.h"
#include "renderable\mesh_renderable.h"
#include "console.h"
#include "camera.h"