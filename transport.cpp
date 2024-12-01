#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#define forever while (1)
using namespace std;


struct edge {
	int flow;
	int cost;
	int goal;
	bool is_active = false;
};


void solve() {
	int markets_amount, storages_amount, nodes_amount;
	cin >> storages_amount;
	cin >> markets_amount;
	nodes_amount = markets_amount+storages_amount;

	int* storages = new int[storages_amount];
	for (int i = 0; i < storages_amount; i++) cin >> storages[i];

	int* markets = new int[markets_amount];
	for (int i = 0; i < markets_amount; i++) cin >> markets[i];

	int** costs = new int*[storages_amount];
	for (int i = 0; i < storages_amount; i++) {
		costs[i] = new int[markets_amount];
		for (int j = 0; j < markets_amount; j++) cin >> costs[i][j];
	}

	unordered_map<int, edge*>* edges = new unordered_map<int, edge*>[nodes_amount];
	for (int i = 0, j = 0; i < storages_amount && j < markets_amount;) {
		edge* tmp_edge = new edge{0, costs[i][j], j+storages_amount, false};
		edges[i][j+storages_amount] = tmp_edge;
		edges[j+storages_amount][i] = tmp_edge;
	}

	for (int i = 0, j = 0; i < storages_amount && j < markets_amount;) {
		edge* cur_edge = edges[i][j+storages_amount];
		cur_edge->flow = min(storages[i], markets[j]);
		cur_edge->is_active = true;
		if (storages[i] < markets[j]) {
			markets[j] -= storages[i];
			i++;
		} else {
			storages[i] - markets[j];
			j++;
		}
	}

	int* sums = new int[nodes_amount];
	int* parents = new int[nodes_amount];
	bool* visited = new bool[nodes_amount];
	forever {
		for (int i = 0; i < nodes_amount; i++) sums[i] = 0, visited[i] = false;
		
		queue<pair<int, int>> visit_order;
		visit_order.push({0, 0});
		while (!visit_order.empty()) {
			pair<int, int> p = visit_order.front();
			visit_order.pop();
			int cur_node = p.first, par_node = p.second;
			if (visited[cur_node]) continue;
			visited[cur_node] = true;
			
			if (cur_node != par_node) {
				parents[cur_node] = par_node;
				edge* par_edge = edges[cur_node][par_node];
				int tmp_sum = par_edge->cost;
				if (par_edge->goal == cur_node) tmp_sum = -tmp_sum;
				sums[cur_node] = sums[par_node] + tmp_sum;
			}

			for (const pair<int, edge*> &p : edges[cur_node]) {
				int son_node = p.first;
				if (visited[par_node]) continue;
				edge* son_edge = p.second;
				if (!son_edge->is_active) continue;
				visit_order.push({son_node, cur_node});
			}
		}

		bool is_stale = true;
		int x_node, y_node;
		for (int i = 0; i < storages_amount; i++) {
			for (int j = 0; j < markets_amount; j++) {
				if (edges[i][j+storages_amount]->is_active) continue;
				if (costs[i][j] + sums[i] - sums[j+storages_amount] >= 0) continue;
				x_node = i, y_node = j + storages_amount, is_stale = false;
				break; 
			}
		}
		if (is_stale) break;
		//border node?
		int min_flow = -1;
		for (int cur_node = x_node, par_node = parents[cur_node]; cur_node != 0; cur_node = par_node, par_node = parents[par_node]) {
			if (cur_node < storages_amount) continue;
			int cur_flow = edges[cur_node][par_node]->flow;
			if (min_flow == -1 || min_flow < cur_flow) min_flow = cur_flow; 
		}
		for (int cur_node = y_node, par_node = parents[cur_node]; ;) break; //TODO
	}
	



	for (int i = 0; i < markets_amount + storages_amount; ++i) {
		for (const pair<int, edge*> &p : edges[i]) delete p.second;
	}
	delete[] edges;
	delete[] storages;
	delete[] markets;
	delete[] costs;
	delete[] sums;
	delete[] parents;
	delete[] visited;
}


int main() {
	cin.tie(0);
	ios_base::sync_with_stdio(false);
	solve();
	flush(cout);
	return 0;
}