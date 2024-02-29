#include "utils.cpp" 

vector2d getAdjacencyList(vector2d adjacencyMatrix) {
	int num_vertices = adjacencyMatrix.size();
	vector<vector<int>> list(num_vertices);

	for (int i = 0; i < num_vertices; i++) {
		vector<int> v;
		for (int j = 0; j < num_vertices; j++) {
			if (adjacencyMatrix[i][j]) {
				v.push_back(j);
			}
		}

		list[i] = vector<int>(num_vertices);


		for (int j = 0; j < v.size(); j++) {
			list[i][j] = v[j];
		}
	}
	
	return list;
}
