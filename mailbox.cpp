#include "mailbox.h"

mailbox::mailbox(void)
{
	InitializeCriticalSectionAndSpinCount(&incr,0x80000400);
	InitializeCriticalSectionAndSpinCount(&outcr,0x80000400);
}

mailbox::~mailbox(void)
{
}
