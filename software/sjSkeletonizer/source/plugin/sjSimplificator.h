#ifndef __SJ_SIMPLI_H__
#define __SJ_SIMPLI_H__

#include "sjDataType.h"
#include "sjKernelPlugin.h"
#include "sjPipeFilter.h"
#include "sjSystem.h"
#include "sjUtils.h"



namespace sj{
	class sjSimplificator{
	public:
		sjSimplificator();
		Matrix4d getFundamentalErrorQuadric(sjHalfedge_handle);
		double calculateSamplingCost(sjHalfedge_handle);
		Matrix4d computeInitialQ(sjVertex_handle);
		bool isCollapseTunnel(sjHalfedge_handle);
	};
}

#endif //__SJ_SIMPLI_H__