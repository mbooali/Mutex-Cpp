#include <iostream>
#include <time.h>
using namespace std;
#pragma once

class email
{
public:
	string textmail;
	int senderID;
	int receiverID;
	time_t timeofsend;
	email(void);
	~email(void);
};
