#pragma once



#include "PlcAdapter.h"




#include "PlcIO.h"

#define use(T, V) T *V = (T *)_use(sizeof(T));






#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))
enum ramp_mode {
	NONE = 0x00,
	LINEAR = 0x01,
	QUADRATIC_IN = 0x02,
	QUADRATIC_OUT = 0x03,
	QUADRATIC_INOUT = 0x04,
	CUBIC_IN = 0x05,
	CUBIC_OUT = 0x06,
	CUBIC_INOUT = 0x07,
	QUARTIC_IN = 0x08,
	QUARTIC_OUT = 0x09,
	QUARTIC_INOUT = 0x0A,
	QUInt32IC_IN = 0x0B,
	QUInt32IC_OUT = 0x0C,
	QUInt32IC_INOUT = 0x0D,
	SINUSOIDAL_IN = 0x0E,
	SINUSOIDAL_OUT = 0x0F,
	SINUSOIDAL_INOUT = 0x10,
	EXPONENTIAL_IN = 0x11,
	EXPONENTIAL_OUT = 0x12,
	EXPONENTIAL_INOUT = 0x13,
	CIRCULAR_IN = 0x14,
	CIRCULAR_OUT = 0x15,
	CIRCULAR_INOUT = 0x16,
	ELASTIC_IN = 0x17,
	ELASTIC_OUT = 0x18,
	ELASTIC_INOUT = 0x19,
	BACK_IN = 0x1A,
	BACK_OUT = 0x1B,
	BACK_INOUT = 0x1C,
	BOUNCE_IN = 0x1D,
	BOUNCE_OUT = 0x1E,
	BOUNCE_INOUT = 0x1F,
	DISCRETE = 0x20
};

typedef struct PlcSeqStep
{
	Int32 value;
	Int32 duration;
	ramp_mode interpolation;
} PlcSeqStep;

typedef struct PlcSequence
{
	PlcSeqStep* plcSeqSteps;
	Int32 seqStep;
	Int32 length;
} PlcSequence;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
double powin(double k, Int32 p);
double powout(double k, Int32 p);
double powinout(double k, Int32 p);
double rampInterpolate(double k, ramp_mode m);


typedef struct Combi
{
	Int32 i;
	bool b00 : 1;
	bool b01 : 1;
	bool b02 : 1;
	bool b03 : 1;
} Combi;

typedef struct ClockData
{
	Int32 nextTime;
	Int32 interval;
	unsigned char status;
	bool outputValue : 1;
	bool b01 : 1;
	bool b02 : 1;
	bool b03 : 1;
} ClockData;

typedef struct RampData
{
	unsigned char status;
	Int32 startTime;
	Int32 value;
} RampData;

typedef struct SeqnenceData
{
	Int32 from;
	Int32 to;
	Int32 fromDuration;
	Int32 toDuration;
	ramp_mode fromInterpolation;
	ramp_mode toInterpolation;
	Int32 value;
	Int32 nextTime;
	unsigned char status;
} SeqnenceData;

typedef struct SimpleData
{
	bool outputValue : 1;
	bool b01 : 1;
	bool b02 : 1;
	bool b03 : 1;
	unsigned char status;
} SimpleData;

typedef struct CountData
{
	unsigned char status;
	bool q : 1;
	Int32 counter;
} CountData;

#define rampBegin 0
#define rampRunning 1
#define rampEnd 2

#define PlcFirst -1
#define PlcNext -2
#define PlcPrev -3

#define PlcStep 0
#define PlcLinear 1





class PlcProcess : public PlcPrint
{
public:
	bool running;
	PlcIO* first;
	PlcIO* last;
	PlcProcess* next;
	Int32* timestamp;
	bool init;
	int priority;
	unsigned char* _data;
	unsigned char* dataPos;
	int maxData;
	int command;
	Int32 nextMillis;

	PlcProcess(int maxData = 500)
	{
		priority = 0;
		nextMillis = millis();
		
		
		this->maxData = maxData;
		this->_data = (unsigned char*)malloc(maxData);
		this->running = true;
		this->first = NULL;
		this->last = NULL;
		this->next = NULL;
		this->init = true;
		this->dataPos = NULL;
		memset(_data ,0, maxData);
	}

	void remap(int& oldRef, int& newRef)
	{

		for (PlcIO* plcIO = this->first; plcIO != NULL; plcIO = plcIO->next)
		{
			plcIO->remap(oldRef, newRef);

		}


	}

	void remap(double& oldRef, double& newRef)
	{

		for (PlcIO* plcIO = this->first; plcIO != NULL; plcIO = plcIO->next)
		{
			plcIO->remap(oldRef, newRef);

		}


	}

