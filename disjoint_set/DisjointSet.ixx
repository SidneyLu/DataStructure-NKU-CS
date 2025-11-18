//
// Created by 31515 on 25-11-18.
//

export module DisjointSet;

import std;


export class DisjointSet
{
private:
    std::vector<int> roots;
    std::vector<int> heights;
public:
    explicit DisjointSet(int size) : roots(size), heights(size, 1) {
        for (int i = 0; i < size; i++) {
            roots[i] = i;
        }
    }

    int find(int x) {
        if (roots[x] != x) {
            roots[x] = find(roots[x]);
        }
        return roots[x];
    }

    void join(int a, int b) {
        int rootA = this->find(a);
        int rootB = this->find(b);
        if (rootA == rootB) {
            return ;
        }
        if (heights[rootA] > heights[rootB]) {
            roots[rootB] = rootA;
        } else if (heights[rootA] < heights[rootB]) {
            roots[rootA] = rootB;
        } else {
            roots[rootB] = rootA;
            heights[rootA]++;
        }
    }

    bool connected(int a, int b) {
        return this->find(a) == this->find(b);
    }
};
