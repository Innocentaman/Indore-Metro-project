#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <stack>
#include <list>
#include <limits>
#include <iomanip>
using namespace std;

class Graph_M {
private:
    // Structure for each vertex in the graph
    struct Vertex {
        unordered_map<string, int> nbrs; // Adjacency list of neighbors with edge weights
    };

    unordered_map<string, Vertex> vtces; // Map of vertices

public:
    // Constructor
    Graph_M() {}

    // Get number of vertices
    int numVertices() {
        return vtces.size();
    }

    // Check if vertex exists
    bool containsVertex(string vname) {
        return vtces.find(vname) != vtces.end();
    }

    // Add a vertex
    void addVertex(string vname) {
        vtces[vname] = Vertex();
    }

    // Remove a vertex
    void removeVertex(string vname) {
        Vertex vtx = vtces[vname];
        vector<string> keys;

        // Remove edges from neighbors
        for (auto& kv : vtx.nbrs) {
            string key = kv.first;
            vtces[key].nbrs.erase(vname);
            keys.push_back(key);
        }

        // Remove vertex itself
        vtces.erase(vname);
    }

    // Get number of edges
    int numEdges() {
        int count = 0;
        for (auto& kv : vtces) {
            count += kv.second.nbrs.size();
        }
        return count / 2; // Since each edge is counted twice
    }

    // Check if edge exists between two vertices
    bool containsEdge(string vname1, string vname2) {
        if (!containsVertex(vname1) || !containsVertex(vname2)) return false;
        return vtces[vname1].nbrs.find(vname2) != vtces[vname1].nbrs.end();
    }

    // Add an edge between two vertices
    void addEdge(string vname1, string vname2, int value) {
        if (!containsVertex(vname1) || !containsVertex(vname2) || containsEdge(vname1, vname2)) return;
        vtces[vname1].nbrs[vname2] = value;
        vtces[vname2].nbrs[vname1] = value;
    }

    // Remove an edge between two vertices
    void removeEdge(string vname1, string vname2) {
        if (!containsVertex(vname1) || !containsVertex(vname2) || !containsEdge(vname1, vname2)) return;
        vtces[vname1].nbrs.erase(vname2);
        vtces[vname2].nbrs.erase(vname1);
    }

    // Display the metro map
    void displayMap() {
        cout << "\t Indore Metro Map" << endl;
        cout << "\t------------------" << endl;
        for (auto& kv : vtces) {
            string key = kv.first;
            Vertex vtx = kv.second;
            cout << key << " =>" << endl;
            for (auto& nbr : vtx.nbrs) {
                cout << "\t" << nbr.first << "\t";
                if (nbr.first.length() < 16) cout << "\t";
                if (nbr.first.length() < 8) cout << "\t";
                cout << nbr.second << " km" << endl;
            }
        }
        cout << "\t------------------" << endl;
    }

    // Display all stations
    void displayStations() {
        cout << "\n***********************************************************************\n";
        int i = 1;
        for (auto& kv : vtces) {
            cout << i << ". " << kv.first << endl;
            i++;
        }
        cout << "\n***********************************************************************\n";
    }

    // Check if there is a path between two vertices using DFS
    bool hasPath(string vname1, string vname2, unordered_map<string, bool>& processed) {
        if (containsEdge(vname1, vname2)) return true;

        processed[vname1] = true;
        Vertex vtx = vtces[vname1];
        for (auto& nbr : vtx.nbrs) {
            if (!processed[nbr.first] && hasPath(nbr.first, vname2, processed)) {
                return true;
            }
        }
        return false;
    }

    // Structure for Dijkstra's algorithm
    struct DijkstraPair {
        string vname;
        string psf;
        int cost;

        bool operator<(const DijkstraPair& other) const {
            return cost > other.cost; // Min-heap based on cost
        }
    };

