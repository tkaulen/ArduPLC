#pragma once

#include "PlcAdapter.h"
//#include <cstdarg.h>
#include "stdarg.h"


class PlcProcess;
class PlcIO
{
private:
	void addAdapter(PlcAdapter* srcAdapter)
	{
		if (first == NULL && last == NULL)
		{
			first = srcAdapter; last = srcAdapter;
		}
		else
		{
			last->next = srcAdapter;
			last = srcAdapter;
		}
		currentAdapter = srcAdapter;
	}
	PlcAdapter* currentAdapter;
	int currentAdapterIndex;
public:
	PlcAdapter* first;
	PlcAdapter* last;
	PlcIO* next;

	PlcProcess* plcProcess;

	bool syncModeSliced;
	Int32 nextMillis;
	int priority;

	


	
	PlcIO()
	{
		first = NULL;
		last = NULL;
		next = NULL;
		syncModeSliced = true;
		plcProcess = NULL;

		currentAdapter = NULL;
		currentAdapterIndex = 0;
		priority = 0;
		nextMillis = millis();
	}

     


	void remap(int& oldRef, int& newRef)
	{
		for (PlcAdapter* plcAdapter = this->first; plcAdapter != NULL; plcAdapter = plcAdapter->next)
		{
			if (plcAdapter->referencesTo(oldRef))
			{
				plcAdapter->remap(newRef);
			}
		}
	}

	void remap(double& oldRef, double& newRef)
	{
		for (PlcAdapter* plcAdapter = this->first; plcAdapter != NULL; plcAdapter = plcAdapter->next)
		{
			if (plcAdapter->referencesTo(oldRef))
			{
				plcAdapter->remap(newRef);
			}
		}
	}

	virtual size_t write(const uint8_t* buffer, size_t size)
	{
		return 0;
	}
	virtual size_t write(uint8_t c)
	{
		return 0;


	}
	
	PlcAdapter* find(const char* name)
	{
		for (PlcAdapter* plcAdapter = this->first; plcAdapter != NULL; plcAdapter = plcAdapter->next)
		{
			if (strcmp(name, plcAdapter->name) == 0)
			{
				return plcAdapter;
			}
		}
		return NULL;
	}

	PlcAdapter* find(int index)
	{
		int i = 0;
		for (PlcAdapter* plcAdapter = this->first; plcAdapter != NULL; plcAdapter = plcAdapter->next)
		{
			if (i == index)
			{
				return plcAdapter;
			}
			i++;
		}
		return NULL;
	}

	PlcAdapter* importAdapter(PlcIO* srcIO)
	{
		if (srcIO == NULL) return NULL;
		for (PlcAdapter* plcAdapter = this->first; plcAdapter != NULL; plcAdapter = plcAdapter->next)
		{
			this->importAdapter(plcAdapter);
		}
		return NULL;
	}
	PlcAdapter* importAdapter(PlcAdapter* srcAdapter)
	{
		if (srcAdapter == NULL) return NULL;
		PlcAdapter* newAdapter = srcAdapter->clone();
		this->addAdapter(newAdapter);
		return newAdapter;
	}
	PlcAdapter* add(int& ref, const char* name)
	{
		PlcIntAdapter* plcIntAdapter = new PlcIntAdapter();
		plcIntAdapter->linkedValue = &ref;
		plcIntAdapter->name = name;
		addAdapter(plcIntAdapter);
		return plcIntAdapter;
	}

	PlcAdapter* add(double& ref, const char* name)
	{
		PlcDoubleAdapter* plcDoubleAdapter = new PlcDoubleAdapter();
		plcDoubleAdapter->linkedValue = &ref;
		plcDoubleAdapter->name = name;
		addAdapter(plcDoubleAdapter);
		return plcDoubleAdapter;
	}

	PlcEnumAdapter* addEnum(int& ref, const char* name, int maxEnums,...)
	{
		PlcEnumAdapter* plcEnumAdapter = new PlcEnumAdapter(maxEnums);
		plcEnumAdapter->linkedValue = &ref;
		plcEnumAdapter->name = name;
		addAdapter(plcEnumAdapter);

		va_list args;

		va_start(args, maxEnums);

		for (int i = 0; i < maxEnums; i++)
		{
			const char* value = va_arg(args, const char*);
			plcEnumAdapter->enums[i] = value;


		}

		va_end(args);
		return plcEnumAdapter;
	}


	
	PlcCommandAdapter* addCommands(int& ref,  int maxEnums, ...)
	{
		PlcCommandAdapter* plcEnumAdapter = new PlcCommandAdapter(maxEnums);
		plcEnumAdapter->linkedValue = &ref;
		plcEnumAdapter->name = "cmd";
		addAdapter(plcEnumAdapter);

		
		

		va_list args;

		va_start(args, maxEnums);

		for (int i = 0; i < maxEnums; i++)
		{
			const char* value = va_arg(args, const char*);
			plcEnumAdapter->enums[i] = value;


		}

		va_end(args);
		return plcEnumAdapter;
	}


	virtual bool onChanged(PlcAdapter* plcAdapter, int index)
	{
		return false;
	}

	void changedTop(PlcAdapter* adapter,int index);
	
	


	
	void syncRead()
	{
		int i = 0;

		if (syncModeSliced)
		{
			if (currentAdapter != NULL)
			{

				if (currentAdapter->transferRead() || currentAdapter->reject || currentAdapter->force)
				{
					currentAdapter->reject = onChanged(currentAdapter, i);
					changedTop(currentAdapter, i);
					currentAdapter->origin = false;
					currentAdapter->force = false;
				}

				if (currentAdapter->next == NULL)
				{
					currentAdapter = first;
				}
				else
				{
					currentAdapter = currentAdapter->next;
				}

			}
		}
		else
		{

			for (PlcAdapter* plcAdapter = this->first; plcAdapter != NULL; plcAdapter = plcAdapter->next)
			{
				currentAdapter = plcAdapter;

				if (currentAdapter != NULL)
				{

					if (currentAdapter->transferRead() || currentAdapter->reject || currentAdapter->force)
					{
						currentAdapter->reject = onChanged(currentAdapter, i);
						changedTop(currentAdapter, i);
						currentAdapter->origin = false;
						currentAdapter->force = false;
					}

					

				}


				i++;
			}


		}


	}

	void syncInit()
	{

		for (PlcAdapter* plcAdapter = this->first; plcAdapter != NULL; plcAdapter = plcAdapter->next)
		{
			plcAdapter->force = true;
		}
		currentAdapter = this->first;


	}

	void syncWrite()
	{
		for (PlcAdapter* plcAdapter = this->first; plcAdapter != NULL; plcAdapter = plcAdapter->next)
		{
			plcAdapter->transferWrite();
		}
	}

	virtual void onTask()
	{
		int z = 0;


	}
};