#pragma once
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

public:
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
            if (dists[v2] > dists[v1] + weight)
                return dists[v1] + weight;
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
