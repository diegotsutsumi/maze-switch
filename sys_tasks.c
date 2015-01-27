#include "sys_tasks.h"

void SYS_Tasks()
{
	MEM_Tasks();
	UI_Tasks();
	APP_Tasks();
}
