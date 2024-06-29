#include <bits/stdc++.h>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <limits>

using namespace std;

map<string, string> users; // Regular user credentials
map<string, string> adminCredentials; // Admin credentials

map<string, map<string, string>> helplines;

void loadUsers() {
    ifstream file("users.txt");
    if (file.is_open()) {
        string username, password;
        while (file >> username >> password) {
            users[username] = password;
        }
        file.close();
    } else {
        cerr << "Error: Unable to open users.txt\n";
    }
}

void saveUsers() {
    ofstream file("users.txt");
    if (file.is_open()) {
        for (const auto &user : users) {
            file << user.first << " " << user.second << endl;
        }
        file.close();
    } else {
        cerr << "Error: Unable to open users.txt\n";
    }
}

void loadAdminCredentials() {
    ifstream file("admin.txt");
    if (file.is_open()) {
        string username, password;
        while (file >> username >> password) {
            adminCredentials[username] = password;
        }
        file.close();
    } else {
        cerr << "Error: Unable to open admin.txt\n";
    }
}

void loadHelplines() {
    ifstream file("helplines.txt");
    if (file.is_open()) {
        string station, service, number;
        while (file >> station >> service >> number) {
            helplines[station][service] = number;
        }
        file.close();
    } else {
        cerr << "Error: Unable to open helplines.txt\n";
    }
}

void saveHelplines() {
    ofstream file("helplines.txt");
    if (file.is_open()) {
        for (const auto &station : helplines) {
            for (const auto &service : station.second) {
                file << station.first << " " << service.first << " " << service.second << endl;
            }
        }
        file.close();
    } else {
        cerr << "Error: Unable to open helplines.txt\n";
    }
}

void addEdge(string s1, string s2, int dist, unordered_map<string, vector<pair<string, int>>>& adj) {
    adj[s1].push_back({s2, dist});
    adj[s2].push_back({s1, dist});
}

void loadEdges(unordered_map<string, vector<pair<string, int>>>& adj) {
    ifstream file("edges.txt");
    if (file.is_open()) {
        string station1, station2;
        int distance;
        while (file >> station1 >> station2 >> distance) {
            addEdge(station1, station2, distance, adj);
        }
        file.close();
    } else {
        cerr << "Error: Unable to open edges.txt\n";
    }
}

void saveEdges(const unordered_map<string, vector<pair<string, int>>>& adj) {
    ofstream file("edges.txt");
    if (file.is_open()) {
        set<pair<string, pair<string, int>>> edgeSet; // To avoid duplicate edges
        for (const auto &node : adj) {
            for (const auto &neighbor : node.second) {
                if (node.first < neighbor.first) { // Avoid duplicate entries
                    edgeSet.insert({node.first, {neighbor.first, neighbor.second}});
                }
            }
        }
        for (const auto &edge : edgeSet) {
            file << edge.first << " " << edge.second.first << " " << edge.second.second << endl;
        }
        file.close();
    } else {
        cerr << "Error: Unable to open edges.txt\n";
    }
}

bool login(const string &username, const string &password) {
    auto it = users.find(username);
    return it != users.end() && it->second == password;
}

bool adminLogin(const string &username, const string &password) {
    auto it = adminCredentials.find(username);
    return it != adminCredentials.end() && it->second == password;
}

void registerUser(const string &username, const string &password) {
    users[username] = password;
    saveUsers();
}

void unregisterUser(const string &username) {
    users.erase(username);
    saveUsers();
}

