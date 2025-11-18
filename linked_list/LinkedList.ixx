export module LinkedList;

import LinkedListNode;

import std;

export template <typename T, typename F>
class LinkedList {

private:
    LinkedListNode<T>* head;
    LinkedListNode<T>* tail;
    F compare;

public:

    explicit LinkedList(const F& comparator) {
        this->head = nullptr;
        this->tail = nullptr;
        this->compare = comparator;
    }

    LinkedList& prepend(const T& value) {
        const LinkedListNode<T>* newNode = new LinkedListNode<T>(value, nullptr);
        this->head = newNode;

        if (this->tail == nullptr) {
            this->tail = newNode;
        }

        return *this;
    }

    LinkedList& append(const T& value) {
        const LinkedListNode<T>* newNode = new LinkedListNode<T>(value, nullptr);

        if (this->head == nullptr) {
            this->head = newNode;
            this->tail = newNode;
            return *this;
        }

        this->tail->next = newNode;
        this->tail = newNode;

        return *this;
    }

    LinkedList& insert(const T& value, const int& rawIndex) {
        const int index = rawIndex < 0 ? 0 : rawIndex;
        if (index == 0) {
            this->prepend(value);
        }
        else {
            int count = 1;
            LinkedListNode<T>* currentNode = this->head;
            auto newNode = new LinkedListNode<T>(value, nullptr);

            while (currentNode) {
                if (count == index) break;
                currentNode = currentNode->next;
                count++;
            }

            if (currentNode) {
                newNode->next = currentNode->next;
                currentNode->next = newNode;
            }
            else {
                if (this->tail) {
                    this->tail->next = newNode;
                    this->tail = newNode;
                }
                else {
                    this->head = newNode;
                    this->tail = newNode;
                }
            }
        }
        return *this;
    }

    std::optional<LinkedListNode<T>> remove(const T& value) {
        if (this->head == nullptr) {
            return std::nullopt;
        }

        LinkedListNode<T>* removedNode = nullptr;

        while (this->head && this->compare.equal(this->head->value, value)) {
            removedNode = this->head;
            this->head = this->head->next;
        }

        LinkedListNode<T>* currentNode = this->head;

        if (currentNode) {
            while (currentNode->next) {
                if (this->compare.equal(currentNode->next->value, value)) {
                    removedNode = currentNode->next;
                    currentNode->next = currentNode->next->next;
                }
                else {
                    currentNode = currentNode->next;
                }
            }
        }

        if (this->compare.equal(this->tail->value, value)) {
            this->tail = currentNode;
        }

        return removedNode ? *removedNode : std::nullopt;
    }

    std::optional<LinkedListNode<T>> find(const T& value, std::function<bool(const T&)> callback) {

        if (this->head == nullptr) {
            return std::nullopt;
        }

        LinkedListNode<T>* currentNode = this->head;

        while (currentNode) {

            if (callback && callback(currentNode->value, value)) {
                return currentNode;
            }

            if (this->compare.equal(currentNode->value, value)) {
                return currentNode;
            }

            currentNode = currentNode->next;
        }

        return std::nullopt;
    }

    std::optional<LinkedListNode<T>> removeTail() {
        const LinkedListNode<T>* removedTail = this->tail;

        if (this->head == this->tail) {
            this->head = nullptr;
            this->tail = nullptr;

            return *removedTail;
        }

        LinkedListNode<T>* currentNode = this->tail;
        while (currentNode->next) {
            if (!currentNode->next->next) {
                currentNode->next = nullptr;
            }
            else {
                currentNode = currentNode->next;
            }
        }

        this->tail = currentNode;

        return removedTail ? *removedTail : std::nullopt;
    }

    std::optional<LinkedListNode<T>> removeHead() {
        if (this->head == nullptr) {
            return std::nullopt;
        }

        const LinkedListNode<T>* removedHead = this->head;

        if (this->head->next) {
            this->head = this->head->next;
        }
        else {
            this->head = nullptr;
            this->tail = nullptr;
        }

        return removedHead ? *removedHead : std::nullopt;
    }

    LinkedList& fromArray(std::vector<T> values) {

        for (T value : values) {
            this->append(value);
        }

        return *this;
    }

    std::vector<LinkedListNode<T>> toArray() {
        std::vector<LinkedListNode<T>> nodes = {};

        LinkedListNode<T>* currentNode = this->head;
        while (currentNode) {
            nodes.push_back(*currentNode);
            currentNode = currentNode->next;
        }

        return nodes;
    }

    std::string toString(std::function<std::string(const T&)> callback) {
        std::vector<LinkedListNode<T>> nodes = this->toArray();

        if (nodes.empty()) {
            return "";
        }

        std::vector<std::string> nodeStrings;
        for (const auto& node : nodes) {
            nodeStrings.push_back(node.toString(callback));
        }

        std::string result;
        for (size_t i = 0; i < nodeStrings.size(); ++i) {
            if (i != 0) {
                result += ",";
            }
            result += nodeStrings[i];
        }

        return result;
    }

    LinkedList& reverse() {
        LinkedListNode<T> currNode = this->head;
        LinkedListNode<T>* prevNode = nullptr;
        LinkedListNode<T>* nextNode = nullptr;

        while (currNode) {
            nextNode = currNode->next;
            currNode->next = prevNode;

            prevNode = currNode;
            currNode = nextNode;
        }

        this->tail = this->head;
        this->head = prevNode;

        return *this;
    }

    ~LinkedList() {
        // 迭代式删除所有节点，避免递归析构导致的栈溢出
        LinkedListNode<T>* current = this->head;
        while (current != nullptr) {
            LinkedListNode<T>* next = current->next;
            delete current;
            current = next;
        }
        
        // 将头尾指针置空，避免悬空指针
        this->head = nullptr;
        this->tail = nullptr;
    }
};