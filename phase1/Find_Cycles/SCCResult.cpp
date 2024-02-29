#include "utils.cpp"

class SCCResult {
	set<int> nodeIDsOfSCC;
	vector2d adjList;
	int lowestNodeId = -1;
	
	SCCResult(vector2d _adjList, int lowestNodeId) {
		adjList = _adjList;
		lowestNodeId = lowestNodeId;
		if (!adjList.empty()) {
			for (int i = lowestNodeId; i < adjList.size(); i++) {
				if (adjList[i].size() > 0) {
					nodeIDsOfSCC.insert(i);
				}
			}
		}
	}

	vector2d getAdjList() {
		return adjList;
	}

	int getLowestNodeId() {
		return lowestNodeId;
	}
};
