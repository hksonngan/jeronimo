#include "sjPluginXmlLoader.h"
#include "sjPluginLoader.h"
#include "sjLog.h"

using namespace sj;


sjPluginXmlLoader::~sjPluginXmlLoader(void){
	printf("sjPluginXmlLoader::~sjPluginXmlLoader\n");
	sjPluginLoader loader;
	loader.unloadLibrary(m_hDLL);
}

sjPluginXmlLoader::sjPluginXmlLoader(std::string file_name){
	int i;
	int length_paramters;
	xmlDocPtr document;
	xmlNodeSetPtr node_parameters;
	
	document= sjXml::parseXmlFile(file_name);
	if(document == NULL) throw sjException(sjException::SJ_EXCEPTION_NULL);
	
	sjParameterStore * information = new sjParameterStore();
	sjParameterStore * parameters = new sjParameterStore();

	information->addParameter(loadPluginParameterInfo(document, "name", "/plugin"));

	information->addParameter(loadPluginParameterInfo(document, "filename", "/plugin"));

	information->addParameter(loadPluginParameterInfo(document, "description", "/plugin"));

	information->addParameter(loadPluginParameterInfo(document, "author", "/plugin"));

	node_parameters = sjXml::getXmlNodeFromExpression(document, "/plugin/parameter");

	length_paramters = sjXml::getSizeXmlNodeSet(node_parameters);

	for(i=0; i<length_paramters; i++){
		char query[256];
		sprintf(query, "/plugin/parameter[%d]", i+1);
		sjParameterBase * param = loadParameter(document, query);
		parameters->addParameter(param);	
	}

	sjPluginLoader loader;
	m_hDLL = loader.loadLibrary(information->getParameter("filename")->getDescription());

	if(!m_hDLL){
		throw sjException(sjException::SJ_EXCEPTION_LOAD_SHARED_LIBRARY_FAILS, loader.getError(m_hDLL));
	}
	printf("sjPluginXmlLoader: -> Line 0\n");
	try
	{
		m_pfnRegisterPlugin = reinterpret_cast<fnRegisterPlugin *>(
			loader.getSymbol(m_hDLL, "registerPlugin"));
		printf("sjPluginXmlLoader: -> Line 1\n");

		sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
		printf("sjPluginXmlLoader: -> Line 4\n");
		kernel.addPluginInfo(information->getParameter("name")->getDescription(), information);
		printf("sjPluginXmlLoader: -> Line 9\n");
		kernel.addMeshFilterInfo(information->getParameter("name")->getDescription(), parameters);

		setName(information->getParameter("name")->getDescription());
		


	}
	catch(...)
	{
		loader.unloadLibrary(m_hDLL);
		throw;
	}
}

