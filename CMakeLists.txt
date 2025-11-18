cmake_minimum_required(VERSION 3.30)
project(DataStructure_NKU_CS)

set(CMAKE_CXX_STANDARD 20)

add_executable(DataStructure_NKU_CS
        tree/BinaryTreeNode.ixx
        tree/avl_tree/AvlTree.ixx
        hash_table/HashTable.ixx
        linked_list/LinkedListNode.ixx
        linked_list/LinkedList.ixx
        linked_list/DoublyLinkedListNode.ixx
        linked_list/DoublyLinkedList.ixx
        tree/fenwick_tree/FenwickTree.ixx
        tree/binary_search_tree/BinarySearchTreeNode.ixx
        tree/binary_search_tree/BinarySearchTree.ixx
        queue/Queue.ixx
        stack/Stack.ixx
        graph/Graph.ixx
        tree/heap/Heap.ixx
        queue/PriorityQueue.ixx
        tree/trie/TrieNode.ixx
        tree/trie/Trie.ixx
        disjoint_set/DisjointSet.ixx
        tree/segment_tree/SegmentTree.ixx
        tree/red_black_tree/RedBlackTree.ixx)
