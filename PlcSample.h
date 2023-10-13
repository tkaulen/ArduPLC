#pragma once

#include "PlcIncludeAll.h"

class Test : public PlcProcess
{
public:
	Test()
	{
		a = 0; b = 0; r = 0;
	}
	int a;
	int b;
	int r;
	int c;
	void onRegisterIO(PlcIO* io)
	{
		//io->add(a, "a");
		//io->add(b, "b");
		io->add(r, "r");
		io->add(c, "c");
	}
	void onTask()
	{
		a = clock(1000);
		b = clock(2000);

		if (r) c = a;



	}
};
