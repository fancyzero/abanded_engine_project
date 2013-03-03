#include "stdafx.h"
#include "..\common.h"
#include "ref_counted.h"

namespace mania
{
void ref_counted::delete_this()
{
	delete this;
}
}