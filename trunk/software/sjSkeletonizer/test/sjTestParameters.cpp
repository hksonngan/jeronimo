#include <stdio.h>
#include <string>
#include "sjParameter.h"
#include "sjException.h"
using namespace sj;

int main(){
	
	sjInfo minf;
	minf.setName("ABCDE");
	printf("Could be ABCDE: %s", minf.getName().c_str());

	sjParameterBase * num = new sjIntParam();
	num->setName("12345");
	num->setData((void *)(new int(12)));

	sjIntParam * numInt =	reinterpret_cast<sjIntParam *>(num);
	int * x = (int *)numInt->getData();
	printf("\nCould be 12: %d", *x);

	sjParameterStore store;
	sjParameterBase * baseptr = store.getParameter("....");
	if(baseptr!= NULL){
		printf("%s", baseptr->getName().c_str());
	}else{
		printf("\nNot found element.");
	}

	return 0;
}