sjParameterBase * sjPluginXmlLoader::loadParameter(xmlDocPtr document, std::string expression_parameter) throw(sjException){
	//sjParameter * param = NULL;
	std::string query = expression_parameter + std::string("/@");
	//param = new sjParameter();
	std::string param_name = sjXml::getStringFromExpression(document, expression_parameter + std::string("/name"));
	std::string datatype = sjXml::getStringFromExpression(document, query + std::string("datatype"));
	std::string description = sjXml::getStringFromExpression(document, query + std::string("description"));
	sjParameterBase * param;
	
	sjTypeData type_data = sjTypeDataUtils::getTypeDataFromString(datatype);

	switch(type_data){
		case SJ_DATA_TYPE_BOOLEAN:				param = new sjBoolParam();				break;
		case SJ_DATA_TYPE_CHAR:					param = new sjUnsignedCharParam();		break;
		case SJ_DATA_TYPE_DOUBLE:				param = new sjDoubleParam();			break;
		case SJ_DATA_TYPE_FLOAT:				param = new sjFloatParam();				break;
		case SJ_DATA_TYPE_INT:					param = new sjIntParam();				break;
		case SJ_DATA_TYPE_LONG:					param = new sjLongParam();				break;
		case SJ_DATA_TYPE_LONG_DOUBLE:			param = new sjLongDoubleParam();		break;
		case SJ_DATA_TYPE_SHORT:				param = new sjShortParam();				break;
		case SJ_DATA_TYPE_STRING:				param = new sjStringParam();			break;
		case SJ_DATA_TYPE_TIME:					param = new sjTimeParam();				break;
		case SJ_DATA_TYPE_UNSIGNED_CHAR:		param = new sjCharParam();				break;
		case SJ_DATA_TYPE_UNSIGNED_INT:			param = new sjUnsignedCharParam();		break;
		case SJ_DATA_TYPE_UNSIGNED_LONG:		param = new sjUnsignedLongParam();		break;
		case SJ_DATA_TYPE_UNSIGNED_SHORT:		param = new sjUnsignedShortParam();		break;
		default:
			throw sjException(sjException::SJ_EXCEPTION_UNKNOW, std::string("sjPluginXmlLoader::loadParameter()"));
			return NULL;
	}
	param->setName(param_name);
	param->setDescription(description);

	std::string minValue, maxValue, step, defaultValue;

	switch(type_data){
		case SJ_DATA_TYPE_CHAR:
		case SJ_DATA_TYPE_DOUBLE:
		case SJ_DATA_TYPE_FLOAT:
		case SJ_DATA_TYPE_INT:
		case SJ_DATA_TYPE_LONG:
		case SJ_DATA_TYPE_LONG_DOUBLE:
		case SJ_DATA_TYPE_SHORT:
		case SJ_DATA_TYPE_UNSIGNED_CHAR:
		case SJ_DATA_TYPE_UNSIGNED_INT:
		case SJ_DATA_TYPE_UNSIGNED_LONG:
		case SJ_DATA_TYPE_UNSIGNED_SHORT:
			minValue = sjXml::getStringFromExpression(document, query + std::string("minValue"));
			maxValue = sjXml::getStringFromExpression(document, query + std::string("maxValue"));
			step = sjXml::getStringFromExpression(document, query + std::string("step"));
			defaultValue = sjXml::getStringFromExpression(document, query + std::string("defaultValue"));
			break;
	}

	char char_min_value = minValue.c_str()[0];
	char char_max_value = maxValue.c_str()[0];
	char char_step = step.c_str()[0];
	char char_default_value = defaultValue.c_str()[0];

	double d_min_value = atof(minValue.c_str());
	double d_max_value = atof(maxValue.c_str());
	double d_step_value = atof(step.c_str());
	double d_default_value = atof(defaultValue.c_str());

	float f_min_value = atof(minValue.c_str());
	float f_max_value = atof(maxValue.c_str());
	float f_step_value = atof(step.c_str());
	float f_default_value = atof(defaultValue.c_str());

	int i_min_value = atoi(minValue.c_str());
	int i_max_value = atoi(maxValue.c_str());
	int i_step_value = atoi(step.c_str());
	int i_default_value = atoi(defaultValue.c_str());

	long l_min_value = atol(minValue.c_str());
	long l_max_value = atol(maxValue.c_str());
	long l_step_value = atol(step.c_str());
	long l_default_value = atol(defaultValue.c_str());

	long double ld_min_value = atol(minValue.c_str());
	long double ld_max_value = atol(maxValue.c_str());
	long double ld_step = atol(step.c_str());
	long double ld_default_value = atol(defaultValue.c_str());

	short s_min_value = atoi(minValue.c_str());
	short s_max_value = atoi(maxValue.c_str());
	short s_step_value = atoi(step.c_str());
	short s_default_value = atoi(defaultValue.c_str());

	unsigned char uchar_min_value =(unsigned char) minValue.c_str()[0];
	unsigned char uchar_max_value =(unsigned char) maxValue.c_str()[0];
	unsigned char uchar_step =(unsigned char) step.c_str()[0];
	unsigned char uchar_default_value =(unsigned char) defaultValue.c_str()[0];

	unsigned int ui_min_value = atoi(minValue.c_str());
	unsigned int ui_max_value = atoi(maxValue.c_str());
	unsigned int ui_step_value = atoi(step.c_str());
	unsigned int ui_default_value = atoi(defaultValue.c_str());

	unsigned long  ul_min_value = strtoul(minValue.c_str(),NULL, 0);
	unsigned long  ul_max_value = strtoul(maxValue.c_str(),NULL, 0);
	unsigned long  ul_step_value = strtoul(step.c_str(),NULL, 0);
	unsigned long  ul_default_value = strtoul(defaultValue.c_str(),NULL, 0);

	unsigned short us_min_value = atoi(minValue.c_str());
	unsigned short us_max_value = atoi(maxValue.c_str());
	unsigned short us_step_value = atoi(step.c_str());
	unsigned short us_default_value = atoi(defaultValue.c_str());

	switch(type_data){
		case SJ_DATA_TYPE_CHAR:				
			((sjCharParam *)param)->setMinValue(char_min_value);
			((sjCharParam *)param)->setMaxValue(char_max_value);
			((sjCharParam *)param)->setStep(char_step);
			((sjCharParam *)param)->setDefaultValue(char_default_value);
			break;

		case SJ_DATA_TYPE_DOUBLE:
			((sjDoubleParam*)param)->setMinValue(d_min_value);
			((sjDoubleParam*)param)->setMaxValue(d_max_value);
			((sjDoubleParam*)param)->setStep(d_step_value);
			((sjDoubleParam*)param)->setDefaultValue(d_default_value);
			break;

		case SJ_DATA_TYPE_FLOAT:
			((sjFloatParam*)param)->setMinValue(f_min_value);
			((sjFloatParam*)param)->setMaxValue(f_max_value);
			((sjFloatParam*)param)->setStep(f_step_value);
			((sjFloatParam*)param)->setDefaultValue(f_default_value);
			break;

		case SJ_DATA_TYPE_INT:			
			((sjIntParam*)param)->setMinValue(i_min_value);
			((sjIntParam*)param)->setMaxValue(i_max_value);
			((sjIntParam*)param)->setStep(i_step_value);
			((sjIntParam*)param)->setDefaultValue(i_default_value);
			break;

		case SJ_DATA_TYPE_LONG:					
			((sjLongParam*)param)->setMinValue(l_min_value);
			((sjLongParam*)param)->setMaxValue(l_max_value);
			((sjLongParam*)param)->setStep(l_step_value);
			((sjLongParam*)param)->setDefaultValue(l_default_value);
			break;

		case SJ_DATA_TYPE_LONG_DOUBLE:			
			((sjLongDoubleParam*)param)->setMinValue(ld_min_value);
			((sjLongDoubleParam*)param)->setMaxValue(ld_max_value);
			((sjLongDoubleParam*)param)->setStep(ld_step);
			((sjLongDoubleParam*)param)->setDefaultValue(ld_default_value);
			break;

		case SJ_DATA_TYPE_SHORT:				
			((sjShortParam*)param)->setMinValue(s_min_value);
			((sjShortParam*)param)->setMaxValue(s_max_value);
			((sjShortParam*)param)->setStep(s_step_value);
			((sjShortParam*)param)->setDefaultValue(s_default_value);
			break;

		case SJ_DATA_TYPE_UNSIGNED_CHAR:		
			((sjUnsignedCharParam *)param)->setMinValue(uchar_min_value);
			((sjUnsignedCharParam *)param)->setMaxValue(uchar_max_value);
			((sjUnsignedCharParam *)param)->setStep(uchar_step);
			((sjUnsignedCharParam *)param)->setDefaultValue(uchar_default_value);
			break;

		case SJ_DATA_TYPE_UNSIGNED_INT:			
			((sjUnsignedIntParam*)param)->setMinValue(ui_min_value);
			((sjUnsignedIntParam*)param)->setMaxValue(ui_max_value);
			((sjUnsignedIntParam*)param)->setStep(ui_step_value);
			((sjUnsignedIntParam*)param)->setDefaultValue(ui_default_value);
			break;

		case SJ_DATA_TYPE_UNSIGNED_LONG:		
			((sjUnsignedLongParam*)param)->setMinValue(ul_min_value);
			((sjUnsignedLongParam*)param)->setMaxValue(ul_max_value);
			((sjUnsignedLongParam*)param)->setStep(ul_step_value);
			((sjUnsignedLongParam*)param)->setDefaultValue(ul_default_value);
			break;

		case SJ_DATA_TYPE_UNSIGNED_SHORT:		
			((sjUnsignedShortParam*)param)->setMinValue(us_min_value);
			((sjUnsignedShortParam*)param)->setMaxValue(us_max_value);
			((sjUnsignedShortParam*)param)->setStep(us_step_value);
			((sjUnsignedShortParam*)param)->setDefaultValue(us_default_value);
			break;
	}

	return param;
}

sjParameterBase * sjPluginXmlLoader::loadPluginParameterInfo(xmlDocPtr document, std::string name, std::string expression_parameter){
	std::string info;
	info = sjXml::getStringFromExpression(document, expression_parameter + std::string("/@") +  name );
	sjParameterBase  * param = new sjStringParam();
	param->setName(name);
	param->setDescription(info);
	return param;
}