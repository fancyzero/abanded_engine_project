#pragma once

namespace mania
{
class rendering_thread;
enum erendertask_type
{
	rendertask_nextframe,
	rendertask_normal,
};

class MANIA_ENGINE_API render_task : public task
{
public:
	render_task( rendering_thread* pRenderingThread );
	virtual ~render_task( void );
	virtual DWORD execute() = 0;
protected:
	rendering_thread*	m_rendering_thread;
	erendertask_type	m_type;
};

#define ENQUEUE_RENDER_TASK_BEGIN( type_name )\
	{\
		class type_name : public render_task\
		{\
		public:\
			type_name##( rendering_thread* pRenderingThread )\
					:render_task( pRenderingThread )\
			{\
			}\
			DWORD execute()\
			{
//insert execute code here
#define ENQUEUE_RENDER_TASK_END( type_name )\
	}\
	};\
	type_name##* newtask = new type_name##( mania::get_core()->get_rendering_thread() );\
	mania::get_core()->get_rendering_thread()->AddRenderTask( newtask );\
	}

#define ENQUEUE_RENDER_TASK_BEGIN_1PARAM( type_name, paramtype1, paramname1 )\
	{\
		class type_name : public render_task\
		{\
			paramtype1 paramname1;\
		public:\
			type_name( rendering_thread* pRenderingThread, paramtype1 p1 )\
					:render_task( pRenderingThread )\
			{\
				paramname1 = p1;\
			}\
			DWORD execute()\
			{
//insert execute code here
#define ENQUEUE_RENDER_TASK_END_1PARAM( type_name, paramvalue1 )\
	}\
	};\
	type_name##* newtask = new type_name##( mania::get_core()->get_rendering_thread(), paramvalue1 );\
	mania::get_core()->get_rendering_thread()->AddRenderTask( newtask );	}


#define ENQUEUE_RENDER_TASK_BEGIN_2PARAM( type_name, paramtype1, paramname1, paramtype2, paramname2 )\
	{\
		class type_name : public render_task\
		{\
			paramtype1 paramname1;\
			paramtype2 paramname2;\
		public:\
			type_name( rendering_thread* pRenderingThread, paramtype1 p1, paramtype2 p2 )\
					:render_task( pRenderingThread )\
			{\
				paramname1 = p1;\
				paramname2 = p2;\
			}\
			DWORD execute()\
			{
//insert execute code here
#define ENQUEUE_RENDER_TASK_END_2PARAM( type_name, paramvalue1, paramvalue2 )\
	}\
	};\
	type_name##* newtask = new type_name##( mania::get_core()->get_rendering_thread(), paramvalue1, paramvalue2 );\
	mania::get_core()->get_rendering_thread()->AddRenderTask( newtask );	}

#define ENQUEUE_RENDER_TASK_BEGIN_3PARAM( type_name, paramtype1, paramname1, paramtype2, paramname2, paramtype3, paramname3 )\
	{\
		class type_name : public render_task\
		{\
			paramtype1 paramname1;\
			paramtype2 paramname2;\
			paramtype3 paramname3;\
		public:\
			type_name( rendering_thread* pRenderingThread, paramtype1 p1, paramtype2 p2, paramtype3 p3  )\
					:render_task( pRenderingThread )\
			{\
				paramname1 = p1;\
				paramname2 = p2;\
				paramname3 = p3;\
			}\
			DWORD execute()\
			{
//insert execute code here
#define ENQUEUE_RENDER_TASK_END_3PARAM( type_name, paramvalue1, paramvalue2, paramvalue3 )\
	}\
	};\
	type_name##* newtask = new type_name##( mania::get_core()->get_rendering_thread(), paramvalue1, paramvalue2, paramvalue3 );\
	mania::get_core()->get_rendering_thread()->AddRenderTask( newtask );	}


//useful tasks
class MANIA_ENGINE_API rendertask_create_vertexbuffer : public render_task
{
public:
	rendertask_create_vertexbuffer( rendering_thread* pRenderingThread,
	                                const mania::vertexbuffer::desc_t &vb_desc,
	                                sptr<vertexbuffer>* vb_created,
	                                sptr<buffer> fill_buffer )
			: render_task( pRenderingThread )
	{
		m_fill_buffer = fill_buffer;
		m_vb_desc = vb_desc;
		m_vb_created = vb_created;
	}
	sptr<vertexbuffer>*				m_vb_created;
	vertexbuffer::desc_t					m_vb_desc;
	sptr<buffer>					m_fill_buffer;

	DWORD execute();
};

class MANIA_ENGINE_API rendertask_create_indexbuffer : public render_task
{
public:
	rendertask_create_indexbuffer( rendering_thread* pRenderingThread,
	                               const mania::indexbuffer::desc_t &ib_desc,
	                               sptr<indexbuffer>* ib_created,
	                               sptr<buffer> fill_buffer )
			: render_task( pRenderingThread )
	{
		m_fill_buffer = fill_buffer;
		m_ib_desc = ib_desc;
		m_ib_created = ib_created;
	}
	sptr<indexbuffer>*				m_ib_created;
	indexbuffer::desc_t						m_ib_desc;
	sptr<buffer>					m_fill_buffer;

	DWORD execute();
};

class MANIA_ENGINE_API rendertask_create_vertexdeclearation : public render_task
{
public:
	rendertask_create_vertexdeclearation( rendering_thread* renderingthread,
	                                      const std::vector<vertexelem_def>& ve_def,
	                                      sptr<vertex_declaration>* vd_created )
			: render_task( renderingthread )
	{
		m_ve_def = ve_def;
		m_vd_created = vd_created;
	}

	sptr<vertex_declaration>*	m_vd_created;
	std::vector<vertexelem_def>			m_ve_def;

	DWORD execute();
};
}