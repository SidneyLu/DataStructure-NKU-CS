//
// Created by 31515 on 25-11-16.
//

export module Graph;

import std;

// 前向声明
export template <typename V, typename W> class GraphVertex;
export template <typename V, typename W> class GraphEdge;
export template <typename V, typename W> class Graph;

// GraphVertex类
template <typename V, typename W>
class GraphVertex {
private:
    V value;
    std::vector<GraphEdge<V, W>> edges;
    std::string key;

public:
    explicit GraphVertex(const V& val) : value(val) {
        // 生成唯一键
        key = "vertex_" + std::to_string(reinterpret_cast<std::atomic_uintptr_t>(this));
    }

    const V& getValue() const {
        return value;
    }

    void setValue(const V& val) {
        value = val;
    }

    const std::string& getKey() const {
        return key;
    }

    void addEdge(const GraphEdge<V, W>& edge) {
        edges.push_back(edge);
    }

    bool deleteEdge(const GraphEdge<V, W>& edge) {
        auto it = std::find(edges.begin(), edges.end(), edge);
        if (it != edges.end()) {
            edges.erase(it);
            return true;
        }
        return false;
    }

    bool hasEdge(const GraphEdge<V, W>& edge) const {
        return std::find(edges.begin(), edges.end(), edge) != edges.end();
    }

    const std::vector<GraphEdge<V, W>>& getEdges() const {
        return edges;
    }

    std::vector<GraphVertex<V, W>*> getNeighbors() const {
        std::vector<GraphVertex<V, W>*> neighbors;
        neighbors.reserve(edges.size());
        for (const auto& edge : edges) {
            neighbors.push_back(edge.getEndVertex());
        }
        return neighbors;
    }

    bool hasNeighbor(const GraphVertex<V, W>* vertex) const {
        auto neighbors = getNeighbors();
        return std::any_of(neighbors.begin(), neighbors.end(), 
                          [vertex](const auto* neighbor) {
                              return neighbor == vertex;
                          });
    }

    size_t getDegree() const {
        return edges.size();
    }

    bool operator==(const GraphVertex& other) const {
        return key == other.key;
    }
};

// GraphEdge类
template <typename V, typename W>
class GraphEdge {
private:
    GraphVertex<V, W>* startVertex;
    GraphVertex<V, W>* endVertex;
    W weight;
    std::string key;

public:
    GraphEdge(GraphVertex<V, W>* start, GraphVertex<V, W>* end, const W& w = W())
        : startVertex(start), endVertex(end), weight(w) {
        // 生成边的键
        key = start->getKey() + "_" + end->getKey();
    }

    const std::string& getKey() const {
        return key;
    }

    GraphVertex<V, W>* getStartVertex() const {
        return startVertex;
    }

    GraphVertex<V, W>* getEndVertex() const {
        return endVertex;
    }

    const W& getWeight() const {
        return weight;
    }

    void setWeight(const W& w) {
        weight = w;
    }

    GraphEdge<V, W> reverse() const {
        return GraphEdge<V, W>(endVertex, startVertex, weight);
    }

    bool operator==(const GraphEdge& other) const {
        return key == other.key;
    }

    bool operator!=(const GraphEdge& other) const {
        return !(*this == other);
    }
};

// Graph类
template <typename V, typename W>
class Graph {
private:
    std::unordered_map<std::string, GraphVertex<V, W>> vertices;
    std::unordered_map<std::string, GraphEdge<V, W>> edges;
    bool isDirected;

public:
    explicit Graph(bool directed = false) : isDirected(directed) {}

    GraphVertex<V, W>& addVertex(const V& value) {
        // 检查是否已存在相同值的顶点
        for (auto& pair : vertices) {
            if (pair.second.getValue() == value) {
                return pair.second;
            }
        }
        
        // 创建新顶点并自动生成键
        std::string key = "vertex_" + std::to_string(vertices.size());
        vertices[key] = GraphVertex<V, W>(value);
        return vertices[key];
    }

    bool deleteVertex(GraphVertex<V, W>& vertex) {
        std::string key = vertex.getKey();
        
        if (!vertices.contains(key)) {
            return false;
        }
        
        // 删除与该顶点相关的所有边
        for (const auto& edge : vertex.getEdges()) {
            deleteEdge(edge);
        }
        
        vertices.erase(key);
        return true;
    }