vector<string> shortestPath(const unordered_map<string, vector<pair<string, int>>>& adj, const string &src, const string &dst) {
    unordered_map<string, int> dist;
    unordered_map<string, string> prev;
    set<pair<int, string>> pq;

    for (const auto &node : adj) {
        dist[node.first] = numeric_limits<int>::max();
    }
    dist[src] = 0;
    pq.insert({0, src});

    while (!pq.empty()) {
        string u = pq.begin()->second;
        pq.erase(pq.begin());

        if (u == dst) break;

        for (const auto &neighbor : adj.at(u)) {
            string v = neighbor.first;
            int weight = neighbor.second;
            int alt = dist[u] + weight;

            if (alt < dist[v]) {
                pq.erase({dist[v], v});
                dist[v] = alt;
                prev[v] = u;
                pq.insert({alt, v});
            }
        }
    }

    vector<string> path;
    for (string at = dst; at != ""; at = prev[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());

    if (path.size() == 1 && path[0] != src) {
        return {}; // No path found
    }
    return path;
}

void displayHelplines() {
    string station;
    cout << "Enter station name to get helpline numbers: ";
    cin.ignore();
    getline(cin, station);

    if (helplines.find(station) != helplines.end()) {
        cout << "Helpline numbers for " << station << ":\n";
        for (const auto& [service, number] : helplines[station]) {
            cout << service << ": " << number << endl;
        }
    } else {
        cout << "Station not found.\n";
    }
}

void removeEdge(string s1, string s2, unordered_map<string, vector<pair<string, int>>>& adj) {
    auto removeFromAdjList = [](vector<pair<string, int>>& list, const string& station) {
        list.erase(remove_if(list.begin(), list.end(), [&](const pair<string, int>& p) {
            return p.first == station;
        }), list.end());
    };

    removeFromAdjList(adj[s1], s2);
    removeFromAdjList(adj[s2], s1);
}

void updateEdge(string s1, string s2, int newDist, unordered_map<string, vector<pair<string, int>>>& adj) {
    removeEdge(s1, s2, adj);
    addEdge(s1, s2, newDist, adj);
}

void addNewStationHelplines(const string &station) {
    if (helplines.find(station) == helplines.end()) {
        string service, number;
        cout << "The station " << station << " is new. Please enter helpline numbers for the following services:\n";
        vector<string> services = {"Police", "Fire", "Ambulance", "Cleaners"};

        for (const auto& svc : services) {
            cout << svc << ": ";
            cin.ignore();
            getline(cin, number);
            helplines[station][svc] = number;
        }
        saveHelplines();
        cout << "Helplines for " << station << " have been added.\n";
    }
}

bool userOperations(const string &username, const unordered_map<string, vector<pair<string, int>>>& adj) {
    while (true) {
        int operation;
        cout << "Welcome to Indore Metro, " << username << "!\n";
        cout << "1. Find shortest path\n2. Important numbers\n3. Unregister user\n4. Logout\nEnter your choice: ";
        cin >> operation;

        if (operation == 1) {
            string src, dst;
            cout << "Enter source station: ";
            cin.ignore();
            getline(cin, src);
            cout << "Enter destination station: ";
            getline(cin, dst);

            vector<string> path = shortestPath(adj, src, dst);
            if (!path.empty()) {
                cout << "Shortest path from " << src << " to " << dst << ":\n";
                for (const auto &station : path) {
                    cout << station << " ";
                }
                cout << endl;
                int totalDistance = 0;
                for (size_t i = 0; i < path.size() - 1; ++i) {
                    for (const auto &neighbor : adj.at(path[i])) {
                        if (neighbor.first == path[i + 1]) {
                            totalDistance += neighbor.second;
                            break;
                        }
                    }
                }
                cout << "Total distance: " << totalDistance << " km\n";
                cout << "Charge: " << totalDistance * 5 << " INR\n";
            } else {
                cout << "No path found.\n";
            }
        } else if (operation == 2) {
            displayHelplines();
        } else if (operation == 3) {
            cout << "Are you sure you want to unregister? (yes/no): ";
            string confirmation;
            cin >> confirmation;
            if (confirmation == "yes") {
                users.erase(username);
                saveUsers();
                cout << "You have been unregistered.\n";
                return false; // Exit the user operations
            }
        } else if (operation == 4) {
            cout << "Logging out...\n";
            return false; // Logout and exit user operations
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

bool adminOperations(unordered_map<string, vector<pair<string, int>>>& adj) {
    while (true) {
        int operation;
        cout << "Admin Panel\n";
        cout << "1. Update helpline numbers\n2. Add an edge\n3. Remove an edge\n4. Update an edge\n5. Logout\nEnter your choice: ";
        cin >> operation;

        if (operation == 1) {
            string station, service, number;
            cout << "Enter station name: ";
            cin.ignore();
            getline(cin, station);
            cout << "Enter service name: ";
            getline(cin, service);
            cout << "Enter new helpline number: ";
            getline(cin, number);

            helplines[station][service] = number;
            saveHelplines();
            cout << "Helpline number updated.\n";
        } else if (operation == 2) {
            string s1, s2;
            int distance;
            cout << "Enter first station: ";
            cin.ignore();
            getline(cin, s1);
            cout << "Enter second station: ";
            getline(cin, s2);
            cout << "Enter distance: ";
            cin >> distance;

            addEdge(s1, s2, distance, adj);

            // Check and add helplines for new stations
            addNewStationHelplines(s1);
            addNewStationHelplines(s2);

            saveEdges(adj);
            cout << "Edge added.\n";
        } else if (operation == 3) {
            string s1, s2;
            cout << "Enter first station: ";
            cin.ignore();
            getline(cin, s1);
            cout << "Enter second station: ";
            getline(cin, s2);

            removeEdge(s1, s2, adj);
            saveEdges(adj);
            cout << "Edge removed.\n";
        } else if (operation == 4) {
            string s1, s2;
            int newDist;
            cout << "Enter first station: ";
            cin.ignore();
            getline(cin, s1);
            cout << "Enter second station: ";
            getline(cin, s2);
            cout << "Enter new distance: ";
            cin >> newDist;

            updateEdge(s1, s2, newDist, adj);
            saveEdges(adj);
            cout << "Edge updated.\n";
        } else if (operation == 5) {
            cout << "Logging out...\n";
            return false; // Logout and exit the program
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

int main() {
    unordered_map<string, vector<pair<string, int>>> adj;
    loadUsers();
    loadAdminCredentials();
    loadHelplines();
    loadEdges(adj);

    while (true) {
        int choice;
        cout << "1. User Login\n2. Admin Login\n3. Register User\n4. Exit\nEnter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string username, password;
            cout << "Enter username: ";
            cin.ignore();
            getline(cin, username);
            cout << "Enter password: ";
            getline(cin, password);

            if (login(username, password)) {
                cout << "Login successful.\n";
                if (!userOperations(username, adj)) {
                    continue; // Return to main menu after logout
                }
            } else {
                cout << "Invalid username or password.\n";
            }
        } else if (choice == 2) {
            string username, password;
            cout << "Enter admin username: ";
            cin.ignore();
            getline(cin, username);
            cout << "Enter admin password: ";
            getline(cin, password);

            if (adminLogin(username, password)) {
                cout << "Admin login successful.\n";
                if (!adminOperations(adj)) {
                    continue; // Return to main menu after logout
                }
            } else {
                cout << "Invalid admin credentials.\n";
            }
        } else if (choice == 3) {
            string username, password;
            cout << "Enter new username: ";
            cin.ignore();
            getline(cin, username);
            cout << "Enter new password: ";
            getline(cin, password);

            if (users.find(username) == users.end()) {
                registerUser(username, password);
                cout << "Registration successful.\n";
            } else {
                cout << "Username already exists.\n";
            }
        } else if (choice == 4) {
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}
