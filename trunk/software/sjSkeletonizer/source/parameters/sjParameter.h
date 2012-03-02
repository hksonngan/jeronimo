#if defined (WIN32) || defined (_WIN32_)
	#pragma once
	#pragma warning(disable: 4290)
	#pragma warning(disable: 4996)
#endif

#ifndef __SJPARAMETER__H__
#define __SJPARAMETER__H__

#include <string>
#include <map>
#include <typeinfo>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>

namespace sj{

	typedef enum{
		SJ_DATA_TYPE_BOOLEAN = 0, 
		SJ_DATA_TYPE_CHAR = 10, 
		SJ_DATA_TYPE_DOUBLE,
		SJ_DATA_TYPE_FLOAT,
		SJ_DATA_TYPE_INT,
		SJ_DATA_TYPE_LONG,
		SJ_DATA_TYPE_LONG_DOUBLE,
		SJ_DATA_TYPE_SHORT, 
		SJ_DATA_TYPE_UNSIGNED_CHAR, 
		SJ_DATA_TYPE_UNSIGNED_INT,
		SJ_DATA_TYPE_UNSIGNED_LONG,
		SJ_DATA_TYPE_UNSIGNED_SHORT,
		SJ_DATA_TYPE_STRING = 100,
		SJ_DATA_TYPE_TIME,
		SJ_DATA_TYPE_UNKNOW
	} sjTypeData;

	class sjTypeDataUtils{
	public:
		static sjTypeData getTypeDataFromString(std::string t_data){
			if(t_data.compare(std::string("SJ_DATA_TYPE_BOOLEAN")) == 0 ) return SJ_DATA_TYPE_BOOLEAN;
			else if(t_data.compare(std::string("SJ_DATA_TYPE_CHAR")) == 0 ) return SJ_DATA_TYPE_CHAR;
			else if(t_data.compare(std::string("SJ_DATA_TYPE_DOUBLE")) == 0 ) return SJ_DATA_TYPE_DOUBLE;
			else if(t_data.compare(std::string("SJ_DATA_TYPE_FLOAT")) == 0 ) return SJ_DATA_TYPE_FLOAT;
			else if(t_data.compare(std::string("SJ_DATA_TYPE_INT")) == 0 ) return SJ_DATA_TYPE_INT;
			else if(t_data.compare(std::string("SJ_DATA_TYPE_LONG")) == 0 ) return SJ_DATA_TYPE_LONG;
			else if(t_data.compare(std::string("SJ_DATA_TYPE_LONG_DOUBLE")) == 0 ) return SJ_DATA_TYPE_LONG_DOUBLE;
			else if(t_data.compare(std::string("SJ_DATA_TYPE_SHORT")) == 0 ) return SJ_DATA_TYPE_SHORT;
			else if(t_data.compare(std::string("SJ_DATA_TYPE_STRING")) == 0 ) return SJ_DATA_TYPE_STRING;
			else if(t_data.compare(std::string("SJ_DATA_TYPE_TIME")) == 0 ) return SJ_DATA_TYPE_TIME;
			else if(t_data.compare(std::string("SJ_DATA_TYPE_UNSIGNED_CHAR")) == 0 ) return SJ_DATA_TYPE_UNSIGNED_CHAR;
			else if(t_data.compare(std::string("SJ_DATA_TYPE_UNSIGNED_INT")) == 0 ) return SJ_DATA_TYPE_UNSIGNED_INT;
			else if(t_data.compare(std::string("SJ_DATA_TYPE_UNSIGNED_LONG")) == 0 ) return SJ_DATA_TYPE_UNSIGNED_LONG;
			else if(t_data.compare(std::string("SJ_DATA_TYPE_UNSIGNED_SHORT")) == 0 ) return SJ_DATA_TYPE_UNSIGNED_SHORT;
			return SJ_DATA_TYPE_UNKNOW;
		}
	};

	class sjInfo
	{
	public:
		sjInfo();
		sjInfo(const sjInfo &Other);
		~sjInfo();
		void setName(std::string);
		void setDescription(std::string);
		std::string getName(void);
		std::string getDescription(void);
	protected:
		std::string name;
		std::string description;
	};
	
	class sjParameterBase
	:public sjInfo
	{
	public:
		sjParameterBase(){}
		virtual ~sjParameterBase(){}
		virtual void * getData(){return NULL;}
		virtual void setData(void * adata){};
		virtual sjTypeData getType(){ return SJ_DATA_TYPE_UNKNOW;}
		virtual void setType(sjTypeData atype){}
	};
	
