//
// Created by 31515 on 25-11-16.
//

export module PriorityQueue;

import Heap;

import std;

// MinHeap 类实现
export template <typename T, typename F>
class MinHeap : public Heap<T, F> {
public:
    explicit MinHeap(const F& comparator) : Heap<T, F>(comparator) {}

    bool inOrder(const T& a, const T& b) const override {
        return this->comparator->lessEqualThan(a, b);
    }
};

// MaxHeap 类实现
export template <typename T, typename F>
class MaxHeap : public Heap<T, F> {
public:
    explicit MaxHeap(const F& comparator) : Heap<T, F>(comparator) {}

    bool inOrder(const T& a, const T& b) const override {
        return this->comparator->greaterEqualThan(a, b);
    }
};

// 优先级比较器类，基于元素的优先级进行比较
export template <typename T>
class PriorityComparator {
private:
    // 存储元素到优先级的映射
    std::unordered_map<T, int>* priorities;

public:
    // 构造函数，接收优先级映射的指针
explicit PriorityComparator(std::unordered_map<T, int>* prioritiesPtr) : priorities(prioritiesPtr) {}

    // 比较两个元素的优先级，a < b
    bool lessThan(const T& a, const T& b) const {
        // 优先级小的认为是"小于"，这将创建最小堆，优先级低的元素在顶部
        return (*priorities)[a] < (*priorities)[b];
    }

    // 比较两个元素的优先级，a <= b
    bool lessEqualThan(const T& a, const T& b) const {
        return (*priorities)[a] <= (*priorities)[b];
    }

    // 比较两个元素的优先级，a > b
    bool greaterThan(const T& a, const T& b) const {
        return (*priorities)[a] > (*priorities)[b];
    }

    // 比较两个元素的优先级，a >= b
    bool greaterEqualThan(const T& a, const T& b) const {
        return (*priorities)[a] >= (*priorities)[b];
    }

    // 检查两个元素是否相等
    bool equal(const T& a, const T& b) const {
        // 首先检查是否有相同的优先级
        if ((*priorities)[a] == (*priorities)[b]) {
            // 如果优先级相同，则检查元素是否相同
            return a == b;
        }
        return false;
    }
};

// 最大优先级比较器，用于创建最大堆
export template <typename T>
class MaxPriorityComparator {
private:
    std::unordered_map<T, int>* priorities;

public:
    explicit MaxPriorityComparator(std::unordered_map<T, int>* prioritiesPtr) : priorities(prioritiesPtr) {}

    // 比较两个元素的优先级，a < b
    bool lessThan(const T& a, const T& b) const {
        // 优先级大的认为是"小于"，这将创建最大堆，优先级高的元素在顶部
        return (*priorities)[a] > (*priorities)[b];
    }

    // 比较两个元素的优先级，a <= b
    bool lessEqualThan(const T& a, const T& b) const {
        return (*priorities)[a] >= (*priorities)[b];
    }

    // 比较两个元素的优先级，a > b
    bool greaterThan(const T& a, const T& b) const {
        return (*priorities)[a] < (*priorities)[b];
    }

    // 比较两个元素的优先级，a >= b
    bool greaterEqualThan(const T& a, const T& b) const {
        return (*priorities)[a] <= (*priorities)[b];
    }

    // 检查两个元素是否相等
    bool equal(const T& a, const T& b) const {
        return (*priorities)[a] == (*priorities)[b] && a == b;
    }
};

export template <typename T>
class PriorityQueue : public MinHeap<T, PriorityComparator<T>> {
private:
    // 存储每个元素的优先级
    std::unordered_map<T, int> priorities;
    // 使用优先级比较器
    PriorityComparator<T> priorityComparator;

public:
    // 构造函数，初始化堆和比较器
    PriorityQueue() : MinHeap<T, PriorityComparator<T>>(priorityComparator), 
                      priorityComparator(&priorities) {}

    // 添加带优先级的元素
    PriorityQueue& add(const T& item, int priority) {
        // 存储优先级
        this->priorities[item] = priority;
        // 使用基类的 add 方法
        this->MinHeap<T, PriorityComparator<T>>::add(item);
        return *this;
    }

    // 移除元素并清理其优先级
    PriorityQueue& remove(const T& item) {
        // 清理优先级映射
        this->priorities.erase(item);
        // 使用基类的 remove 方法
        this->MinHeap<T, PriorityComparator<T>>::remove(item);
        return *this;
    }

    // 改变元素的优先级
    PriorityQueue& change(const T& item, int priority) {
        // 移除旧元素
        this->remove(item);
        // 添加带有新优先级的元素
        this->add(item, priority);
        return *this;
    }

    // 检查队列是否包含特定元素
    bool hasValue(const T& item) const {
        return this->priorities.find(item) != this->priorities.end();
    }

    // 获取元素的优先级
    std::optional<int> getPriority(const T& item) const {
        auto it = this->priorities.find(item);
        if (it != this->priorities.end()) {
            return it->second;
        }
        return std::nullopt;
    }
};

// 最大优先级队列特化
export template <typename T>
class MaxPriorityQueue : public MaxHeap<T, MaxPriorityComparator<T>> {
private:
    std::unordered_map<T, int> priorities;
    MaxPriorityComparator<T> priorityComparator;

public:
    MaxPriorityQueue() : MaxHeap<T, MaxPriorityComparator<T>>(priorityComparator), 
                         priorityComparator(&priorities) {}

    MaxPriorityQueue& add(const T& item, int priority) {
        this->priorities[item] = priority;
        this->MaxHeap<T, MaxPriorityComparator<T>>::add(item);
        return *this;
    }

    MaxPriorityQueue& remove(const T& item) {
        this->priorities.erase(item);
        this->MaxHeap<T, MaxPriorityComparator<T>>::remove(item);
        return *this;
    }

    bool hasValue(const T& item) const {
        return this->priorities.find(item) != this->priorities.end();
    }
};