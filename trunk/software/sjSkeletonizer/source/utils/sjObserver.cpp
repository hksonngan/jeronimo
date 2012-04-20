#include "sjObserver.h"
#include "sjLog.h"

using namespace sj;

void sjSubject::attach( sjObserver* observer )
{
	std::vector< sjObserver* >::iterator i;
	int contained = 0;
	for( i = observers.begin(); i != observers.end(); ++i )
	{
		if( *i == observer )
		{
			contained = 1;
		}
	}
	if( !contained )
	{
		observers.push_back( observer );
	}
}

void sjSubject::detach( sjObserver* observer )
{
	std::vector< sjObserver* >::iterator i;
	for( i = observers.begin(); i != observers.end(); ++i )
	{
		if( *i == observer )
		{
			observers.erase(i);
			return;
		}
	}
}

void sjSubject::dispatch(sjEvent * evt)
{
	sjLogDebug("sjSubject::dispatch 1\n");
	std::vector< sjObserver* >::iterator i;
	sjLogDebug("sjSubject::dispatch 2\n");
	for( i = observers.begin(); i != observers.end(); ++i )
	{
		sjLogDebug("sjSubject::dispatch 3\n");
		(*i) -> proccesEvent(evt);
		sjLogDebug("sjSubject::dispatch 4\n");
	}
}

void sjSubject::clear(){
	observers.clear();
}

sjObserver * sjSubject::getObserver(int index){
	return observers[index];
}