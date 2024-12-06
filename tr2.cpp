#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cassert>
#define forever while (1)
using namespace std;


struct edge {
	int flow;
	int cost;
	bool is_active = false;
};


struct visit {
    int cur;
    int par;
    int height; 
};


void solve() {
    int supply_amount, demand_amount;
    cin >> supply_amount >> demand_amount;

    vector<int> supply(supply_amount);
    for (int i = 0; i < supply_amount; i++) cin >> supply[i];

    vector<int> demand(demand_amount);
    for (int i = 0; i < demand_amount; i++) cin >> demand[i];

    vector<vector<int>> costs(supply_amount, vector<int>(demand_amount));
    for (int i = 0; i < supply_amount; i++) {
        for (int j = 0; j < demand_amount; j++) cin >> costs[i][j];
    }

    const int nodes_amount = supply_amount + demand_amount;
    vector<unordered_map<int, edge*>> edges(nodes_amount);
    for (int i = 0; i < supply_amount; i++) {
        for (int j = 0; j < demand_amount; j++) {
            edge* e = new edge{0, costs[i][j], false};
            edges[i][j+supply_amount] = e;
            edges[j+supply_amount][i] = e;
        }
    }

    for (int i = 0, j = 0; i < supply_amount && j < demand_amount;) {
        edge* e = edges[i][j+supply_amount];
        e->is_active = true;
        e->flow = min(supply[i], demand[j]);
        if (supply[i] < demand[j]) {
            demand[j] -= supply[i];
            i++;
        } else {
            supply[i] -= demand[j];
            j++;
        }
    }

    vector<int> heights(nodes_amount);
    vector<int> parents(nodes_amount);
    vector<int> sums(nodes_amount);
    queue<visit> visit_order;
    forever {
        for (int i = 0; i < nodes_amount; i++) {
            heights[i] = 0;
            parents[i] = 0;
            sums[i] = 0;
        }
        visit_order.push(visit{0, 0, 0});

        while (!visit_order.empty()) {
            visit p = visit_order.front();
            visit_order.pop();
            const int cur = p.cur, par = p.par, height = p.height + 1;
            if (heights[cur] != 0) continue;
            heights[cur] = height;
            parents[cur] = par;

            if (cur != par) {
                int cost = edges[cur][par]->cost;
                if (cur < supply_amount) cost = -cost;
                sums[cur] = sums[par] + cost;
            }
        
            for (const pair<int, edge*> &p : edges[cur]) {
                const int son = p.first;
                if (heights[son] != 0) continue;
                edge* e = edges[cur][son];
                if (!e->is_active) continue;
                visit_order.push(visit{son, cur, height});
            }
        }
        
        int x = -1, y = -1;
        for (int i = 0; i < supply_amount; i++) {
            for (int j = supply_amount; j < nodes_amount; j++) {
                if (edges[i][j]->is_active) continue;
                const int revenue = sums[i] - sums[j] + costs[i][j-supply_amount];
                if (revenue >= 0) continue;
                x = i, y = j;
                break;
            }
            if (x != -1) break;
        }
        if (x == -1) break;

        int zx = x, zy = y;
        while (heights[zx] != heights[zy]) {
            if (heights[zx] < heights[zy]) zy = parents[zy];
            else zx = parents[zx];
        }
        while (zx != zy) zx = parents[zx], zy = parents[zy];
        assert(zx == zy);

        int min_flow = -1;
        for (int cx = x, px = parents[x]; cx != zx; cx = px, px = parents[px]) {
            if (cx >= supply_amount) continue;
            const int flow = edges[cx][px]->flow;
            if (min_flow == -1 || min_flow > flow) min_flow = flow;
            assert(min_flow >= 0);
        }
        for (int cy = y, py = parents[y]; cy != zy; cy = py, py = parents[py]) {
            if (cy < supply_amount) continue;
            const int flow = edges[cy][py]->flow;
            if (min_flow == -1 || min_flow > flow) min_flow = flow;
            assert(min_flow >= 0);
        }
        assert(min_flow >= 0);

        edges[x][y]->flow += min_flow;
        edges[x][y]->is_active = true;

        bool not_disabled = true;
        for (int cx = x, px = parents[x]; cx != zx; cx = px, px = parents[px]) {
            int flow = min_flow;
            if (cx < supply_amount) flow = -flow;
            edges[cx][px]->flow += flow;
            assert(edges[cx][px]->flow >= 0);
            if (not_disabled && cx < supply_amount && edges[cx][px]->flow == 0) {
                edges[cx][px]->is_active = false;
                not_disabled = false;
            }
        }
        for (int cy = y, py = parents[y]; cy != zy; cy = py, py = parents[py]) {
            int flow = min_flow;
            if (cy >= supply_amount) flow = -flow;
            edges[cy][py]->flow += flow;
            assert(edges[cy][py]->flow >= 0);
            if (not_disabled && cy >= supply_amount && edges[cy][py]->flow == 0) {
                edges[cy][py]->is_active = false;
                not_disabled = false;
            }
        }
    }

    int result = 0;
    for (int i = 0; i < supply_amount; i++) {
        for (int j = supply_amount; j < nodes_amount; j++) {
            edge* e = edges[i][j];
            result += e->flow * e->cost;
        }
    }
    cout << result << endl;
    int real_res;
    cin >> real_res;
    if (real_res != result) cout << "WRONG ANSWER\n" << real_res << endl; 

    for (int i = 0; i < supply_amount; i++) {
		for (int j = supply_amount; j < nodes_amount; j++) delete edges[i][j];
	}
}


int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(false);
    solve();
    flush(cout);
    return 0;
}