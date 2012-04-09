#if defined (WIN32) || defined (_WIN32_)
	#pragma once
#endif

#ifndef __SJPIPEFILTER__H__
#define __SJPIPEFILTER__H__

#include <list>
#include <assert.h>
#include "sjObserver.h"
#include "sjDataType.h"
#include "sjParameter.h"
#include "sjLog.h"

namespace sj{

template<class T, class B> class sjPF{
	public:
		typedef T PolyhedronType;
		typedef B ParametersType;
		class sjPipe;

		class sjProducer{
		public:
			sjProducer():output_pipe(NULL){
			}
			virtual void update() = 0;
			void setOutputPipe(sjPipe * p){
				output_pipe = p;
			}
			sjPipe * getOutputPipe(){
				return output_pipe;
			}
		protected:
			sjPipe * output_pipe;
		};

		class sjConsumer {
		public:
			sjConsumer(): input_pipe(NULL){
			}
			virtual void notify() = 0;
			void setInputPipe(sjPipe * p){
				input_pipe = p;
			}
			sjPipe * getInputPipe(){
				return input_pipe;
			}
		protected:
			sjPipe * input_pipe;
		};

		class sjSink : public sjConsumer {
		public:
			virtual void notify(){
				assert(this->input_pipe != NULL);
				consume(this->input_pipe->read());
			}

			virtual void consume(T) = 0;
		protected:
			T polyhedron;
		};

		class sjSource : public sjProducer {
		public:
			void update(){
				assert(this->output_pipe != NULL);
				output_pipe->write( produce());
			}
			virtual T produce() = 0;
		};

		class sjFilter : public sjConsumer, public sjProducer {
		public:
			void notify(){
				sjLogDebug("sjFilter notify 1\n");
				assert(this->input_pipe != NULL);
				sjLogDebug("sjFilter notify 2\n");
				update();
				sjLogDebug("sjFilter notify 3\n");
			}
			void update(){
				sjLogDebug("sjFilter update 1\n");
				assert(this->output_pipe != NULL);
				sjLogDebug("sjFilter update 2\n");
				this->output_pipe->write( iterate());
				sjLogDebug("sjFilter update 3\n");
			}
			
			virtual void setParameters(B *) = 0;
			virtual void setMesh(T ) = 0;
			virtual size_t getNumberOfVertex() = 0;
			virtual T iterate() = 0;
			virtual T getMesh() = 0;
			virtual B * getParameters() = 0;
		};

		class sjPipe {
		public:
			sjPipe():ouput_consumer(NULL){
			}
			T read(){
				sjLogDebug("sjPipe read 1\n");
				T p =  this->packet;
				sjLogDebug("sjPipe read 2\n");
				return p;
			}
			void write(T p){
				sjLogDebug("sjPipe write 1\n");
				this->packet = p;
				sjLogDebug("sjPipe write 2\n");
				assert(this->ouput_consumer != NULL);
				sjLogDebug("sjPipe write 3\n");
				this->ouput_consumer->notify();
				sjLogDebug("sjPipe write 4\n");
			}
			void setOuputConsumer(sjConsumer * consumer){
				this->ouput_consumer  = consumer;
			}

			sjConsumer * getOuputConsumer(){
				return ouput_consumer;
			}
			protected:
				sjConsumer * ouput_consumer;
				T packet;
		};
};

typedef sjPF<sjPolyhedron, sjParameterStore> sjPolyhedronPipe;
/*
	sjPolyhedronPipe::sjConsumer
	sjPolyhedronPipe::sjFilter
	sjPolyhedronPipe::sjFilterDecorator
	sjPolyhedronPipe::sjPipe
	sjPolyhedronPipe::sjProducer
	sjPolyhedronPipe::sjSink
	sjPolyhedronPipe::sjSource
*/


}
#endif //__sjPIPEFILTER__H__