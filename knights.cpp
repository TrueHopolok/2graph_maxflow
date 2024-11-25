#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
// #include <cassert> //DEBUG

using namespace std;


struct edge {
    int flow;
    int goal;
};


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
    // for (int i = 0; i < nodes_amount; i++) edges[i] = unordered_map<int, edge*>();

    {
        vector<bool> board_tiles(tiles_total);
        for (int f = 0; f < removed_amount; f++) {
            int x, y, p;
            cin >> x >> y;
            p = (x-1) * board_size + (y-1);
            board_tiles[p] = true;
        }

        unordered_map<int, int> start_tiles, end_tiles;
        int i = 1;

        for (int p = 0; p < tiles_total; p += 2) {
            if (board_tiles[p]) continue;
            start_tiles[p] = i;
            edge *tmp_edge = new edge{0, i};
            edges[start_node][i] = tmp_edge;
            edges[i][start_node] = tmp_edge; 
            i++;
        }

        for (int p = 1; p < tiles_total; p += 2) {
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
                int j = end_tiles[b];
                edge *tmp_edge = new edge{0, j};
                edges[i][j] = tmp_edge;
                edges[j][i] = tmp_edge;
            }
        }
    }

    // //DEBUG-start
    // for (int i = 0; i < edges.size(); i++) {
    //     for (auto p : edges[i]) {
    //         if (i == p.second->goal) continue;
    //         cout << i << ' ' << p.first << '\n';
    //     }
    // }
    // cout << '\n';
    // //DEBUG-end

    int max_flow = 0;
    cout << tiles_total - max_flow;
}


int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(false);
    solve();
    flush(cout);
    return 0;
}