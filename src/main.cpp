
#include "processing_graph.h"

using namespace std;
using namespace processing_graph;


Nodes buildAdjMatrix(const vector<string> &nodes)
{
    Nodes v = {};

    for(std::size_t i = 0; i< nodes.size(); i++)
        v[nodes.at(i)] = i;

    return v;
}

void TestFunction(const Nodes &nodes, const vector<Edge> &edges, const string &fname)
{
    ProcessingGraph graph_f = ProcessingGraph(nodes, edges);

    {
        cout << "Test 1: Ordering----------------------------------------------------------------------------------------------------------------------------" << endl;
        auto ordering = graph_f.get_ordering();
        cout << "Result Test 1.1: Ordering:" << endl;
        for(auto fn: ordering.first)
            cout << fn << " ";
        cout << endl;

        cout << "Result Test 1.2: These items can be processed in parallel (items with same index):" << endl;
        for(auto nodes: ordering.second)
            cout << "(" << nodes.first << "," << nodes.second << ") ";
        cout << endl;
        cout << "--------------------------------------------------------------------------------------------------------------------------------------------" << endl << endl;
    }

    {
        cout << "Test 2: Cyclic Dependency----------------------------------------------------------------------------------------------------------------" << endl;
            bool has_cyclic_dep = graph_f.has_cyclic_dependency();
        if (has_cyclic_dep)
            cout << "Result Test 2: The graph has at least one cyclic dependency." << endl;
        else
            cout << "Result Test 2: The graph does not have cyclic dependency." << endl;
        cout << "-----------------------------------------------------------------------------------------------------------------------------------------" << endl << endl;
    }

    {        
        cout << "Test 3: Impact due dependency---------------------------------------------------------------------------------------------------------------" << endl;
        auto diff_calc = graph_f.impact(nodes.at(fname));
        cout << "A change to '" << fname << "' will impact on: ";
        for(auto n: diff_calc)
            cout << n << " ";
        cout << endl;
        cout << "-----------------------------------------------------------------------------------------------------------------------------------------" << endl << endl;
    }
}



int main(int, char*[])
{
    {
        cout << "Test A:" << endl << endl;
        const vector<string> nodes_names = { {"Va"}, {"Vb"}, {"Vc"}, {"Vd"}, {"Ve"} };
        const Nodes nodes = buildAdjMatrix(nodes_names);

        const vector<Edge> edges = { Edge(nodes.at("Va"), nodes.at("Vb")), Edge(nodes.at("Vb"), nodes.at("Vc")), Edge(nodes.at("Vc"), nodes.at("Vd")), Edge(nodes.at("Vd"), nodes.at("Ve")) };

        const string change_on = "Vc";

        TestFunction(nodes, edges, change_on);
    }

    cout << endl << endl;

    {
        cout << "Test B:" << endl << endl;
        const vector<string> nodes_names = { {"Va"}, {"Vb"}, {"Vc"}, {"Vd"}, {"Ve"}, {"Vf"}, {"Vg"}, {"Vh"}, {"Vi"}, {"Vj"}, {"Vk"}, {"Vl"}, {"Vm"}, {"Vn"}, {"Vo"} };
        Nodes nodes = buildAdjMatrix(nodes_names);

        vector<Edge> edges = { Edge(nodes.at("Va"), nodes.at("Vb")), Edge(nodes.at("Vb"), nodes.at("Vc")), Edge(nodes.at("Vc"), nodes.at("Vd")), Edge(nodes.at("Vd"), nodes.at("Ve")) };

        const string change_on = "Vc";

        TestFunction(nodes, edges, change_on);
    }

    return 0;
}