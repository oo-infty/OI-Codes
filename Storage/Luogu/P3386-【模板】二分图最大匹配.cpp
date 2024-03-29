#include <iostream>
#include <vector>
using namespace std;

constexpr int MAX_N = 500 + 10;

int n, m, e;
vector<int> graph[MAX_N];
bool vis[MAX_N];
int match[MAX_N];

void link(int x, int y) {
    graph[x].push_back(y);
}

bool augment(int x) {
    for (int y : graph[x]) {
        if (vis[y])
            continue;
        
        vis[y] = true;

        if (!match[y] || augment(match[y])) {
            match[y] = x;
            return true;
        }
    }

    return false;
}

int hungary() {
    int res = 0;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j)
            vis[j] = false;
        
        if (augment(i))
            ++res;
    }

    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin >> n >> m >> e;

    for (int i = 1; i <= e; ++i) {
        int x, y;
        cin >> x >> y;
        link(x, y);
    }

    cout << hungary() << endl;
    return 0;
}