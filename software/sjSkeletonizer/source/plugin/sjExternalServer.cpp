#include "sjExternalServer.h"

using namespace sj;

sjExternalServer::sjExternalServer()
{
}

sjExternalServer::sjExternalServer(const sjExternalServer &Other)
{
}

sjExternalServer::~sjExternalServer()
{
}

bool sjExternalServer::addSystemDriver(sjSystemDriver * ptr_system_driver)
{
	if(ptr_system_driver == NULL) return false;
	m_system_driver_vector.push_back(ptr_system_driver);
	return true;
}

size_t sjExternalServer::getSystemDriverCount() const
{
	return m_system_driver_vector.size();
}

sjSystemDriver * sjExternalServer::getSystemDriver(size_t index)
{
	if(index >= getSystemDriverCount()) return NULL;
	return m_system_driver_vector.at(index);
}

sjSystemDriver * sjExternalServer::getSystemDriver(std::string filter_name)
{
	size_t tam = m_system_driver_vector.size();
	size_t it;
	for(it = 0; it<tam; it++){
		std::string cad = (m_system_driver_vector.at(it))->getName();
		if(cad.compare(filter_name) == 0){
			return m_system_driver_vector.at(it);
		}
	}
	return NULL;
}

bool sjExternalServer::exiSystemDriver(std::string filter_name){
	size_t tam = m_system_driver_vector.size();
	size_t it;
	for(it = 0; it<tam; it++){
		std::string cad = (m_system_driver_vector.at(it))->getName();
		if(cad.compare(filter_name) == 0){
			return true;
		}
	}
	return false;
}