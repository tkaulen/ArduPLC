#pragma once




#include "PlcPlatform.h"
#include "PlcAdapter.h"

#include "PlcIO.h"
#include "PlcProcess.h"

#include "PlcStdIO.h"


#include "PlcPrintStream.h"




void PlcIO::changedTop(PlcAdapter* adapter, int index)
{
	if (plcProcess != NULL) 	plcProcess->onMessage(this,adapter, index);



}

/*
void PlcAdapter::bla(PlcProcess* p)
{

	process->init = true;


}*/





#ifdef _arduino_

#define printStream Serial
#define printStream0 Serial0
#define printStream1 Serial1
#define printStream2 Serial2

#endif


#ifdef _windows_

void setup();
void loop();

PlcPrintStream printStream;
PlcPrintStream printStream0;
PlcPrintStream printStream1;
PlcPrintStream printStream2;

int main()
{
	setup();
	while (true)
	{
		loop();
	}
}

#endif





