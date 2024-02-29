#include "utils.cpp"
#include "SCCResult.cpp"

class StrongConnectedComponents {
	/** Adjacency-list of original graph */
	vector2d adjListOriginal;

	/** Adjacency-list of currently viewed subgraph */
	vector2d adjList;
	
	/** Helpattribute for finding scc's */
	vector<bool> visited;

	/** Helpattribute for finding scc's */
	vector<int> S;

	/** Helpattribute for finding scc's */
	vector<int> lowlink;

	/** Helpattribute for finding scc's */
	vector<int> number;

	/** Helpattribute for finding scc's */
	int sccCounter = 0;

	/** Helpattribute for finding scc's */
	vector<int> currentSCCs;

	StrongConnectedComponents(vector2d adjList) {
		adjListOriginal = adjList;
	}

	SCCResult getAdjacencyList(int node) {
		visited.reserve(adjListOriginal.size());
		lowlink.reserve(adjListOriginal.size());
		number.reserve(adjListOriginal.size());
		visited.reserve(adjListOriginal.size());

		makeAdjListSubgraph(node);

		for (int i = node; i < adjListOriginal.size(); i++) {
			if (!visited[i]) {
				getStrongConnectedComponents(i);
				vector<int> nodes = getLowestIdComponent();
				if (nodes != NULL && !nodes.contains(new Integer(node)) && !nodes.contains(new Integer(node + 1))) {
					return getAdjacencyList(node + 1);
				} else {
					Vector[] adjacencyList = getAdjList(nodes);
					if (adjacencyList != null) {
						for (int j = 0; j < adjListOriginal.length; j++) {
							if (adjacencyList[j].size() > 0) {
								return new SCCResult(adjacencyList, j);
							}
						}
					}
				}
			}
		}

		return NULL;
	}

	void makeAdjListSubgraph(int node) {
		adjList = new int[adjListOriginal.length][0];

		for (int i = node; i < adjList.length; i++) {
			Vector successors = new Vector();
			for (int j = 0; j < adjListOriginal[i].length; j++) {
				if (adjListOriginal[i][j] >= node) {
					successors.add(new Integer(adjListOriginal[i][j]));
				}
			}
			if (successors.size() > 0) {
				adjList[i] = new int[successors.size()];
				for (int j = 0; j < successors.size(); j++) {
					Integer succ = (Integer) successors.get(j);
					adjList[i][j] = succ.intValue();
				}
			}
		}
	}

	private Vector getLowestIdComponent() {
		int min = adjList.length;
		Vector currScc = null;

		for (int i = 0; i < currentSCCs.size(); i++) {
			Vector scc = (Vector) currentSCCs.get(i);
			for (int j = 0; j < scc.size(); j++) {
				Integer node = (Integer) scc.get(j);
				if (node.intValue() < min) {
					currScc = scc;
					min = node.intValue();
				}
			}
		}

		return currScc;
	}

	private Vector[] getAdjList(Vector nodes) {
		Vector[] lowestIdAdjacencyList = null;

		if (nodes != null) {
			lowestIdAdjacencyList = new Vector[adjList.length];
			for (int i = 0; i < lowestIdAdjacencyList.length; i++) {
				lowestIdAdjacencyList[i] = new Vector();
			}
			for (int i = 0; i < nodes.size(); i++) {
				int node = ((Integer) nodes.get(i)).intValue();
				for (int j = 0; j < adjList[node].length; j++) {
					int succ = adjList[node][j];
					if (nodes.contains(new Integer(succ))) {
						lowestIdAdjacencyList[node].add(new Integer(succ));
					}
				}
			}
		}

		return lowestIdAdjacencyList;
	}

	private void getStrongConnectedComponents(int root) {
		sccCounter++;
		lowlink[root] = sccCounter;
		number[root] = sccCounter;
		visited[root] = true;
		S.add(new Integer(root));

		for (int i = 0; i < adjList[root].length; i++) {
			int w = adjList[root][i];
			if (!visited[w]) {
				getStrongConnectedComponents(w);
				lowlink[root] = Math.min(lowlink[root], lowlink[w]);
			} else if (number[w] < number[root]) {
				if (S.contains(new Integer(w))) {
					lowlink[root] = Math.min(lowlink[root], number[w]);
				}
			}
		}

		// found scc
		if ((lowlink[root] == number[root]) && (S.size() > 0)) {
			int next = -1;
			Vector scc = new Vector();

			do {
				next = ((Integer) S.get(S.size() - 1)).intValue();
				S.remove(S.size() - 1);
				scc.add(new Integer(next));
			} while (number[next] > number[root]);

			// simple scc's with just one node will not be added
			if (scc.size() > 1) {
				currentSCCs.add(scc);
			}
		}
	}

	public static void main(String[] args) {
		boolean[][] adjMatrix = new boolean[10][];

		for (int i = 0; i < 10; i++) {
			adjMatrix[i] = new boolean[10];
		}

		/*adjMatrix[0][1] = true;
		adjMatrix[1][2] = true;
		adjMatrix[2][0] = true;
		adjMatrix[2][4] = true;
		adjMatrix[1][3] = true;
		adjMatrix[3][6] = true;
		adjMatrix[6][5] = true;
		adjMatrix[5][3] = true;
		adjMatrix[6][7] = true;
		adjMatrix[7][8] = true;
		adjMatrix[7][9] = true;
		adjMatrix[9][6] = true;*/
		
        adjMatrix[0][1] = true;
        adjMatrix[1][2] = true;
        adjMatrix[2][0] = true; adjMatrix[2][6] = true;
        adjMatrix[3][4] = true;
        adjMatrix[4][5] = true; adjMatrix[4][6] = true;
        adjMatrix[5][3] = true;
        adjMatrix[6][7] = true;
        adjMatrix[7][8] = true;
        adjMatrix[8][6] = true;
        
        adjMatrix[6][1] = true;

		int[][] adjList = AdjacencyList.getAdjacencyList(adjMatrix);
		StrongConnectedComponents scc = new StrongConnectedComponents(adjList);
		for (int i = 0; i < adjList.length; i++) {
			System.out.print("i: " + i + "\n");
			SCCResult r = scc.getAdjacencyList(i);
			if (r != null) {
				Vector[] al = scc.getAdjacencyList(i).getAdjList();
				for (int j = i; j < al.length; j++) {
					if (al[j].size() > 0) {
						System.out.print("j: " + j);
						for (int k = 0; k < al[j].size(); k++) {
							System.out.print(" _" + al[j].get(k).toString());
						}
						System.out.print("\n");
					}
				}
				System.out.print("\n");
			}
		}
	}
};
