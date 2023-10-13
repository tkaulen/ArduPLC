#pragma once

//#include "PlcAdapter.h"

class PlcBasis
{
public:

	//PlcAdapter *adds;

	//virtual bool onChanged(PlcAdapter* plcAdapter, int index)=0;
	

	virtual bool onChanged( int index)
	{
		return false;


	}


};
