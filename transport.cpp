#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

struct edge {
	int flow;
	int cost;
	int goal;
	bool is_active = false;

} ;

int main()
{
	unsigned int storesNum, storageNum;
	cin >> storageNum;
	cin >> storesNum;

	int* storage = new int[storageNum];
	for (int i = 0; i < storageNum; i++) cin >> storage[i];

	int* store = new int[storesNum];
	for (int i = 0; i < storesNum; i++) cin >> store[i];

	int** costs = new int*[storageNum];
	for (int i = 0; i < storageNum; i++) {
		costs[i] = new int[storesNum];
		for (int j = 0; j < storesNum; j++) cin >> costs[i][j];
	}

	unordered_map<int, edge*>* edges = new unordered_map<int, edge*>[storesNum+storageNum];

	for (int i = 0, int j = 0; i < storageNum && j < storesNum;) {

	}


	cout << "DONE";

	for (int i = 0; i < storesNum + storageNum; ++i) {
		for (auto noIdea : edges[i]) {
			delete noIdea.second; // Deallocate each edge* if dynamically allocated
		}
		edges[i].clear(); // Clear the map
	}
	delete[] edges;
	delete[] storage;
	delete[] store;
	delete[] costs;

}