    // Dijkstra's algorithm to find shortest path distance
    int dijkstra(string src, string des) {
        priority_queue<DijkstraPair> pq;
        unordered_map<string, DijkstraPair> map;

        // Initialize all vertices with infinite cost, except source
        for (auto& kv : vtces) {
            DijkstraPair np;
            np.vname = kv.first;
            np.cost = INT_MAX;
            if (np.vname == src) {
                np.cost = 0;
                np.psf = src;
            }
            pq.push(np);
            map[kv.first] = np;
        }

        // Process vertices while priority queue is not empty
        while (!pq.empty()) {
            DijkstraPair rp = pq.top();
            pq.pop();

            // If destination is reached, return cost
            if (rp.vname == des) return rp.cost;

            map.erase(rp.vname);

            Vertex v = vtces[rp.vname];
            for (auto& nbr : v.nbrs) {
                if (map.find(nbr.first) != map.end()) {
                    int oc = map[nbr.first].cost;
                    int nc = rp.cost + nbr.second;
                    if (nc < oc) {
                        DijkstraPair& gp = map[nbr.first];
                        gp.psf = rp.psf + nbr.first;
                        gp.cost = nc;
                        pq.push(gp);
                    }
                }
            }
        }
        return INT_MAX; // Destination not reachable
    }

    // Structure for path finding
    struct Pair {
        string vname;
        string psf;
        int min_dis;
    };

    // Utility function to get path with minimum distance
    string getMinPath(string src, string dst) {
        int min_val = INT_MAX;
        string ans;
        unordered_map<string, bool> processed;
        deque<Pair> stack;

        // Start with source vertex
        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        stack.push_front(sp);

        // Process vertices until stack is empty
        while (!stack.empty()) {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed[rp.vname]) continue;
            processed[rp.vname] = true;

            if (rp.vname == dst) {
                if (rp.min_dis < min_val) {
                    ans = rp.psf;
                    min_val = rp.min_dis;
                }
                continue;
            }

            Vertex rpvtx = vtces[rp.vname];
            for (auto& nbr : rpvtx.nbrs) {
                if (!processed[nbr.first]) {
                    Pair np;
                    np.vname = nbr.first;
                    np.psf = rp.psf + nbr.first + "  ";
                    np.min_dis = rp.min_dis + nbr.second;
                    stack.push_front(np);
                }
            }
        }
        ans += to_string(min_val) + " km";
        return ans;
    }

    // Get interchanges between stations in a path
    vector<string> getInterchanges(string path) {
        vector<string> arr;
        size_t pos = 0;
        string token;
        while ((pos = path.find("  ")) != string::npos) {
            token = path.substr(0, pos);
            arr.push_back(token);
            path.erase(0, pos + 2);
        }
        arr.push_back(path); // Last station
        return arr;
    }
};

// Function to create the metro map for Indore
void createMetroMap(Graph_M& g) {
    g.addVertex("Vijay Nagar");
    g.addVertex("Bhawarkua");
    g.addVertex("Regal Square");
    g.addVertex("Rajwada");
    g.addVertex("Indore Junction");
    g.addVertex("Vijay Nagar Square");
    g.addVertex("Palasia");
    g.addVertex("Geeta Bhawan");
    g.addVertex("Navlakha");
    g.addVertex("Mhow Naka");
    g.addVertex("Nanda Nagar");
    g.addVertex("Rau");
    g.addVertex("Bangali Square");

    g.addEdge("Vijay Nagar", "Bhawarkua", 4);
    g.addEdge("Bhawarkua", "Regal Square", 3);
    g.addEdge("Regal Square", "Rajwada", 1);
    g.addEdge("Rajwada", "Indore Junction", 2);
    g.addEdge("Indore Junction", "Vijay Nagar Square", 5);
    g.addEdge("Vijay Nagar Square", "Palasia", 3);
    g.addEdge("Palasia", "Geeta Bhawan", 2);
    g.addEdge("Geeta Bhawan", "Navlakha", 4);
    g.addEdge("Navlakha", "Mhow Naka", 5);
    g.addEdge("Mhow Naka", "Nanda Nagar", 3);
    g.addEdge("Nanda Nagar", "Rau", 6);
    g.addEdge("Rau", "Bangali Square", 4);
}

int main() {
    Graph_M g;
    createMetroMap(g);

    g.displayMap();

    cout << "\nTotal Stations: " << g.numVertices() << endl;
    cout << "Total Edges: " << g.numEdges() << endl;

    string src = "Vijay Nagar";
    string des = "Geeta Bhawan";
    cout << "\nShortest Path from " << src << " to " << des << ": ";
    string path1 = g.getMinPath(src, des);
    cout << path1 << endl;
    cout << "Interchanges: ";
    vector<string> interchanges1 = g.getInterchanges(path1);
    for (string stn : interchanges1) {
        cout << stn << " -> ";
    }
    cout << endl;

    return 0;
}
