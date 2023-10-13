#include "PlcProcess.h"
#include <math.h>
double powin(double k, Int32 p) {
	return pow(k, p);
}
double powout(double k, Int32 p) {
	return 1 - pow(1 - k, p);
}
double powinout(double k, Int32 p) {
	k *= 2;
	if (k < 1)
		return 0.5 * pow(k, p);
	return 1 - 0.5 * fabs(pow(2 - k, p));
}
double rampInterpolate(double k, ramp_mode m) {
	if (k == 0 || k == 1)
		return k;
	double a, p, s;
	switch (m) {
	case QUADRATIC_IN:
		return powin(k, 2);
	case QUADRATIC_OUT:
		return powout(k, 2);
	case QUADRATIC_INOUT:
		return powinout(k, 2);
	case CUBIC_IN:
		return powin(k, 3);
	case CUBIC_OUT:
		return powout(k, 3);
	case CUBIC_INOUT:
		return powinout(k, 3);
	case QUARTIC_IN:
		return powin(k, 4);
	case QUARTIC_OUT:
		return powout(k, 4);
	case QUARTIC_INOUT:
		return powinout(k, 4);
	case QUInt32IC_IN:
		return powin(k, 5);
	case QUInt32IC_OUT:
		return powout(k, 5);
	case QUInt32IC_INOUT:
		return powinout(k, 5);
	case SINUSOIDAL_IN:
		return 1 - cos(k * (M_PI / 2));
	case SINUSOIDAL_OUT:
		return sin(k * (M_PI / 2));
	case SINUSOIDAL_INOUT:
		return -0.5 * (cos(M_PI * k) - 1);
	case EXPONENTIAL_IN:
		return pow(2, 10 * (k - 1));
	case EXPONENTIAL_OUT:
		return (1 - pow(2, -10 * k));
	case EXPONENTIAL_INOUT:
		k *= 2.;
		if (k < 1)
			return 0.5 * pow(2, 10 * (k - 1));
		k--;
		return 0.5 * (2 - pow(2, -10 * k));
	case CIRCULAR_IN:
		return -(sqrt(1 - k * k) - 1);
	case CIRCULAR_OUT:
		k--;
		return sqrt(1 - k * k);
	case CIRCULAR_INOUT:
		k *= 2;
		if (k < 1)
			return -0.5 * (sqrt(1 - k * k) - 1);
		k -= 2;
		return 0.5 * (sqrt(1 - k * k) + 1);
	case ELASTIC_IN:
		k -= 1;
		a = 1;
		p = 0.3 * 1.5;
		s = p * asin(1 / a) / (2 * M_PI);
		return -a * pow(2, 10 * k) * sin((k - s) * (2 * M_PI) / p);
	case ELASTIC_OUT:
		a = 1;
		p = 0.3;
		s = p * asin(1 / a) / (2 * M_PI);
		return (a * pow(2, -10 * k) * sin((k - s) * (2 * M_PI) / p) + 1);
	case ELASTIC_INOUT:
		k = k * 2 - 1;
		a = 1;
		p = 0.3 * 1.5;
		s = p * asin(1 / a) / (2 * M_PI);
		if ((k + 1) < 1)
			return -0.5 * a * pow(2, 10 * k) * sin((k - s) * (2 * M_PI) / p);
		return a * pow(2, -10 * k) * sin((k - s) * (2 * M_PI) / p) * 0.5 + 1;
	case BACK_IN:
		s = 1.70158;
		return k * k * ((s + 1) * k - s);
	case BACK_OUT:
		k--;
		s = 1.70158;
		return k * k * ((s + 1) * k + s) + 1;
	case BACK_INOUT:
		k *= 2;
		s = 1.70158;
		s *= 1.525;
		if (k < 1)
			return 0.5 * k * k * ((s + 1) * k - s);
		k -= 2;
		return 0.5 * k * k * ((s + 1) * k + s) + 1;
	case BOUNCE_IN:
		return 1 - rampInterpolate(1 - k, BOUNCE_OUT);
	case BOUNCE_OUT:
		if (k < (1 / 2.75))
			return 7.5625 * k * k;
		if (k < (2 / 2.75)) {
			k -= 1.5 / 2.75;
			return 7.5625 * k * k + 0.75;
		}
		if (k < (2.5 / 2.75)) {
			k -= (2.25 / 2.75);
			return 7.5625 * k * k + 0.9375;
		}
		k -= (2.625 / 2.75);
		return 7.5625 * k * k + 0.984375;
	case BOUNCE_INOUT:
		if (k < 0.5) {
			return rampInterpolate(k * 2, BOUNCE_IN) * 0.5;
		}
		return rampInterpolate(k * 2 - 1, BOUNCE_OUT) * 0.5 + 0.5;
		break;
	case DISCRETE:
		return 0.0;
		break;
	case LINEAR:
	default:
		return k;
	}
}
unsigned char* PlcProcess::_use(Int32 size) {
	unsigned char* temp;
	temp = dataPos;
	dataPos += size;
	return temp;
}
Int32 PlcProcess::sequence(bool running, Int32 identifier)
{
	use(SeqnenceData, data);
	if (init)
	{
		data->status = 0;
	}
	if (running == false)
	{
		data->status = 0;
	}
	if (data->status == 0)
	{
		PlcSeqStep plcSeqStep = getNextSequenceStep(identifier, PlcFirst);
		data->from = plcSeqStep.value;
		data->fromDuration = plcSeqStep.duration;
		data->fromInterpolation = plcSeqStep.interpolation;
		plcSeqStep = getNextSequenceStep(identifier, PlcNext);
		data->to = plcSeqStep.value;
		data->toDuration = plcSeqStep.duration;
		data->toInterpolation = plcSeqStep.interpolation;
		RampData* rampData = ramp(running, data->from, data->to, data->fromDuration, data->fromInterpolation);
		data->status = 1;
		return rampData->value;
	}
	if (data->status == 1)
	{
		RampData* rampData = ramp(running, data->from, data->to, data->fromDuration, data->fromInterpolation);
		if (rampData->status == 2)
		{
			rampData->status = rampBegin;
			data->status = 2;
		}
		return rampData->value;
	}
	if (data->status == 2)
	{
		data->from = data->to;
		data->fromDuration = data->toDuration;
		data->fromInterpolation = data->toInterpolation;
		PlcSeqStep plcSeqStep = getNextSequenceStep(identifier, PlcNext);
		data->to = plcSeqStep.value;
		data->toDuration = plcSeqStep.duration;
		data->toInterpolation = plcSeqStep.interpolation;
		RampData* rampData = ramp(running, data->from, data->to, data->fromDuration, data->fromInterpolation);
		data->status = 1;
		return rampData->value;
	}
	return 0;
}
RampData* PlcProcess::ramp(bool running, Int32 from, Int32 to, Int32 duration, ramp_mode mode)
{
	use(RampData, data);
	if (init) {
		data->status = 0;
		data->value = from;
	}
	if (running == false)
	{
		data->status = rampBegin;
	}
	if (data->status == rampBegin)
	{
		data->startTime = *timestamp;
		data->status = rampRunning;
		data->value = to >= from ? (from + (to - from) * rampInterpolate(0.0, mode)) : (from - (from - to) * rampInterpolate(0.0, mode));;
		return data;
	}
	if (data->status == rampRunning)
	{
		Int32 delta = *timestamp - data->startTime;
		if (duration > 0 and from != to) {
			double k = delta / (double)duration;
			data->value = to >= from ? (from + (to - from) * rampInterpolate(k, mode)) : (from - (from - to) * rampInterpolate(k, mode));
		}
		else {
			data->value = to >= from ? (from + (to - from) * rampInterpolate(1.0, mode)) : (from - (from - to) * rampInterpolate(1.0, mode));
			//data->value = to;
		}
		if (delta >= duration)
		{
			data->status = rampEnd;
		}
		return data;
	}
	if (data->status == rampEnd)
	{
		data->value = to >= from ? (from + (to - from) * rampInterpolate(1.0, mode)) : (from - (from - to) * rampInterpolate(1.0, mode));;
	}
	return data;
}
bool PlcProcess::fall(bool trigger) {

	use(SimpleData, data);
	if (init) {
		data->outputValue = false;
		data->status = 2;
		return false;


	}
	if (data->status == 0 && !trigger) {
		data->status = 1;
		data->outputValue = true;
		return data->outputValue;
	}
	if (data->status == 1) {
		data->status = 2;
		data->outputValue = false;
		return data->outputValue;
	}
	if (data->status == 2 && trigger == true) {
		data->status = 0;
	}
	return data->outputValue;
	
}
bool PlcProcess::rise(bool trigger) {
	use(SimpleData, data);
	if (init) {
		data->outputValue = false;
		data->status = 0;
		return false;
		
		
	}
	if (data->status == 0 && trigger) {
		data->status = 1;
		data->outputValue = true;
		return data->outputValue;
	}
	if (data->status == 1) {
		data->status = 2;
		data->outputValue = false;
		return data->outputValue;
	}
	if (data->status == 2 && trigger == false) {
		data->status = 0;
	}
	return data->outputValue;
}
bool PlcProcess::rs(bool set, bool reset) {
	use(SimpleData, data);
	if (init) {
		data->status = 0;
		data->outputValue = false;
	}
	if (reset) data->outputValue = false;
	if (set) data->outputValue = true;
	return data->outputValue;
}
Int32 PlcProcess::ctu(bool trigger, bool reset, Int32 pv, bool& q) {
	use(CountData, data);
	if (init) {
		data->status = 0;
		data->counter = 0;
	}
	if (reset) {
		data->status = 0;
		data->counter = 0;
		data->q = false;
		q = data->q;
		return data->counter;
	}
	if (data->status == 0 && trigger) {
		data->status = 1;
		if (data->counter >= pv) {
			data->q = true;
		}
		else {
			data->counter++;
			data->q = false;
		}
		q = data->q;
		return data->counter;
	}
	if (data->status == 1 && trigger == false) {
		data->status = 0;
	}
	q = data->q;
	return data->counter;
}
bool PlcProcess::sr(bool set, bool reset) {
	use(SimpleData, data);
	if (init) {
		data->status = 0;
		data->outputValue = false;
	}
	if (set) data->outputValue = true;
	if (reset) data->outputValue = false;
	return data->outputValue;
}
// Ausschaltverzögerung
bool PlcProcess::tof(bool trigger, Int32 duration) {
	use(ClockData, data);
	if (init) {
		data->interval = duration;
		data->nextTime = 0;
		data->outputValue = false;
		data->status = 0;
	}
	if (data->status == 0 && trigger) {
		data->status = 1;
		data->outputValue = true;
		return data->outputValue;
	}
	if (data->status == 1 && trigger == false) {
		data->nextTime = *timestamp + duration;
		data->status = 2;
		return data->outputValue;
	}
	if (data->status == 2) {
		if (trigger == true) {
			data->status = 0;
			data->outputValue = true;
			return data->outputValue;
		}
		if (trigger == false && *timestamp >= data->nextTime) {
			data->status = 0;
			data->outputValue = false;
			return data->outputValue;
		}
	}
	return data->outputValue;
}
// Einschaltverzögerung
bool PlcProcess::ton(bool trigger, Int32 duration) {
	use(ClockData, data);
	if (init) {
		data->interval = duration;
		data->nextTime = 0;
		data->outputValue = false;
		data->status = 0;
	}
	if (data->status == 0 && trigger) {
		data->nextTime = *timestamp + duration;
		data->outputValue = false;
		data->status = 1;
		return data->outputValue;
	}
	if (data->status == 1) {
		if (trigger == false) {
			data->status = 0;
			data->outputValue = false;
			return data->outputValue;
		}
		if (trigger && *timestamp >= data->nextTime) {
			return data->outputValue = true;
		}
	}
	return data->outputValue;
}
/// Impulsbildung
bool PlcProcess::tp(bool trigger, Int32 duration) {
	use(ClockData, data);
	if (init) {
		data->interval = duration;
		data->nextTime = 0;
		data->outputValue = false;
		data->status = 0;
	}
	if (trigger && data->status == 0) {
		data->nextTime = *timestamp + duration;
		data->outputValue = true;
		data->status = 1;
		return data->outputValue;
	}
	if (data->status == 1 && *timestamp >= data->nextTime) {
		data->outputValue = false;
		data->status = 0;
		return data->outputValue;
	}
	return data->outputValue;
}
bool PlcProcess::clock(Int32 interval) {
	use(ClockData, data);
	if (init) {
		data->interval = interval;
		data->nextTime = 0;
		data->outputValue = false;
	}
	if (interval != data->interval)
	{
		data->nextTime = *timestamp;
		data->interval = interval;
	}
	if (*timestamp >= data->nextTime) {
		data->nextTime += interval / 2;
		data->outputValue = !data->outputValue;
	}
	return data->outputValue;
}
bool PlcProcess::gateBool(bool open, bool value)
{
	use(SimpleData, data);
	if (init)
	{
		data->b01 = false;
	}
	if (open)
	{
		data->b01 = value;
	}
	return data->b01;
}
Int32 PlcProcess::gate(bool open, Int32 value)
{
	use(CountData, data);
	if (init)
	{
		data->counter = 0;
	}
	if (open)
	{
		data->counter = value;
	}
	return data->counter;
}
Int32 PlcProcess::stopwatch(bool run)
{
	use(ClockData, data);
	if (init)
	{
		data->status = 0;
		data->interval = 0;
		data->nextTime = 0;
	}
	if (data->status == 0 && run)
	{
		data->nextTime = *timestamp;
		data->interval = 0;
		data->status = 1;
		return data->interval;
	}
	if (data->status == 1)
	{
		if (run)
		{
			data->interval = *timestamp - data->nextTime;
		}
		else
		{
			data->status = 0;
		}
	}
	return  data->interval;
}
