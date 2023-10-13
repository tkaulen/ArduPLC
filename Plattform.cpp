#include "PlcPlatform.h"



#ifdef _windows_
auto start1 = high_resolution_clock::now();
int millis()
{
	//	auto start1 = high_resolution_clock::now();
	auto stop1 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop1 - start1);
	Int32 timestamp = duration.count();
	return timestamp / 1000.0;
}
#endif