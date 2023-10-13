#pragma once
#include "PlcStream.h"
#include "PlcPlatform.h"
#include "PlcNonBlockingParser.h"
class PlcAdapter
{
public:
	const char* name;
	PlcAdapter* next;
	bool reject:1;
	bool force : 1;
	bool autoForce : 1;
	bool readOnly : 1;
	bool origin : 1;
	PlcAdapter()
	{
		name = NULL;
		reject = false;
		force = false;
		next = NULL;
		origin = false;
		readOnly = false;
		autoForce = false;
	}
	virtual bool isCommand()
	{
		return false;
	}
	PlcAdapter* setReadOnly()
	{
		readOnly = true;
		return this;
	}
	PlcAdapter* setAutoForce()
	{
		autoForce = true;
		return this;
	}
	//void (*resetFunc)() = 0;
	virtual bool transferRead()
	{
		return false;
	}
	virtual void transferWrite()
	{
	}
	virtual void setValue(int value)
	{
	}
	virtual int getIntValue()
	{
		return 0;
	}

	virtual bool referencesTo(int &value)
	{
		return false;
	}

	virtual bool referencesTo(double& value)
	{
		return false;
	}

	virtual void remap(int& value)
	{
		
	}

	virtual void remap(double& value)
	{

	}



	virtual void setValue(double value)
	{
	}
	virtual double getDoubleValue()
	{
		return 0;
	}
	/*
	virtual void getStringValue(char* buffer, uint8_t base = 10)
	{
	}*/
	/*
	virtual int setIntValue(const char* svalue, char ignore)
	{
		return 0;
	}*/
	virtual void toStream(PlcStream * stream, uint8_t base = 10)
	{
	}
	/*virtual int fromStream(PlcStream* stream, char ignore)
	{
		return 0;
	}*/
	virtual PlcAdapter* clone()
	{
		return NULL;
	}
	virtual void beginParse(PlcNonBlockingParser* parser)
	{
		parser->beginParse();
	}
	virtual int endParse(PlcNonBlockingParser* parser)
	{
		this->origin = true;
		return 0;
		//parser->endParse();
	}
	bool parseToken(PlcNonBlockingParser* parser,PlcStream* stream)
	{
		bool result = parser->parseToken(stream);
		if (result == true)
		{
			endParse(parser);
		}
		return result;
	}
};
class PlcNumberAdapter : public PlcAdapter
{
public:
};


class PlcIntAdapter : public PlcAdapter
{
public:
	int value;
	int* linkedValue;
	PlcIntAdapter() : PlcAdapter()
	{
		linkedValue = NULL;
		value = 0;
	}

	bool referencesTo(int& value)
	{
		return (linkedValue == &value);
		
	}

	void remap(int& value)
	{
		linkedValue = &value;
		value = value;
		this->force = true;

	}
	PlcAdapter* clone()
	{
		PlcIntAdapter* newAdapter = new PlcIntAdapter();
		newAdapter->value = this->value;
		newAdapter->linkedValue = this->linkedValue;
		newAdapter->name = this->name;
		return newAdapter;
	}
	bool transferRead()
	{
		if (value != *linkedValue )
		{
			value = *linkedValue;
			return true;
		}
		return false;
	}
	void transferWrite()
	{
		*linkedValue = value;
	}
	void setValue(int value)
	{
		this->value = value;
		*linkedValue = value;
	}
	int getIntValue()
	{
		return value;
	}
	void toStream(PlcStream* stream, uint8_t base = 10)
	{
		stream->print(value);
	}
	  int endParse(PlcNonBlockingParser* parser)
	{
		  if (this->readOnly) return 0;
	
		*this->linkedValue = parser->getInt();
		this->origin = true;
		if (this->autoForce) this->force = true;

		return 0;
	}
};


class PlcDoubleAdapter : public PlcAdapter
{
public:
	double value;
	double* linkedValue;
	PlcDoubleAdapter() : PlcAdapter()
	{
		linkedValue = NULL;
		value = 0;
	}
	bool referencesTo(double& value)
	{
		return (linkedValue == &value);
	}

	void remap(double& value)
	{
		linkedValue = &value;
		value = value;
		this->force = true;

	}


	PlcAdapter* clone()
	{
		PlcDoubleAdapter* newAdapter = new PlcDoubleAdapter();
		newAdapter->value = this->value;
		newAdapter->linkedValue = this->linkedValue;
		newAdapter->name = this->name;
		return newAdapter;
	}
	bool transferRead()
	{
		if (value != *linkedValue)
		{
			value = *linkedValue;
			return true;
		}
		return false;
	}
	void transferWrite()
	{
		*linkedValue = value;
	}
	void setValue(int value)
	{
		this->value = value;
		*linkedValue = value;
	}
	int getIntValue()
	{
		return value;
	}

	 void setValue(double value)
	{
		 this->value = value;
		 *linkedValue = value;
	}
	 double getDoubleValue()
	{
		return value;
	}
	void toStream(PlcStream* stream, uint8_t base = 10)
	{
		stream->print(value);
	}
	int endParse(PlcNonBlockingParser* parser)
	{
		if (this->readOnly) return 0;
		
		*this->linkedValue = parser->getDouble();
		this->origin = true;
		if (this->autoForce) this->force = true;
		
		return 0;
	}
};
class PlcEnumAdapter : public PlcIntAdapter
{
public:
	 const char** enums;
	int maxEnums;
	PlcEnumAdapter(int maxEnums=3) : PlcIntAdapter() 
	{
		this->maxEnums = maxEnums;
		enums = (const char**)malloc(sizeof(const char**) * maxEnums);
	}
	void toStream(PlcStream* stream, uint8_t base = 10)
	{
		if (value >= 0 && value < this->maxEnums)
		{
			stream->print(enums[value]);
		}
	}
	int endParse(PlcNonBlockingParser* parser)
	{
		if (this->readOnly) return 0;
		for (int i = 0; i < this->maxEnums; i++)
		{
			if (strcmp(enums[i], parser->symbol) == 0)
			{
				*this->linkedValue = i;
				this->origin = true;
				if (this->autoForce) this->force = true;
				return 0;
			}
		}
		int intValue = parser->getInt();
		if (intValue >= 0 && intValue < this->maxEnums)
		{
			*this->linkedValue = intValue;
			this->origin = true;
			if (this->autoForce) this->force = true;
		}
		return 0;
	}
};
class PlcCommandAdapter : public PlcEnumAdapter
{
public:
	PlcCommandAdapter(int maxEnums = 3) : PlcEnumAdapter(maxEnums)
	{
		autoForce = true;
	}
	int endParse(PlcNonBlockingParser* parser)
	{
		if (this->readOnly) return 0;
		for (int i = 0; i < this->maxEnums; i++)
		{
			if (strcmp(enums[i], parser->symbol) == 0)
			{
			    if (this->autoForce) this->force = true;
				*this->linkedValue = i;
				this->origin = true;
				return 0;
			}
		}
		return 0;
	}
	bool isCommand()
	{
		return true;
	}
};
