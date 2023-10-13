// PlcRework.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//
/*
#include "PlcIncludeAll.h"
#include "PlcSample.h"

PlcStdIO* io;
PlcMachine machine;

class Example : public PlcProcess
{
public:
	Example()
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
} example;

void setup() {
	printStream.begin(9600);
	io = new PlcStdIO(&printStream);
	example.registerIO(io);
	machine.registerProcess(&example);
}

void loop() {
	machine.tick();
}

*/



