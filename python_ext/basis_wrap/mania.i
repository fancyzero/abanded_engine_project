/* File : example.i */

%module mania
%{
namespace mania{};
using namespace mania;
#include "..\\..\\basis\\src\\stdafx.h"
#include "..\\..\\engine\\src\\stdafx.h"
%}
%typemap(in) HWND  {
    $1 = (HWND) ((int)PyInt_AsLong($input));
}
%typemap(in) HINSTANCE  {
    $1 = (HINSTANCE) ((int)PyInt_AsLong($input));
}


%rename(_assign) *::operator=;
%rename(_float) *::operator float*; 
%rename(_kfloat) *::operator const float*; 
%rename(_DWORD) *::operator DWORD; 
%rename(_kDWORD) *::operator const DWORD; 
%rename(_vector3) *::operator mania::math::vector3;
#ignore(g_pCore)
//basis
#define FOR_SWIG
#define MANIA_BASIS_API
#define MANIA_ENGINE_API

%include "..\\..\\basis\\src\\stdafx.h"
%include "..\\..\\basis\\src\\threading\\critical_section.h"
%include "..\\..\\basis\\src\\math\\math.h"
%include "..\\..\\basis\\src\\basic\\classinfo.h"
%include "..\\..\\basis\\src\\basic\\ref_counted.h"
%include "..\\..\\basis\\src\\basic\object.h"
//engine
%include "..\\..\\engine\\src\\stdafx.h"
%include "..\\..\\engine\\src\\engine.h"
%include "..\\..\\engine\\src\\core.h"
%include "..\\..\\engine\\src\\sg\node.h"
%include "..\\..\\engine\\src\\sg\tnode.h"
%include "..\\..\\engine\\src\\sg\renderable.h"
%include "..\\..\\engine\\src\\sg\scene_renderer.h"
%include "..\\..\\engine\\src\\sg\transform.h"
%include "..\\..\\engine\\src\\camera.h"

%extend mania::math::vector3 
{
   char *__str__() 
   {
       static char tmp[1024];
       sprintf(tmp,"vector3(%g,%g,%g)", $self->x,$self->y,$self->z);
       return tmp;
   }
}

%extend mania::transform 
{
   char *__str__() 
   {
       static char tmp[1024];
	   sprintf(tmp,"trans:[ %g %g %g ]  rot:[ %g %g %g ] scale:[ %g %g %g ]", 
		   $self->get_translation().x,$self->get_translation().y,$self->get_translation().z,
		   $self->get_rotation().x,$self->get_rotation().y,$self->get_rotation().z,
		   $self->get_scale().x,$self->get_scale().y,$self->get_scale().z  );
       return tmp;
   }
}

