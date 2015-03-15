#include <vector>
#include "email.h"
#include <windows.h>
using namespace std;
#pragma once

class mailbox
{
public:
	vector<email> inbox,outbox;
	CRITICAL_SECTION incr,outcr;
	mailbox(void);
	~mailbox(void);
};
