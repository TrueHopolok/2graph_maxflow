#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
// #include <cassert> //DEBUG
#define forever while (1)
using namespace std;


struct edge {
    int flow;
    int goal;
};


struct visit {
    int parent;
    int current;
    int dist; 
};


int slow_method(vector<unordered_map<int, edge*>> &edges, const int &nodes_amount, const int &start_node, const int &end_node) {
    // DEBUG-start
    // Reset flow in graph
    for (const unordered_map<int, edge*> &m : edges) {
        for (pair<int, edge*> p : m) {
            p.second->flow = 0;
        }
    }
    //DEBUG-end
    int max_flow = 0;
    forever {
        unordered_set<int> visited;
        queue<pair<int, int>> visit_queue;
        visit_queue.push(pair<int, int>{start_node, start_node});
        vector<int> parents(nodes_amount, -1);
        while (!visit_queue.empty()) {
            pair<int, int> visit_pair = visit_queue.front();
            visit_queue.pop();
            int cur_node = visit_pair.first, par_node = visit_pair.second;
            if (visited.count(cur_node)) continue;
            visited.insert(cur_node);
            parents[cur_node] = par_node;
            if (cur_node == end_node) break;
            for (const pair<int, edge*> &p : edges[cur_node]) {
                int next_node = p.first;
                if (visited.count(next_node)) continue;
                edge *cur_edge = p.second;
                int cur_flow = cur_edge->flow;
                if (cur_edge->goal == cur_node) cur_flow = cur_flow == 1 ? 0 : 1;
                if (cur_flow == 1) continue;
                visit_queue.push(pair<int, int>{next_node, cur_node}); 
            }
        }
        if (parents[end_node] == -1) break;
        max_flow++;
        for (int cur_node = end_node, par_node = parents[cur_node]; cur_node != start_node; cur_node = par_node, par_node = parents[par_node]) {
            edge *cur_edge = edges[cur_node][par_node];
            int cur_flow = cur_edge->flow == 1 ? 0 : 1;
            cur_edge->flow = cur_flow;
        }
    }
    return max_flow;
}


vector<int> get_possible_pos(int p, int n) {
    int x = p / n, y = p % n;
    vector<int> pos;
    if (y - 2 >= 0) {
        if (x - 1 >= 0) pos.push_back((y-2) + (x-1)*n);
        if (x + 1 < n)  pos.push_back((y-2) + (x+1)*n); 
    } 
    if (y - 1 >= 0) {
        if (x - 2 >= 0) pos.push_back((y-1) + (x-2)*n);
        if (x + 2 < n)  pos.push_back((y-1) + (x+2)*n); 
    }
    if (y + 1 < n) {
        if (x - 2 >= 0) pos.push_back((y+1) + (x-2)*n);
        if (x + 2 < n)  pos.push_back((y+1) + (x+2)*n); 
    }
    if (y + 2 < n) {
        if (x - 1 >= 0) pos.push_back((y+2) + (x-1)*n);
        if (x + 1 < n)  pos.push_back((y+2) + (x+1)*n); 
    }
    return pos;
}