	size_t write(uint8_t c)
	{
		for (PlcIO* plcIO = this->first; plcIO != NULL; plcIO = plcIO->next)
		{
			plcIO->write(c);;
			
		}
		return 0;


	}

	size_t write(const uint8_t* buffer, size_t size)
	{
		for (PlcIO* plcIO = this->first; plcIO != NULL; plcIO = plcIO->next)
		{
			plcIO->write(buffer,size);

		}
		return 0;
		

	}

	int availableForWrite() { return 0; }

	void flush()
	{


	}

	

	PlcSeqStep getNextSequenceStep(Int32 id, Int32 index)
	{
		PlcSeqStep plcSeqStep;
		return plcSeqStep;
	}

	virtual void onMessage(PlcIO *plcIO, PlcAdapter* adapter,int index)
	{
		


	}
	void test();
	void registerIO(PlcIO* newPlcIO, bool syncModeSliced = true, int priority =0)
	{
		newPlcIO->plcProcess = this;
		newPlcIO->priority = priority;
		newPlcIO->syncModeSliced = syncModeSliced;
		newPlcIO->nextMillis = millis() + newPlcIO->priority;
		if (first == NULL && last == NULL)
		{
			first = newPlcIO; last = newPlcIO;
		}
		else
		{
			last->next = newPlcIO;
			last = newPlcIO;
		}
		onRegisterIO(newPlcIO);
	}



	

	void syncInit()
	{
		
		for (PlcIO* plcIO = this->first; plcIO != NULL; plcIO = plcIO->next)
		{
			plcIO->syncInit();
			plcIO->onTask();
		}


	}
	void syncIOS()
	{
		for (PlcIO* plcIO = this->first; plcIO != NULL; plcIO = plcIO->next)
		{

			if (*timestamp >= plcIO->nextMillis)
			{
				plcIO->nextMillis += plcIO->priority;
				plcIO->syncRead();
				
			}
			
			plcIO->onTask();
		}
	}
	void tick()
	{
		if (running)
		{
			dataPos = _data;
			onTask();
			syncIOS();
			init = false;
		}

	}


	virtual void onRegisterIO(PlcIO* io)
	{


	}

	virtual void onTask()
	{


	}

	Int32 sequence(bool running, Int32 identifier);
	RampData* ramp(bool running, Int32 from, Int32 to, Int32 duration, ramp_mode mode);


	bool fall(bool trigger);
	bool rise(bool trigger);

	unsigned char* _use(Int32 size);


	
	// speichernde Einschaltverzögerung
	/*bool ston(bool trigger,unsigned Int32 duration)
  {
	use(clockData, data);
	  if (init) {
		data->Int32erval = duration;
		data->nextTime = 0;
		data->outputValue = false;
		data->status =0;
	  }
	  if (data->status ==0 && trigger)
	  {
		data->nextTime = timestamp +duration;
		data->outputValue = false;
		data->status = 1;
		return data->outputValue;
	  }
	  if (data->status == 1)
	  {
		if (trigger == false)
		{
		  data->status =0;
		  data->outputValue = false;
		  return data->outputValue;
		}
		if (trigger && timestamp >= data->nextTime)
		{
		  return data->outputValue = true;
		}
	  }
   return data->outputValue;
  }*/
	bool rs(bool set, bool reset);
	Int32 ctu(bool trigger, bool reset, Int32 pv, bool& q);
	bool sr(bool set, bool reset);
	// Ausschaltverzögerung
	bool tof(bool trigger, Int32 duration);
	// Einschaltverzögerung
	bool ton(bool trigger, Int32 duration);
	/// Impulsbildung
	bool tp(bool trigger, Int32 duration);
	bool clock(Int32 interval);
	bool gateBool(bool open, bool value);
	Int32 gate(bool open, Int32 value);
	Int32 stopwatch(bool run);
};
class PlcMachine
{
public:
	PlcProcess* first;
	PlcProcess* last;
	Int32 timestamp;
	PlcMachine()
	{
		first = NULL;
		last = NULL;
		timestamp = 0;
	}
	void registerProcess(PlcProcess* plcProcess, Int32 priority=0)
	{
		plcProcess->timestamp = &timestamp;
		if (first == NULL && last == NULL)
		{
			first = plcProcess; last = plcProcess;
		}
		else
		{
			last->next = plcProcess;
			last = plcProcess;
		}
		plcProcess->syncInit();
		plcProcess->priority = priority;
		
		plcProcess->nextMillis = millis()+ plcProcess->priority;
	}
	void tick()
	{
		this->timestamp = millis();
		for (PlcProcess* plcProcess = this->first; plcProcess != NULL; plcProcess = plcProcess->next)
		{		
			if (timestamp >= plcProcess->nextMillis)
			{
				plcProcess->nextMillis += plcProcess->priority;
				plcProcess->tick();
			}
			
		}
	}
};




