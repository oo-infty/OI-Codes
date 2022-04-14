#include <iostream>
#include <queue>
#include <tuple>

constexpr int MAX_N = 2000 + 10;
constexpr long long INFINITY = 0x3f3f3f3f3f3f3f3f;

class Network {
public:
    void init(int n) {
        tot = 1;
        source = 0;
        sink = n + 1;
    }

    void link(int x, int y, int c, int w) {
        net[++tot] = {head[x], y, c, w};
        head[x] = tot;
        net[++tot] = {head[y], x, 0, -w};
        head[y] = tot;
    }

    int getSource() { return source; }

    int getSink() { return sink; }

    long long maxflow() {
        long long res = 0;

        while (spfa())
            dfs(source, 0x3f3f3f3f, res);

        return res;
    }

private:
    struct Edge {
        int next, to, capacity, weight;
    };

    Edge net[MAX_N * MAX_N];
    int tot, head[MAX_N], cur[MAX_N];
    int source, sink;
    bool vis[MAX_N];
    long long dis[MAX_N];

    bool spfa() {
        std::queue<int> q;

        for (int i = source; i <= sink; ++i) {
            vis[i] = false;
            dis[i] = -INFINITY;
        }

        dis[source] = 0;
        q.push(source);

        while (!q.empty()) {
            int x = q.front();
            q.pop();
            vis[x] = false;
            cur[x] = head[x];

            for (int i = head[x], y; y = net[i].to, i; i = net[i].next) {
                if (net[i].capacity <= 0 || dis[y] >= dis[x] + net[i].weight)
                    continue;

                dis[y] = dis[x] + net[i].weight;

                if (!vis[y]) {
                    vis[y] = true;
                    q.push(y);
                }
            }
        }

        return dis[sink] != -INFINITY;
    }

    int dfs(int x, int in, long long &cost) {
        if (x == sink)
            return in;

        int res = 0;
        vis[x] = true;

        for (int &i = cur[x], y; y = net[i].to, i; i = net[i].next) {
            if (vis[y] || net[i].capacity <= 0 || dis[y] != dis[x] + net[i].weight)
                continue;

            int f = dfs(y, std::min(net[i].capacity, in - res), cost);

            if (f > 0) {
                net[i].capacity -= f;
                net[i ^ 1].capacity += f;
                res += f;
                cost += 1ll * f * net[i].weight;

                if (res == in)
                    break;
            }
        }

        if (res == in)
            vis[x] = false;

        return res;
    }
};

int n;
Network net;
std::tuple<int, int, int> male[MAX_N];
std::tuple<int, int, int> female[MAX_N];

namespace Subtask1 {
    void main() {
        net.init(2 * n);

        for (int i = 1; i <= n; ++i) {
            auto [x, y, c] = male[i];
            net.link(net.getSource(), i, c, 0);
        }

        for (int i = 1; i <= n; ++i) {
            auto [x, y, c] = female[i];
            net.link(i + n, net.getSink(), c, 0);
        }

        for (int i = 1; i <= n; ++i) {
            int x1 = std::get<0>(male[i]), y1 = std::get<1>(male[i]);

            for (int j = 1; j <= n; ++j) {
                int x2 = std::get<0>(female[j]), y2 = std::get<1>(female[j]);
                net.link(i, j + n, 0x3f3f3f3f, std::abs(x1 - x2) + std::abs(y1 - y2));
            }
        }

        std::cout << net.maxflow() << std::endl;
    }
} // namespace Subtask1

namespace Subtask2 {
    void main() {
        net.init(2 * n + 4);

        for (int i = 1; i <= n; ++i) {
            auto [x, y, c] = male[i];
            net.link(net.getSource(), i, c, 0);
            net.link(i, 2 * n + 1, 0x3f3f3f3f, x + y);
            net.link(i, 2 * n + 2, 0x3f3f3f3f, x - y);
            net.link(i, 2 * n + 3, 0x3f3f3f3f, -x + y);
            net.link(i, 2 * n + 4, 0x3f3f3f3f, -x - y);
        }

        for (int i = 1; i <= n; ++i) {
            auto [x, y, c] = female[i];
            net.link(i + n, net.getSink(), c, 0);
            net.link(2 * n + 4, i + n, 0x3f3f3f3f, x + y);
            net.link(2 * n + 3, i + n, 0x3f3f3f3f, x - y);
            net.link(2 * n + 2, i + n, 0x3f3f3f3f, -x + y);
            net.link(2 * n + 1, i + n, 0x3f3f3f3f, -x - y);
        }

        std::cout << net.maxflow() << std::endl;
    }
} // namespace Subtask2

int main() {
#ifdef ONLINE_JUDGE
    std::freopen("Diana.in", "r", stdin);
    std::freopen("Diana.out", "w", stdout);
#else
    std::freopen("project.in", "r", stdin);
    std::freopen("project.out", "w", stdout);
#endif
    std::ios::sync_with_stdio(false);

    std::cin >> n;

    for (int i = 1; i <= n; ++i) {
        auto &[x, y, c] = male[i];
        std::cin >> x >> y >> c;
    }

    for (int i = 1; i <= n; ++i) {
        auto &[x, y, c] = female[i];
        std::cin >> x >> y >> c;
    }

    if (n <= 200)
        Subtask1::main();
    else
        Subtask2::main();

    return 0;
}
