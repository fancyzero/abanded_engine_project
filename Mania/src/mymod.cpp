#include "StdAfx.h"
#include "mod.h"
#include "mymod.h"
#include "scene.h"

using namespace mania;
mymod::mymod( HINSTANCE ainst, HWND amain_window )
		: mod( ainst, amain_window )
{
}

mymod::~mymod( void )
{
}
sptr<skeletalmesh_renderable> ra;
sptr<skeletalmesh> skm;
gresult mymod::init()
{
	//load scene
	RECT rc;
	rc.left = rc.top = 0;
	rc.right = 1024;
	rc.bottom = 768;
	::AdjustWindowRect( &rc, ::GetWindowLong(m_main_window, GWL_STYLE), FALSE );
	::SetWindowPos( m_main_window, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top, SWP_NOZORDER | SWP_NOMOVE );

	mania::get_core()->init( this->m_instance, this->m_main_window );
	m_renderer = new scene_renderer();
	m_scene = new scene();
	m_scene->get_local_transform().set_identity();
	m_scene->set_name( L"scene root" );
	ra = new skeletalmesh_renderable();
	ra->get_local_transform().set_identity();
	skm = new skeletalmesh();
 	skm->load( L"Stream\\Models\\sey.dae" );
	ra->set_name( L"skeletalmesh" );
	ra->set_mesh( skm );
	m_scene->attach( ra );
	m_camera = construct_object<camera>( L"test camera", m_camera );
	m_scene->attach( m_camera );
	get_core()->set_scene_renderer( this->m_renderer );
	return grc_succeeded;
}

gresult mymod::uninit()
{
	m_scene->detach_children();
	m_camera = NULL;
	ra = NULL;
	skm = NULL;
	m_scene = NULL; 
	return grc_succeeded;
}

gresult mymod::frame()
{
	get_core()->get_inputdevice()->update_state();
	get_core()->BeginFrame();
	get_core()->update();
	//get_core()->execute_script(L"mymod.script_frame()");
	switch ( m_context.m_stage )
	{
	case ms_title:
		title_frame();
		break;
	case ms_gameplay:
		gameplay_frame();
		break;
	default:
		break;
	}
	m_renderer->render_all( m_scene );
	get_core()->EndFrame();
	return grc_succeeded;
}

static DWORD old_tick = GetTickCount();
gresult mymod::title_frame()
{
	matrix44 mat;	
	float lag = (GetTickCount() - old_tick) / 1000.0f;
	old_tick = GetTickCount();
	float t;
	t = GetTickCount()/1000.0f;
	//ra->get_local_transform().set_rotation(vector3(t,0,0));
	//::build_matrix_lookat_lh( &mat, &math::vector3( 0,0,200), &math::vector3(0,0,0), &math::vector3(0,1,0) );
	//m_renderer->set_viewtransform(mat);
	m_scene->update( lag );
	input_state istate;
	get_core()->get_inputdevice()->get_input_state(&istate);
	vector3 vleft(-30,0,0);
	vector3 vfront(0,0,30);
	vector3 vup(0,30,0);
	math::vec_transform( &vleft , &m_camera->get_world_transform().get_matrix() );
	vleft = vleft - m_camera->get_world_transform().get_translation();
	math::vec_transform( &vfront , &m_camera->get_world_transform().get_matrix() );
	vfront = vfront - m_camera->get_world_transform().get_translation();
	math::vec_transform( &vup , &m_camera->get_world_transform().get_matrix() );
	vup = vup - m_camera->get_world_transform().get_translation();
	if ( istate.key_states['A'] & 0x80 )
		m_camera->get_local_transform().set_translation(m_camera->get_local_transform().get_translation() + vleft * lag  );
	if ( istate.key_states['D'] & 0x80 )
		m_camera->get_local_transform().set_translation(m_camera->get_local_transform().get_translation() - vleft * lag  );
	if ( istate.key_states['C'] & 0x80 )
		m_camera->get_local_transform().set_translation(m_camera->get_local_transform().get_translation() + vup * lag  );
	if ( istate.key_states['V'] & 0x80 )
		m_camera->get_local_transform().set_translation(m_camera->get_local_transform().get_translation() - vup * lag  );
	if ( istate.key_states['W'] & 0x80 )
		m_camera->get_local_transform().set_translation(m_camera->get_local_transform().get_translation() + vfront * lag  );
	if ( istate.key_states['S'] & 0x80 )
		m_camera->get_local_transform().set_translation(m_camera->get_local_transform().get_translation() - vfront * lag  );
	math::vector3 rot = m_camera->get_local_transform().get_rotation();

	if ( istate.key_states['Q'] & 0x80 )
		m_camera->get_local_transform().set_rotation( rot.x, rot.y+0.05f, rot.z );
	if ( istate.key_states['E'] & 0x80 )
		m_camera->get_local_transform().set_rotation( rot.x, rot.y-0.05f, rot.z );
	if ( istate.key_states['Z'] & 0x80 )
		m_camera->get_local_transform().set_rotation( rot.x+0.05f, rot.y, rot.z );
	if ( istate.key_states['X'] & 0x80 )
		m_camera->get_local_transform().set_rotation( rot.x-0.05f, rot.y, rot.z );


	build_matrix_perspective_fov_lh( &mat, 60.0f/180.0f*3.1415f, 4.0f/3.0f, 1.0f, 1000.0f );
	m_renderer->set_projtransform( mat );
	return grc_succeeded;
}

gresult mymod::gameplay_frame()
{

	return grc_succeeded;
}