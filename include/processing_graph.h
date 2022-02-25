#ifndef PROCESSING_GRAPH_H
#define PROCESSING_GRAPH_H

#include <iostream>
#include <iterator>
#include <algorithm>
#include <time.h>
#include <unordered_map>
#include <stdlib.h>
#include <cstring>

#include <boost/utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <iterator>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/visitors.hpp>

namespace processing_graph
{
    using namespace boost;
    using namespace std;

    typedef std::pair< int, int > Edge;
    typedef std::map<std::string, std::size_t> Nodes; //Maps variable name --> adjacency matrix index.

    class ProcessingGraph
    {
        typedef adjacency_list< vecS, vecS, bidirectionalS > Graph;
        typedef graph_traits< Graph >::vertex_descriptor Vertex;
        typedef list< Vertex > MakeOrder;

    private:
        Graph g;
        vector<string> var_name;
        vector<Edge> edges;
        std::size_t N;
        pair<vector<string>, vector<pair<string, int>>> ordering;

    public:
        ProcessingGraph(const Nodes f, const vector<Edge> edges): edges(edges)
        {
            Edge used_by[edges.size()];
            copy(edges.begin(), edges.end(), used_by);
            std::size_t nedges = edges.size();

            var_name = vector<string>(f.size(), "");
            for(auto m: f)
                var_name[m.second] = m.first;

            N = var_name.size();
            g = Graph(used_by, used_by + nedges, N);

            ordering = calculate_ordering();
        }

        ProcessingGraph()
        {
            var_name = {};
            edges = {};
            N = 0;
            ordering = {};
        }

        void operator = (const ProcessingGraph &PG )
        { 
            g = PG.g;
            var_name = PG.var_name;
            edges = PG.edges;
            N = PG.N;
            ordering = PG.ordering;
        }

        pair<vector<string>, vector<pair<string, int>>> get_ordering()
        {
            return ordering;
        }

        //If a item is changed determines what others dependents are affected.
        vector<string> impact(const std::size_t index)
        {
            vector<string> output = {};

            print_visitor vis(var_name);
            breadth_first_search(g, vertex(index, g), visitor(vis));

            auto vect = vis.GetVector();
            for(auto v: vect)
                output.push_back(var_name[v]);

            return output;
        }

        //Detects if there are circular dependencies.
        bool has_cyclic_dependency()
        {
            bool has_cycle = false;
            cycle_detector vis(has_cycle);
            depth_first_search(g, visitor(vis));

            return has_cycle;
        }

        struct print_visitor : public bfs_visitor<>
        {
            print_visitor(const vector<string>& var_name) : m_f_name(var_name), vv(new std::vector<Vertex>()) {}

            template < class Vertex, class Graph >
            void discover_vertex(Vertex v, Graph&)
            {
                ////cout << m_f_name[v] << " ";
                vv->push_back(v);
            }

            public:
                std::vector<Vertex>& GetVector() const  { return *vv; }

            protected:
                const vector<string>& m_f_name;
                boost::shared_ptr< std::vector<Vertex> > vv;
        };

        struct cycle_detector : public dfs_visitor<>
        {
            cycle_detector(bool& has_cycle) : m_has_cycle(has_cycle) {}

            template < class Edge, class Graph > void back_edge(Edge, Graph&)
            {
                m_has_cycle = true;
            }

            protected:
                bool& m_has_cycle;
        };

    private:
        //Determines the ordering where functions that can be calculated in parallel.
        //Returns:
            //first: vector with the calculation ordering;
            //second: vector of pairs with the parallel calculation ordering; functions with the same sloth number can be calculated in parallel.
        pair<vector<string>, vector<pair<string, int>>> calculate_ordering()
        {
            vector<string> calculation_ordering = {};
            vector<pair<string, int>> parallel_ordering = {}; //Pair: (function name, slot number)

            MakeOrder::iterator i;
            MakeOrder make_order;

            //Handle here the exception "terminate called after throwing an instance of 'boost::wrapexcept<boost::not_a_dag>' what():  The graph must be a DAG."
            //Note1: a DAG graph has each edge directed from one vertex to another, such that there is no way to start at any vertex v and follow a consistently-directed sequence of edges that eventually loops back to v again.
            //Note2: Equivalently, a DAG is a directed graph that has a topological ordering, a sequence of the vertices such that every edge is directed from earlier to later in the sequence.
            try
            {
                topological_sort(g, std::front_inserter(make_order));
            }
            catch(const std::exception& e)
            {
                cout << "A cyclic dependency was found: " << e.what();
            }

            for (i = make_order.begin(); i != make_order.end(); ++i)
                calculation_ordering.push_back(var_name[*i]);

            // Parallel compilation ordering
            std::vector< int > time(N, 0);
            for (i = make_order.begin(); i != make_order.end(); ++i)
            {
                // Walk through the in_edges an calculate the maximum time.
                if (in_degree(*i, g) > 0)
                {
                    Graph::in_edge_iterator j, j_end;
                    int maxdist = 0;
                    // Through the order from topological sort, we are sure that every time we are using here is already initialized.
                    for (boost::tie(j, j_end) = in_edges(*i, g); j != j_end; ++j)
                        maxdist = (std::max)(time[source(*j, g)], maxdist);
                    time[*i] = maxdist + 1;
                }
            }

            //Parallel calculation ordering; vertices/functions with same group number can be made in parallel.
            {
                graph_traits< Graph >::vertex_iterator i, iend;
                for (boost::tie(i, iend) = vertices(g); i != iend; ++i)
                {
                    ////cout << "time_slot[" << var_name[*i] << "] = " << time[*i] << endl;
                    parallel_ordering.push_back(make_pair(var_name[*i], time[*i]));
                }
            }

            return make_pair(calculation_ordering, parallel_ordering);
        }
    };
}

#endif /*PROCESSING_GRAPH_H*/