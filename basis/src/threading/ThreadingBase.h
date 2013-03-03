#pragma once

namespace mania
{
enum threading_return_code
{
	thread_wait_obj0 = 0,
	thread_wait_input,
	thread_wait_abandoned,
	thread_wait_timeout,
	thread_wait_failed
};
}
#include <queue>
#include "critical_section.h"
#include "event.h"
#include "mutex.h"
#include "task.h"
#include "taskqueuemanager.h"
#include "Thread.h"
#include "semaphore.h"
#include "synchronizer.h"
