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


int solve() {
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
	for (int i = 0; i < storages_amount; i++) {
		for (int j = 0; j < markets_amount; j++) {
			edge* tmp_edge = new edge{0, costs[i][j], j+storages_amount, false};
			edges[i][j+storages_amount] = tmp_edge;
			edges[j+storages_amount][i] = tmp_edge;
		}
	}

	for (int i = 0, j = 0; i < storages_amount && j < markets_amount;) {
		edge* cur_edge = edges[i][j+storages_amount];
		cur_edge->flow = min(storages[i], markets[j]);
		cur_edge->is_active = true;
		if (storages[i] < markets[j]) {
			markets[j] -= storages[i];
			i++;
		} else {
			storages[i] -= markets[j];
			j++;
		}
	}

	//DEBUG-start
	for (int i = 0; i < storages_amount; i++) {
		for (int j = storages_amount; j < nodes_amount; j++) {
			cout << edges[i][j]->cost << '|' << i << ' ' << j-storages_amount << ' ' << edges[i][j]->flow << endl;
		}
	}
	//DEBUG-end

	int* sums = new int[nodes_amount];
	pair<int, int>* parents = new pair<int, int>[nodes_amount];
	bool* visited = new bool[nodes_amount];
	forever {
		cout << endl; //DEBUG
		for (int i = 0; i < nodes_amount; i++) {
			sums[i] = 0, visited[i] = false, parents[i] = {0, 0};
		}

		queue<pair<int, int>> visit_order;
		visit_order.push({0, 0});
		while (!visit_order.empty()) {
			pair<int, int> p = visit_order.front();
			visit_order.pop();
			int cur_node = p.first, par_node = p.second;
			if (visited[cur_node]) continue;
			visited[cur_node] = true;
			
			if (cur_node != par_node) {
				parents[cur_node] = {par_node, parents[par_node].second + 1};
				edge* par_edge = edges[cur_node][par_node];
				int tmp_sum = par_edge->cost;
				if (par_edge->goal != cur_node) tmp_sum = -tmp_sum;
				sums[cur_node] = sums[par_node] + tmp_sum;
			}

			for (const pair<int, edge*> &p : edges[cur_node]) {
				int son_node = p.first;
				if (visited[son_node]) continue;
				edge* son_edge = p.second;
				if (!son_edge->is_active) continue;
				visit_order.push({son_node, cur_node});
			}
		}

		//DEBUG-start
		for (int i = 0; i < nodes_amount; i++) {
			cout << sums[i] << ' ';
		}
		cout << endl;
		//DEBUG-end

		bool is_stale = true;
		int x_node, y_node;
		for (int i = 0; i < storages_amount; i++) {
			for (int j = 0; j < markets_amount; j++) {
				if (edges[i][j+storages_amount]->is_active) continue;
				cout << i << '+' << j << '=' << costs[i][j] + sums[i] - sums[j+storages_amount] << '(' << costs[i][j] << ")|"; //DEBUG
				if (costs[i][j] + sums[i] - sums[j+storages_amount] >= 0) continue;
				cout << "X";
				x_node = i, y_node = j + storages_amount, is_stale = false;
				break; 
			}
			if (!is_stale) break;
		}
		//DEBUG-start
		cout << "\nparents= ";
		for (int i = 0; i < nodes_amount; i++) cout << parents[i].first << ' ';
		cout << endl;
		//DEBUG-end
		if (is_stale) break;
		//DEBUG-start
		cout << x_node << '+' << y_node-storages_amount << '=' << endl;
		cout << costs[x_node][y_node-storages_amount] << '+' << sums[x_node] << '-' << sums[y_node]; 
		//DEBUG-end

		int zx_node = x_node;
		while (parents[zx_node].second > parents[y_node].second) {
			zx_node = parents[zx_node].first;
		}
		int zy_node = y_node;
		while (parents[x_node].second < parents[zy_node].second) {
			zy_node = parents[zy_node].first;
		}
		while (zx_node != zy_node) {
			zx_node = parents[zx_node].first;
			zy_node = parents[zy_node].first;
		}
		
		int min_flow = -1;
		for (int cur_node = x_node, par_node = parents[cur_node].first; cur_node != zx_node; cur_node = par_node, par_node = parents[par_node].first) {
			if (cur_node < storages_amount) continue;
			int cur_flow = edges[cur_node][par_node]->flow;
			if (min_flow == -1 || min_flow > cur_flow) min_flow = cur_flow;
		}
		for (int cur_node = y_node, par_node = parents[cur_node].first; cur_node != zy_node; cur_node = par_node, par_node = parents[par_node].first) {
			if (cur_node < storages_amount) continue;
			int cur_flow = edges[cur_node][par_node]->flow;
			if (min_flow == -1 || min_flow > cur_flow) min_flow = cur_flow;
		}

		cout << endl << min_flow; //DEBUG

		edge* new_edge = edges[x_node][y_node];
		new_edge->is_active = true;
		new_edge->flow += min_flow;

		bool not_disabled = true;
		for (int cur_node = x_node, par_node = parents[cur_node].first; cur_node != zx_node; cur_node = par_node, par_node = parents[par_node].first) {
			if (cur_node < storages_amount) {
				edges[cur_node][par_node]->flow += min_flow;
				continue;
			}
			edge* cur_edge = edges[cur_node][par_node];
			cur_edge->flow -= min_flow;
			if (not_disabled && cur_edge->flow == 0) {
				cur_edge->is_active = false;
				not_disabled = false;
			}
		}
		for (int cur_node = y_node, par_node = parents[cur_node].first; cur_node != zy_node; cur_node = par_node, par_node = parents[par_node].first) {
			if (cur_node < storages_amount) {
				edges[cur_node][par_node]->flow += min_flow;
				continue;
			}
			edge* cur_edge = edges[cur_node][par_node];
			cur_edge->flow -= min_flow;
			if (not_disabled && cur_edge->flow == 0) {
				cur_edge->is_active = false;
				not_disabled = false;
			}
		}

		//DEBUG-start
		int result = 0;
		for (int i = 0; i < storages_amount; i++) {
			for (int j = storages_amount; j < nodes_amount; j++) {
				result += edges[i][j]->cost * edges[i][j]->flow;
			}
		}
		cout << endl << result << endl; 
		//DEBUG-end
	}

	cout << "\n\nEnding:" << endl; //DEBUG
	int result = 0;
	for (int i = 0; i < storages_amount; i++) {
		for (int j = storages_amount; j < nodes_amount; j++) {
			cout << edges[i][j]->cost << '|' << i << ' ' << j-storages_amount << ' ' << edges[i][j]->flow << '\n'; //DEBUG
			result += edges[i][j]->cost * edges[i][j]->flow;
		}
	}
	cout << endl; //DEBUG

	cout << result << endl;

	for (int i = 0; i < markets_amount + storages_amount; i++) {
		for (int j = storages_amount; j < nodes_amount; j++) delete edges[i][j];
	}
	delete[] edges;
	delete[] storages;
	delete[] markets;
	delete[] costs;
	delete[] sums;
	delete[] parents;
	delete[] visited;

	return result;
}


int main() {
	cin.tie(0);
	ios_base::sync_with_stdio(false);
	int res = solve();
	int ans;
	cin >> ans;
	cout << (ans == res ? "ok" : "WRONG ANSWER");
	flush(cout);
	return 0;
}