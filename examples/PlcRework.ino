// PlcRework.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//
#include "PlcIncludeAll.h"
#include "PlcSample.h"

PlcStdIO* io;
PlcMachine machine;


class Example : public PlcProcess
{
public:
	enum commands {  run , stop  , list,last};
	int btn1;
	int stopwatchTempValue;
	int measuredTime;
	int clockRun;
	int clockTick;
	int clockResult;
	void onRegisterIO(PlcIO* io)
	{	
		io->addCommands(command, last,  "run", "stop","list");	
		io->add(btn1, "btn1");
		io->add(measuredTime, "measured time");
		io->add(clockTick, "clock tick");
	}
	Example()
	{	
		clockTick = 0; clockRun = 0; measuredTime = 0;
	}
	void onMessage(PlcIO* plcIO, PlcAdapter* adapter, int index)
	{
		if (adapter->isCommand() && adapter->origin == true)
		{
			switch (adapter->getIntValue())
			{
			case run:
				clockRun = true;
				this->println("clock start");
				break;
			case stop:
				clockRun = false;
				this->println("clock stop");
				break;
			case list:
				syncInit();
				break;
			}
		}
		int u = 0;
	}
	void onTask()
	{
		/*PLC Node Calls e.g.clock, ... must not be used within if statemnts or loops.
		Only linear sequences are permitted !!!!
		forbidden example:
		 if (condition)
		 {
		    a = clock(1000);
		    b = clock(2000);
		 } 
		*/
		stopwatchTempValue = stopwatch(btn1);
		if (fall(btn1))
		{
			measuredTime = stopwatchTempValue;
			clockRun = true;
		}
		clockResult=  clock(measuredTime);
		clockTick = clockRun & clockResult;
	}
} example;
void setup() {
	printStream.begin(9600);
	io = new PlcStdIO(&printStream);
	example.registerIO(io);
	machine.registerProcess(&example);
	PlcStream *st;
	PlcBugStream* m;
}
void loop() {
	machine.tick();
}