	template <class T>
	class sjNumericParam
	:public sjParameterBase
	{
	public:
		typedef typename T NumericType;
		sjNumericParam(){
			min_value = 0;
			max_value = 0;
			step = 0;
			default_value = 0;
		}

		~sjNumericParam(){
		}

		void * getData(){
			T * ptr = new T;
			* ptr = data;
			void * ptr_void = ptr;
			return ptr_void;
		}

		void setData(void * a_data){
			T * ptr = new T;
			ptr = (T *)a_data;
			data = *ptr;
		}

		sjTypeData getType(){
			if(typeid(T) == typeid(float)) type = SJ_DATA_TYPE_FLOAT;
			else if(typeid(T) == typeid(int)) type = SJ_DATA_TYPE_INT;
			else if(typeid(T) == typeid(double)) type = SJ_DATA_TYPE_DOUBLE;
			else if(typeid(T) == typeid(long)) type = SJ_DATA_TYPE_LONG;
			else if(typeid(T) == typeid(long double)) type = SJ_DATA_TYPE_LONG_DOUBLE;
			else if(typeid(T) == typeid(short)) type = SJ_DATA_TYPE_SHORT;
			else if(typeid(T) == typeid(unsigned int)) type = SJ_DATA_TYPE_UNSIGNED_INT;
			else if(typeid(T) == typeid(unsigned long)) type = SJ_DATA_TYPE_UNSIGNED_LONG;
			else if(typeid(T) == typeid(unsigned short)) type = SJ_DATA_TYPE_UNSIGNED_SHORT;
			else if(typeid(T) == typeid(char)) type = SJ_DATA_TYPE_CHAR;
			else if(typeid(T) == typeid(unsigned char)) type = SJ_DATA_TYPE_UNSIGNED_CHAR;
			return type;
		}
		
		void setType(sjTypeData a_type){
			type = getType();
		}
		void setMinValue(T a_min){
			min_value = a_min;
		}
		void setMaxValue(T a_max){
			max_value = a_max;
		}
		void setStep(T a_step){
			step = a_step;
		}
		void setDefaultValue(T a_default_value){
			default_value = a_default_value;
		}

		T getMinValue(void){
			return min_value;
		}
		T getMaxValue(void){
			return max_value;
		}
		T getStep(void){
			return step;
		}
		T getDefaultValue(void){
			return default_value;
		}
		T getValue(){
			return data;
		}
		void setValue(T adata){
			data = adata;
		}

		void setValueFromString(std::string a_value){
			if(a_value.length() == 0) return;
			if(typeid(T) == typeid(float)) data = (float)atof(a_value.c_str());
			else if(typeid(T) == typeid(int)) data = (int)atoi(a_value.c_str());
			else if(typeid(T) == typeid(double)) data = (double)atof(a_value.c_str());
			else if(typeid(T) == typeid(long)) data = (long)atoi(a_value.c_str());
			else if(typeid(T) == typeid(long double))  data = (long double)atol(a_value.c_str());
			else if(typeid(T) == typeid(short)) data = (short)atoi(a_value.c_str());
			else if(typeid(T) == typeid(unsigned int)) data = (unsigned int)atoi(a_value.c_str());
			else if(typeid(T) == typeid(unsigned long)) data = (unsigned long)strtoul(a_value.c_str(),NULL, 0);
			else if(typeid(T) == typeid(unsigned short)) data = (unsigned short)atoi(a_value.c_str());
			else if(typeid(T) == typeid(char)) data = (char)a_value.c_str()[0];
			else if(typeid(T) == typeid(unsigned char)) data = (unsigned char)a_value.c_str()[0];
		}

		std::string toString(){
			std::ostringstream ss;
			ss << data;
			std::string s(ss.str());
			return s;
		}

		std::string convertToString(T a_value){
			std::ostringstream ss;
			ss << a_value;
			std::string s(ss.str());
			return s;
		}
		
	protected:
		T data;
		T min_value;
		T max_value;
		T step;
		T default_value;
		sjTypeData type;
	};

	typedef sjNumericParam<char> sjCharParam;
	typedef sjNumericParam<double> sjDoubleParam;
	typedef sjNumericParam<float> sjFloatParam;
	typedef sjNumericParam<int> sjIntParam;
	typedef sjNumericParam<long> sjLongParam;
	typedef sjNumericParam<long double> sjLongDoubleParam;
	typedef sjNumericParam<short> sjShortParam;
	typedef sjNumericParam<unsigned char> sjUnsignedCharParam;
	typedef sjNumericParam<unsigned int> sjUnsignedIntParam;
	typedef sjNumericParam<unsigned long> sjUnsignedLongParam;
	typedef sjNumericParam<unsigned short> sjUnsignedShortParam;

	class sjStringParam
	:public sjParameterBase
	{
	public:
		virtual void * getData();
		virtual void setData(void *);
		virtual sjTypeData getType();
		virtual void setType(sjTypeData);
		std::string getValue();
		void setValue(std::string);
	protected:
		std::string data;
	};
	
	class sjBoolParam
	:public sjParameterBase
	{
	public:
		virtual void * getData();
		virtual void setData(void *);
		virtual sjTypeData getType();
		virtual void setType(sjTypeData);
		bool getValue();
		void setValue(bool);
	protected:
		bool data;
	};

	class sjTimeParam
	:public sjParameterBase
	{
	public:
		virtual void * getData();
		virtual void setData(void *);
		virtual sjTypeData getType();
		virtual void setType(sjTypeData);
		time_t getValue();
		void setValue(time_t);
	protected:
		time_t data;
	};

	class sjParameterStore
	{
	public:
		sjParameterStore(void);
		~sjParameterStore(void);

		void addParameter(sjParameterBase *);

		sjParameterBase * getParameter(std::string);
		sjParameterBase * getParameter(unsigned int );

		unsigned int getNumberOfParameters(void);

	protected:
		std::map<std::string, sjParameterBase *> parameters;

	};
	
}

#endif //__SJPARAMETER__H__