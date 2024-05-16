#pragma once

#include "queue_linked_list.h"
#include <unordered_map>
#include <vector>

const int inf = 10e10;

template<typename tag_t>
class oriented_graph {

    typedef std::unordered_map<tag_t, std::vector<tag_t>> vertices_map_t;
    typedef std::unordered_map<std::pair<tag_t, tag_t>, int64_t> weights_map_t;
    typedef std::unordered_map<tag_t, int64_t> distances_map_t;

    vertices_map_t vertices = {};
    weights_map_t weights = {};
    std::vector<int> distances;
    size_t amound_vertices;
    alg_ds::queue<tag_t> que;
    std::unordered_map<tag_t, bool> in_que;


public:
    auto map_init = {}(auto pair){return {pair.first, false};};

    oriented_graph(vertices_map_t vertices_, weights_map_t weights_) :
        vertices(vertices_), weights(weights_) {
        amound_vertices = vertices.size();
    }

    std::vector<int64_t> spfa(tag_t src, oriented_graph* graph) {

        distances.assign(amound_vertices, inf);
        distances[src] = 0;

        for (auto& p : graph->vertices) in_que[p.first];

        que.insert(src);
        in_que[src] = true;

        while (!que.is_empty()) {
            tag_t first = que.peek();
            que.remove();
            in_q[first] = false;
            bellman_ford(first);
        }
    }

    void add_edge(tag_t v1, tag_t v2, int64_t cost) {
        if (vertices.find(v1) == vertices.end() ||
            vertices.find(v2) == vertices.end()) return;

        vertices[v1].push_back(v2);
        weights[std::make_pair(v1, v2)] = cost;
        return;
    }

    void add_vertice(tag_t v) {
        if (vertices.find(v) != vertices.end()) return;
        vertices[v] = {};
    }

    int64_t relax_edge(std::pair<tag_t, tag_t> nodes, int64_t weight, distances_map_t &dists) {
        tag_t v1 = nodes.first, v2 = nodes.second;

        if (dists[v1] < inf) {
            if (dists[v2] > dists[v1] + weight) {

                if (!in_que[v2]) {
                 que.insert(v2);
                 in_que[v2] = true;
                }
            return dists[v1] + weight;
            }
        }
        return dists[v2];
    }

    distances_map_t *bellman_ford(tag_t src) {
        distances_map_t dists = {};
        for (auto &v : vertices)
            dists[v.first] = inf;
        dists[src] = 0;
        for (size_t _ = 0; _ < vertices.size() - 1) {
            for (auto &e : weights) {
                relax_edge(e.first, e.second, dists);
            }
        }
    }
};


    // typedef std::unordered_map<tag_t, std::vector<tag_t>> vertices_map_t;
    // typedef std::unordered_map<std::pair<tag_t, tag_t>, int64_t> weights_map_t;
    // typedef std::unordered_map<tag_t, int64_t> distances_map_t;
// int main() {
//     int n = 5; // Number of nodes
//     vector<vector<pair<int, int>>> graph(n);
//     vector<int> distances(n);

//     // Sample graph
//     graph[0].push_back(make_pair(1, 5));
//     graph[0].push_back(make_pair(2, 3));
//     graph[1].push_back(make_pair(3, 6));
//     graph[2].push_back(make_pair(1, -2));
//     graph[2].push_back(make_pair(3, 7));
//     graph[3].push_back(make_pair(4, -4));

//     int source = 0; // Source node

//     SPFA(source, graph, distances);

//     cout << "Shortest distances from node " << source << " to other nodes:" << endl;
//     for (int i = 0; i < n; ++i) {
//         cout << "Node " << i << ": " << distances[i] << endl;
//     }

//     return 0;
// }
