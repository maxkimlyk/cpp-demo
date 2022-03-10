#include <type_traits>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <demo-common.h>

template <class Graph>
void PrintGraphInfo(const Graph g)
{
    auto index_map = get(boost::vertex_index, g);

    auto [v_begin, v_end] = vertices(g);
    std::cout << "vertices: ";
    for (auto it = v_begin; it != v_end; ++it)
    {
        std::cout << index_map[*it] << " ";
    }
    std::cout << std::endl;

    auto [e_begin, e_end] = edges(g);
    std::cout << "edges: ";
    for (auto it = e_begin; it != e_end; ++it)
    {
        std::cout << "(" << index_map[source(*it, g)] << " -> " << index_map[target(*it, g)] << ") ";
    }
    std::cout << std::endl;

    for (auto v_it = v_begin; v_it != v_end; ++v_it)
    {
        std::cout << "vertex " << index_map[*v_it] << std::endl;

        auto [oe_begin, oe_end] = out_edges(*v_it, g);
        std::cout << " out-edges: ";
        for (auto it = oe_begin; it != oe_end; ++it)
        {
            std::cout << "(" << index_map[source(*it, g)] << " -> " << index_map[target(*it, g)]
                      << ") ";
        }
        std::cout << std::endl;

        if constexpr (std::is_same_v<typename boost::graph_traits<Graph>::directed_category, boost::bidirectional_tag>)
        {
            auto [ie_begin, ie_end] = in_edges(*v_it, g);
            std::cout << " in-edges: ";
            for (auto it = ie_begin; it != ie_end; ++it)
            {
                std::cout << "(" << index_map[source(*it, g)] << " -> " << index_map[target(*it, g)]
                          << ") ";
            }
            std::cout << std::endl;
        }

        auto [av_begin, av_end] = adjacent_vertices(*v_it, g);
        std::cout << " adjacent vertices: ";
        for (auto it = av_begin; it != av_end; ++it)
        {
            std::cout << index_map[*it] << " ";
        }
        std::cout << std::endl;
    }
}

DEMO(adjacency_list)
{
    using Graph = boost::adjacency_list<
        /* OutEdgeListS = */ boost::vecS,
        /* VertexListS = */ boost::vecS,
        /* DirectedS = */ boost::bidirectionalS>;

    const size_t num_vertices(5);
    Graph g(num_vertices);

    add_edge(0, 1, g);
    add_edge(0, 2, g);
    add_edge(0, 3, g);
    add_edge(1, 0, g);
    add_edge(2, 0, g);
    add_edge(2, 3, g);
    add_edge(3, 4, g);
    add_edge(4, 1, g);

    PrintGraphInfo(g);
}

DEMO(adjacency_matrix)
{
    using Graph = boost::adjacency_matrix<
        /* Directed = */ boost::directedS>;

    const size_t num_vertices(5);
    Graph g(num_vertices);

    add_edge(0, 1, g);
    add_edge(0, 2, g);
    add_edge(0, 3, g);
    add_edge(1, 0, g);
    add_edge(2, 0, g);
    add_edge(2, 3, g);
    add_edge(3, 4, g);
    add_edge(4, 1, g);

    PrintGraphInfo(g);
}

DEMO(dijkstra)
{
    using Graph = boost::adjacency_list<
        /* OutEdgeListS = */ boost::listS,
        /* VertexListS = */ boost::vecS,
        /* DirectedS = */ boost::directedS,
        /* VertexProperty = */ boost::no_property,
        /* EdgeProperty = */ boost::property<boost::edge_weight_t, int>>;

    const int num_vertices = 5;
    const std::vector<std::pair<int, int>> edges = {
        {0, 1}, {0, 2}, {0, 3}, {1, 0}, {2, 0}, {2, 3}, {3, 4}, {4, 1},
    };
    const std::vector<int> weights = {1, 2, 1, 3, 2, 3, 3, 1};

    Graph g(edges.begin(), edges.end(), weights.data(), num_vertices);

    std::vector<int> distances(num_vertices);
    dijkstra_shortest_paths(g, *(vertices(g).first), boost::distance_map(distances.data()));

    for (size_t i = 0; i < num_vertices; ++i)
    {
        std::cout << "Distance 0 -> " << i << ": " << distances[i] << std::endl;
    }
}

RUN_DEMOS
