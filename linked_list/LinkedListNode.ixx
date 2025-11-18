//
// Created by 31515 on 25-11-15.
//
export module LinkedListNode;

import std;

export template <typename T>
class LinkedListNode {
public:

    T value;
    LinkedListNode<T>* next;

    LinkedListNode(T value, LinkedListNode<T>* next) {
        this->value = value;
        this->next = next;
    }

    std::string toString(std::function<std::string(const T&)> callback) {
        return callback ? callback(this->value) : std::to_string(this->value);
    }

    ~LinkedListNode() {
        this->next = nullptr;
    }
};