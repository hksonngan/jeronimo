#ifndef __SJ__REFINEMENT_H__
#define __SJ__REFINEMENT_H__

#include <iostream>
#include <list>
#include <set>
#include <vector>
#include "sjDataType.h"
#include "sjUtils.h"
#include "sjGraphSkeleton.h"
#include "sjSimplificator.h"

using namespace std;

namespace sj{
	class a{
		a();
	};

	class sjRefinement{
	public:
		bool isTerminalNode(int vid);
		bool isLineNode(int vid);
		bool isJunctionNode(int vid);
		vector<vector<int>> getBoundaries(int vid);
		set<int> getNeighborsToMeshRegion(set<int> mesh_region);
		sjVector_3 calculateWeightedAverageDisplacement(vector<vector<int>> boundaries, int node_id);
		vector<int> getAdjacentEdgesinBoundaryToVertex(vector<int> boundary, int vid);
		double getLengthAdjacentEdges(vector<int> adjacent_points, int vid);

		sjGraphSkeletonType sjskeleton;
		sjGraphSkeletonType original_skeleton;
		sjGraphSkeletonType contracted_skeleton;

	};
}
#endif // __SJ__REFINEMENT_H__