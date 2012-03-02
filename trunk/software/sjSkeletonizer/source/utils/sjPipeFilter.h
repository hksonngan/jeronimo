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
		protected:
			sjPipe * output_pipe;
		};

		class sjConsumer {
		public:
			sjConsumer(): input_pipe(NULL){
			}
			virtual void notify() = 0;
			void setInputPipe(sjPipe * p);
		protected:
			sjPipe * input_pipe;
		};

		class sjSink : public sjConsumer {
		public:
			virtual void notify();
			virtual void consume() = 0;
		protected:
			std::list<T *> packets;
		};

		class sjSource : public sjProducer {
		public:
			virtual void update();
			virtual T * produce() = 0;
		};

		class sjFilter : public sjConsumer, public sjProducer {
		public:
			void notify(){
				assert(this->input_pipe != NULL);
				update();
			}
			void update(){
				assert(this->output_pipe != NULL);
				iterate();
			}
			
			virtual void setParameters(B *) = 0;
			virtual void setMesh(T) = 0;
			virtual size_t getNumberOfVertex() = 0;
			virtual T * iterate() = 0;
			virtual T getMesh() = 0;
			virtual B * getParameters() = 0;
		};

		class sjPipe {
		public:
			sjPipe();
			T * read(){
				assert(size() != 0);
				T * p = *( this->packets.begin());
				this->packets.pop_back();
				return p;
			}
			void write(T * p);
			int size();
			void setOuputConsumer(sjConsumer * consumer);
			protected:
				sjConsumer * ouput_consumer;
				std::list<T * > packets;
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