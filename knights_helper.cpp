#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, m, x, y, p;
    cin >> n >> m;
    vector<bool> a(n*n);
    for (; m > 0; m--) {
        cin >> x >> y;
        p = (x-1)*n + y-1;
        a[p] = true;
    }
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            p = x*n + y;
            if (a[p]) continue;
            cout << x+1 << ' ' << y+1 << endl;
        }
    }
}