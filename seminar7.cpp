#include <iostream>
#include <queue>

struct IGraph {
    virtual ~IGraph() {}
        
        // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;
    
    virtual int VerticesCount() const  = 0;
    
    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph {
public:
    explicit ListGraph(int size) : vertices_count(size) {
        graph.resize(size);
        prev_graph.resize(size);
    }

    ListGraph(const IGraph& igraph) {

    }

    virtual ~ListGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) {
        graph[from].push_back(to);
        prev_graph[to].push_back(from);
    }

    virtual int VerticesCount() const {
        return vertices_count;
    }

    virtual std::vector<int> GetNextVertices(int vertex) const {
        std::vector<int> result(graph[vertex].size());
        // for (auto v : graph[vertex]) {
        //     result.push_back(v);
        // }
        std::copy(graph[vertex].begin(), graph[vertex].end(), result.begin());
        return result;
    }

    virtual std::vector<int> GetPrevVertices(int vertex) const {
        std::vector<int> result(prev_graph[vertex].size());
        // for (int i = 0; i < vertices_count; ++i) {
        //     for (int v : graph[i]) {
        //         if (v == vertex) {
        //             result.push_back(i);
        //         }
        //     }
        // }
        std::copy(prev_graph[vertex].begin(), prev_graph[vertex].end(), result.begin());
        return result;
    }
private:
    std::vector< std::vector<int> > graph;
    std::vector< std::vector<int> > prev_graph;
    
    int vertices_count;
};


void dfs_aux(const IGraph& graph, void (*callback)(int v), int vertex, std::vector<bool>& visited) {
    visited[vertex] = true;
    callback(vertex);

    for (int v : graph.GetNextVertices(vertex)) {
        if (!visited[v]) {
            dfs_aux(graph, callback, v, visited);            
        }
    }
}

void dfs(const IGraph& graph, void (*callback)(int v)) {
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int v = 0; v < graph.VerticesCount(); ++v) {
        if (!visited[v]) {
            dfs_aux(graph, callback, v, visited);            
        }
    }
}

void bfs_aux(const IGraph& graph, void (*callback)(int v), int vertex, std::vector<bool>& visited) {
    std::queue<int> q;
    q.push(vertex);
    visited[vertex] = true;

    while(!q.empty()) {
        int cur_vertex = q.front();
        q.pop();

        callback(cur_vertex);
        for (int v : graph.GetNextVertices(cur_vertex)) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }   
        }   
    }
}

void bfs(const IGraph& graph, void (*callback)(int v)) {
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int v = 0; v < graph.VerticesCount(); ++v) {
        if (!visited[v]) {
            bfs_aux(graph, callback, v, visited);            
        }
    }
}
   
int main() {
    ListGraph list_graph(6);
    list_graph.AddEdge(0, 1);
    list_graph.AddEdge(1, 2);
    list_graph.AddEdge(2, 3);
    list_graph.AddEdge(3, 1);
    list_graph.AddEdge(3, 4);

    dfs(list_graph, [](int v) {
        std::cout << "dfs=" << v << std::endl;
    });
    
    bfs(list_graph, [](int v) {
        std::cout << "bfs=" << v << std::endl;
    });
    return 0;
}