int fast_method(vector<unordered_map<int, edge*>> &edges, const int &nodes_amount, const int &start_node, const int &end_node) {
    int max_flow = 0;
    forever {
        int end_dist = -1; 
        {   //BFS
            unordered_set<int> visited;
            queue<pair<int, int>> visit_queue;
            visit_queue.push(pair<int, int>{start_node, 0}); 
            while (!visit_queue.empty()) {
                pair<int, int> pp = visit_queue.front();
                visit_queue.pop();
                int cur_node = pp.first, cur_dist = pp.second;
                if (visited.count(cur_node)) continue;
                visited.insert(cur_node);
                if (cur_node == end_node) {
                    end_dist = cur_dist;
                    break;
                }
                cur_dist++;
                for (const pair<int, edge *> &pp : edges[cur_node]) {
                    int next_node = pp.first;
                    if (visited.count(next_node)) continue;
                    edge *cur_edge = pp.second;
                    int cur_flow = cur_edge->flow;
                    if (cur_edge->goal == cur_node) cur_flow = cur_flow == 1 ? 0 : 1;
                    if (cur_flow == 1) continue;
                    visit_queue.push(pair<int, int>{next_node, cur_dist});
                }
            }
        }
        // int old_max_flow = max_flow; //DEBUG
        if (end_dist == -1) break;
        {   //DFS
            unordered_map<int, int> visited_dist;
            vector<visit> visit_stack;
            visit_stack.push_back(visit{start_node, start_node, 0});
            vector<int> parents(nodes_amount, -1);
            while (!visit_stack.empty()) {
                visit vv = visit_stack.back();
                visit_stack.pop_back();
                int 
                cur_node = vv.current, 
                par_node = vv.parent,
                cur_dist = vv.dist;
                if (visited_dist.count(cur_node)) if (visited_dist[cur_node] <= cur_dist) continue;
                parents[cur_node] = par_node;
                visited_dist[cur_node] = cur_dist;
                if (cur_node == end_node) {
                    for (; cur_node != start_node; cur_node = par_node, par_node = parents[par_node]) {
                        edge *cur_edge = edges[cur_node][par_node];
                        int cur_flow = cur_edge->flow == 1 ? 0 : 1;
                        cur_edge->flow = cur_flow;
                    }
                    max_flow++;
                    visited_dist.clear();
                    visit_stack.clear();
                    visit_stack.push_back(visit{start_node, start_node, 0});
                    continue;
                } else if (cur_dist >= end_dist) continue;
                cur_dist++;
                for (const pair<int, edge*> &pp : edges[cur_node]) {
                    int next_node = pp.first;
                    if (cur_dist >= end_dist) if (next_node != end_node) continue;
                    else if (visited_dist.count(next_node)) if (visited_dist[next_node] <= cur_dist) continue;
                    edge *cur_edge = pp.second;
                    int cur_flow = cur_edge->flow;
                    if (cur_edge->goal == cur_node) cur_flow = cur_flow == 1 ? 0 : 1;
                    if (cur_flow == 1) continue;
                    visit_stack.push_back(visit{cur_node, next_node, cur_dist});
                }
            }
            // assert(old_max_flow < max_flow); //DEBUG
        }
    }
    return max_flow;
}


void solve() {
    int board_size, removed_amount;
    cin >> board_size >> removed_amount;
    
    const int
    tiles_total = board_size * board_size,
    tiles_amount = tiles_total - removed_amount,
    nodes_amount = tiles_amount + 2,
    start_node = 0,
    end_node = nodes_amount - 1;
    vector<unordered_map<int, edge*>> edges(nodes_amount);

    {
        vector<bool> board_tiles(tiles_total);
        for (int f = 0; f < removed_amount; f++) {
            int x, y, p;
            cin >> x >> y;
            p = (x-1) + board_size * (y-1);
            board_tiles[p] = true;
        }

        unordered_map<int, int> start_tiles, end_tiles;
        int i = 1;

        for (int tmp_p = 0; tmp_p < tiles_total; tmp_p += 2) {
            int p = tmp_p;
            if (((board_size&1) == 0) ) p += (p/board_size)&1;
            if (board_tiles[p]) continue;
            start_tiles[p] = i;
            edge *tmp_edge = new edge{0, i};
            edges[start_node][i] = tmp_edge;
            edges[i][start_node] = tmp_edge; 
            i++;
        }

        for (int tmp_p = 1; tmp_p < tiles_total; tmp_p += 2) {
            int p = tmp_p;
            if (((board_size&1) == 0) ) p -= (p/board_size)&1;
            if (board_tiles[p]) continue;
            end_tiles[p] = i;
            edge *tmp_edge = new edge{0, end_node};
            edges[end_node][i] = tmp_edge;
            edges[i][end_node] = tmp_edge; 
            i++;
        }

        // assert(i == end_node); //DEBUG

        for (const pair<int, int> &pp : start_tiles) {
            int p = pp.first, i = pp.second;
            for (const int &b : get_possible_pos(p, board_size)) {
                if (board_tiles[b]) continue;
                // assert(end_tiles.count(b)); //DEBUG
                int j = end_tiles[b];
                edge *tmp_edge = new edge{0, j};
                edges[i][j] = tmp_edge;
                edges[j][i] = tmp_edge;
            }
        }
    }

    // //DEBUG-start
    // cout << "GRAPH itself:" << endl;
    // for (int i = 0; i < edges.size(); i++) {
    //     for (auto p : edges[i]) {
    //         if (i == p.second->goal) continue;
    //         cout << i << ' ' << p.first << '\n';
    //     }
    // }
    // cout << '\n';
    // //DEBUG-end

    int max_flow = fast_method(edges, nodes_amount, start_node, end_node);
    // //DEBUG-start
    // int slow_flow = slow_method(edges, nodes_amount, start_node, end_node);
    // cout << max_flow << ' ' << slow_flow << endl;
    // assert(max_flow == slow_flow);
    // //DEBUG-end
    cout << tiles_amount - max_flow << endl;
}


int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(false);
    solve();
    flush(cout);
    return 0;
}