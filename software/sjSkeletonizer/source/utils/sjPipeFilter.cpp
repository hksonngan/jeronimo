#include "sjPipeFilter.h"

using namespace sj;
/*
template<class T, class B>
sjPF<T, B>::sjProducer::sjProducer()
:output_pipe(NULL)
{
}
*/
/*template<class T, class B>
void sjPF<T, B>::sjProducer::setOutputPipe(sjPipe * p){
	output_pipe = p;
}*/

/*template<class T, class B>
sjPF<T, B>::sjConsumer::sjConsumer()
: input_pipe(NULL)
{
}*/

template<class T, class B>
void sjPF<T, B>::sjConsumer::setInputPipe(sjPipe * p){
	input_pipe = p;
}

template<class T, class B>
void sjPF<T, B>::sjSink::notify()
{
	assert(this->input_pipe != NULL);
	while(this->input_pipe->size() >0){
		this->packets.push_back(this->input_pipe->read());
	}
	consume();
}

template<class T, class B>
void sjPF<T, B>::sjSource::update() 
{
	assert(this->output_pipe != NULL);
	output_pipe->write( produce());
}

/*template<class T, class B>
void sjPF<T, B>::sjFilter::notify() 
{
	assert(this->input_pipe != NULL);
	update();
}*/

/*template<class T, class B>
void sjPF<T, B>::sjFilter::update()
{
	assert(this->output_pipe != NULL);
	transform();
}*/

template<class T, class B>
sjPF<T, B>::sjPipe::sjPipe()
:ouput_consumer(NULL)
{
}

/*template<class T, class B>
T * sjPF<T, B>::sjPipe::read() 
{
	assert(size() != 0);
	T * p = *( this->packets.begin());
	this->packets.pop_back();
	return p;
}*/

template<class T, class B>
void sjPF<T, B>::sjPipe::write(T * p)
{
	this->packets.push_back(p);
	assert(this->ouput_consumer != NULL);
	this->ouput_consumer->notify();
}

template<class T, class B>
int sjPF<T, B>::sjPipe::size()
{
	return this->packets.size();
}

template<class T, class B>
void sjPF<T, B>::sjPipe::setOuputConsumer(sjConsumer * consumer)
{
	this->ouput_consumer  = consumer;
}