    GraphEdge<V, W>& addEdge(GraphVertex<V, W>& startVertex, GraphVertex<V, W>& endVertex, const W& weight = W()) {
        GraphEdge<V, W> edge(&startVertex, &endVertex, weight);
        std::string key = edge.getKey();
        
        if (edges.contains(key)) {
            edges[key].setWeight(weight);
            return edges[key];
        }
        
        edges[key] = edge;
        startVertex.addEdge(edges[key]);
        
        if (!isDirected) {
            GraphEdge<V, W> reverseEdge(&endVertex, &startVertex, weight);
            std::string reverseKey = reverseEdge.getKey();
            edges[reverseKey] = reverseEdge;
            endVertex.addEdge(edges[reverseKey]);
        }
        
        return edges[key];
    }

    bool deleteEdge(const GraphEdge<V, W>& edge) {
        std::string key = edge.getKey();
        
        if (!edges.contains(key)) {
            return false;
        }
        
        auto& startVertex = *edges[key].getStartVertex();
        startVertex.deleteEdge(edges[key]);
        
        if (!isDirected) {
            GraphEdge<V, W> reverseEdge(*edges[key].getEndVertex(), 
                                      *edges[key].getStartVertex(), 
                                      edge.getWeight());
            std::string reverseKey = reverseEdge.getKey();
            edges[key].getEndVertex()->deleteEdge(edges[reverseKey]);
            edges.erase(reverseKey);
        }
        
        edges.erase(key);
        return true;
    }

    std::vector<GraphVertex<V, W>*> getVertices() {
        std::vector<GraphVertex<V, W>*> result;
        result.reserve(vertices.size());
        for (auto& pair : vertices) {
            result.push_back(&pair.second);
        }
        return result;
    }

    std::vector<GraphEdge<V, W>*> getEdges() {
        std::vector<GraphEdge<V, W>*> result;
        result.reserve(edges.size());
        for (auto& pair : edges) {
            result.push_back(&pair.second);
        }
        return result;
    }

    bool isDirectedGraph() const {
        return isDirected;
    }

    GraphVertex<V, W>* getVertex(const std::string& key) {
        auto it = vertices.find(key);
        if (it != vertices.end()) {
            return &(it->second);
        }
        return nullptr;
    }

    std::vector<GraphVertex<V, W>*> breadthFirstSearch(GraphVertex<V, W>& startVertex) {
        std::vector<GraphVertex<V, W>*> result;
        std::unordered_set<std::string> visited;
        std::queue<GraphVertex<V, W>*> queue;
        
        queue.push(&startVertex);
        visited.insert(startVertex.getKey());
        
        while (!queue.empty()) {
            auto currentVertex = queue.front();
            queue.pop();
            result.push_back(currentVertex);
            
            auto neighbors = currentVertex->getNeighbors();
            for (auto neighbor : neighbors) {
                if (neighbor && !visited.contains(neighbor->getKey())) {
                    queue.push(neighbor);
                    visited.insert(neighbor->getKey());
                }
            }
        }
        
        return result;
    }

    std::vector<GraphVertex<V, W>*> depthFirstSearch(GraphVertex<V, W>& startVertex) {
        std::vector<GraphVertex<V, W>*> result;
        std::unordered_set<std::string> visited;
        std::stack<GraphVertex<V, W>*> stack;
        
        stack.push(&startVertex);
        visited.insert(startVertex.getKey());
        
        while (!stack.empty()) {
            auto currentVertex = stack.top();
            stack.pop();
            result.push_back(currentVertex);
            
            auto neighbors = currentVertex->getNeighbors();
            std::reverse(neighbors.begin(), neighbors.end());
            for (auto neighbor : neighbors) {
                if (neighbor && !visited.contains(neighbor->getKey())) {
                    stack.push(neighbor);
                    visited.insert(neighbor->getKey());
                }
            }
        }
        
        return result;
    }

    size_t getVertexCount() const {
        return vertices.size();
    }

    size_t getEdgeCount() const {
        return edges.size();
    }
    
    // 析构函数
    ~Graph() {
        // vertices和edges会自动调用其析构函数
        edges.clear();
        vertices.clear();
    }
};

// 特化std::hash以支持GraphVertex和GraphEdge作为unordered_map的键
template <typename V, typename W>
class std::hash<GraphVertex<V, W>> {
public:
    std::size_t operator()(const GraphVertex<V, W>& vertex) const {
        return std::hash<std::string>()(vertex.getKey());
    }
};

template <typename V, typename W>
class std::hash<GraphEdge<V, W>> {
public:
    std::size_t operator()(const GraphEdge<V, W>& edge) const {
        return std::hash<std::string>()(edge.getKey());
    }
};