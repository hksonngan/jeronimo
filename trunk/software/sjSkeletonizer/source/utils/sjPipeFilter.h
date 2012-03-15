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
			void setInputPipe(sjPipe * p){
				input_pipe = p;
			}
		protected:
			sjPipe * input_pipe;
		};

		class sjSink : public sjConsumer {
		public:
			virtual void notify(){
				assert(this->input_pipe != NULL);
				while(this->input_pipe->size() >0){
					this->packets.push_back(this->input_pipe->read());
				}
				consume();
			}

			virtual void consume() = 0;
		protected:
			std::list<T > packets;
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
				printf("sjFilter notify 1\n");
				assert(this->input_pipe != NULL);
				printf("sjFilter notify 2\n");
				update();
				printf("sjFilter notify 3\n");
			}
			void update(){
				printf("sjFilter update 1\n");
				assert(this->output_pipe != NULL);
				printf("sjFilter update 2\n");
				this->output_pipe->write( iterate());
				printf("sjFilter update 3\n");
			}
			
			virtual void setParameters(B *) = 0;
			virtual void setMesh(T) = 0;
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
				printf("sjPipe read 1\n");
				assert(size() != 0);
				printf("sjPipe read 2\n");
				T p =  this->packets.back();
				printf("sjPipe read 3\n");
				this->packets.pop_back();
				printf("sjPipe read 4\n");
				return p;
			}
			void write(T p){
				printf("sjPipe write 1\n");
				this->packets.push_back(p);
				printf("sjPipe write 2\n");
				assert(this->ouput_consumer != NULL);
				printf("sjPipe write 3\n");
				this->ouput_consumer->notify();
				printf("sjPipe write 4\n");
			}
			int size(){
				return this->packets.size();
			}
			void setOuputConsumer(sjConsumer * consumer){
				this->ouput_consumer  = consumer;
			}
			protected:
				sjConsumer * ouput_consumer;
				std::list<T > packets;
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