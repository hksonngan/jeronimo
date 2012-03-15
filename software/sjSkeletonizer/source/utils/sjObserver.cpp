#include "sjObserver.h"

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
	printf("sjSubject::dispatch 1\n");
	std::vector< sjObserver* >::iterator i;
	printf("sjSubject::dispatch 2\n");
	for( i = observers.begin(); i != observers.end(); ++i )
	{
		printf("sjSubject::dispatch 3\n");
		(*i) -> proccesEvent(evt);
		printf("sjSubject::dispatch 4\n");
	}
}