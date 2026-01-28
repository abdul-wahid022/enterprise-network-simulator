#ifndef NETWORK_GRAPH_H
#define NETWORK_GRAPH_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

// ============================================================================
// GRAPH FOR NETWORK TOPOLOGY
// ============================================================================

struct Edge {
    string target;
    int weight;
    
    Edge(string t, int w) : target(t), weight(w) {}
};

class NetworkGraph {
private:
    map<string, vector<Edge> > adjList;
    
public:
    void addEdge(string from, string to, int weight = 1) {
        adjList[from].push_back(Edge(to, weight));
        adjList[to].push_back(Edge(from, weight));
    }
    
    // BFS - Find shortest path
    vector<string> bfsPath(string start, string end) {
        map<string, bool> visited;
        map<string, string> parent;
        queue<string> q;
        
        q.push(start);
        visited[start] = true;
        parent[start] = "";
        
        while (!q.empty()) {
            string current = q.front();
            q.pop();
            
            if (current == end) {
                // Reconstruct path
                vector<string> path;
                string node = end;
                while (node != "") {
                    path.push_back(node);
                    node = parent[node];
                }
                reverse(path.begin(), path.end());
                return path;
            }
            
            for (size_t i = 0; i < adjList[current].size(); i++) {
                string neighbor = adjList[current][i].target;
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    parent[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }
        
        return vector<string>(); // No path found
    }
    
    // DFS - Network exploration
    void dfs(string start, set<string>& visited) {
        visited.insert(start);
        cout << start << " -> ";
        
        for (size_t i = 0; i < adjList[start].size(); i++) {
            string neighbor = adjList[start][i].target;
            if (visited.find(neighbor) == visited.end()) {
                dfs(neighbor, visited);
            }
        }
    }
    
    // Dijkstra - Find shortest weighted path
    int dijkstra(string start, string end) {
        map<string, int> dist;
        map<string, bool> visited;
        priority_queue<pair<int, string>, vector<pair<int, string> >, greater<pair<int, string> > > pq;
        
        // Initialize distances
        for (map<string, vector<Edge> >::iterator it = adjList.begin(); it != adjList.end(); ++it) {
            dist[it->first] = 999999;
        }
        dist[start] = 0;
        pq.push(make_pair(0, start));
        
        while (!pq.empty()) {
            string current = pq.top().second;
            pq.pop();
            
            if (visited[current]) continue;
            visited[current] = true;
            
            if (current == end) {
                return dist[end];
            }
            
            for (size_t i = 0; i < adjList[current].size(); i++) {
                string neighbor = adjList[current][i].target;
                int weight = adjList[current][i].weight;
                
                if (dist[current] + weight < dist[neighbor]) {
                    dist[neighbor] = dist[current] + weight;
                    pq.push(make_pair(dist[neighbor], neighbor));
                }
            }
        }
        
        return -1; // No path
    }
    
    void displayConnections() {
        cout << "\n=== NETWORK CONNECTIONS ===\n";
        for (map<string, vector<Edge> >::iterator it = adjList.begin(); it != adjList.end(); ++it) {
            cout << it->first << " -> ";
            for (size_t i = 0; i < it->second.size(); i++) {
                cout << it->second[i].target << "(" << it->second[i].weight << "ms) ";
            }
            cout << "\n";
        }
    }
};

#endif // NETWORK_GRAPH_H