#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <climits>
#include <algorithm>

using namespace std;

// 图的类定义
class Graph {
public:
    map<char, list<pair<char, int>>> adjList;  // 使用字符作为顶点，存储邻接表（包含边和权重）

    void addVertex(char vertex) {
        adjList[vertex];  // 默认创建一个空的邻接表
    }

    void addEdge(char u, char v, int weight) {
        adjList[u].push_back(make_pair(v, weight));
        adjList[v].push_back(make_pair(u, weight));  // 如果是有向图，删除这一行
    }

    void printGraph() {
        for (auto& pair : adjList) {
            cout << pair.first << ": ";
            for (auto& neighbor : pair.second) {
                cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
            }
            cout << endl;
        }
    }
};

// 广度优先搜索 (BFS)
void bfs(const Graph& graph, char start) {
    map<char, bool> visited;
    queue<char> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        char node = q.front();
        q.pop();
        cout << node << " ";

        for (auto& neighbor : graph.adjList.at(node)) {
            if (!visited[neighbor.first]) {
                visited[neighbor.first] = true;
                q.push(neighbor.first);
            }
        }
    }
    cout << endl;
}

// 深度优先搜索 (DFS)
void dfsUtil(const Graph& graph, char node, map<char, bool>& visited) {
    visited[node] = true;
    cout << node << " ";

    for (auto& neighbor : graph.adjList.at(node)) {
        if (!visited[neighbor.first]) {
            dfsUtil(graph, neighbor.first, visited);
        }
    }
}

void dfs(const Graph& graph, char start) {
    map<char, bool> visited;
    dfsUtil(graph, start, visited);
    cout << endl;
}

// Dijkstra 最短路径算法
struct Node {
    char vertex;
    int distance;
    bool operator>(const Node& other) const {
        return distance > other.distance;
    }
};

void dijkstra(const Graph& graph, char start) {
    map<char, int> distances;
    map<char, char> previous;
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    for (auto& pair : graph.adjList) {
        distances[pair.first] = INT_MAX;
    }

    distances[start] = 0;
    pq.push({ start, 0 });

    while (!pq.empty()) {
        char u = pq.top().vertex;
        pq.pop();

        for (auto& neighbor : graph.adjList.at(u)) {
            char v = neighbor.first;
            int weight = neighbor.second;
            int newDist = distances[u] + weight;

            if (newDist < distances[v]) {
                distances[v] = newDist;
                previous[v] = u;
                pq.push({ v, distances[v] });
            }
        }
    }

    // 打印最短路径
    for (auto& pair : distances) {
        cout << "从 " << start << " 到 " << pair.first << " 的最短路径长度为: " << pair.second << endl;
    }
}

// Kruskal 最小生成树算法

class DisjointSet {
public:
    map<char, char> parent;
    map<char, int> rank;

    DisjointSet(const vector<char>& vertices) {
        for (char v : vertices) {
            parent[v] = v;
            rank[v] = 0;
        }
    }

    char find(char x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // 路径压缩
        }
        return parent[x];
    }

    void unionSets(char x, char y) {
        char rootX = find(x);
        char rootY = find(y);

        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            }
            else if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            }
            else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }
};

struct Edge {
    char u, v;
    int weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

void kruskal(const Graph& graph) {
    vector<Edge> edges;
    vector<char> vertices;
    for (auto& pair : graph.adjList) {
        vertices.push_back(pair.first);
        for (auto& neighbor : pair.second) {
            if (pair.first < neighbor.first) {  // 保证每条边只添加一次
                edges.push_back({ pair.first, neighbor.first, neighbor.second });
            }
        }
    }

    sort(edges.begin(), edges.end());

    DisjointSet ds(vertices);
    vector<Edge> mst;

    for (Edge& edge : edges) {
        if (ds.find(edge.u) != ds.find(edge.v)) {
            mst.push_back(edge);
            ds.unionSets(edge.u, edge.v);
        }
    }

    cout << "最小生成树的边: " << endl;
    for (Edge& edge : mst) {
        cout << "(" << edge.u << ", " << edge.v << ") 权重: " << edge.weight << endl;
    }
}

int main() {
    // 创建图
    Graph g;
    g.addVertex('A');
    g.addVertex('B');
    g.addVertex('C');
    g.addVertex('D');
    g.addVertex('E');
    g.addVertex('F');

    // 添加边
    g.addEdge('A', 'B', 1);
    g.addEdge('A', 'C', 4);
    g.addEdge('B', 'C', 2);
    g.addEdge('B', 'D', 5);
    g.addEdge('C', 'D', 1);
    g.addEdge('D', 'F', 3);

    // 打印图
    cout << "图的邻接表:" << endl;
    g.printGraph();

    // 测试 BFS 和 DFS
    cout << "BFS Traversal starting from 'A': ";
    bfs(g, 'A');
    cout << "DFS Traversal starting from 'A': ";
    dfs(g, 'A');

    // 测试 Dijkstra 最短路径算法
    cout << "Dijkstra Shortest Path from 'A': " << endl;
    dijkstra(g, 'A');

    // 测试 Kruskal 最小生成树算法
    cout << "Kruskal's Minimum Spanning Tree: " << endl;
    kruskal(g);

    return 0;
}
