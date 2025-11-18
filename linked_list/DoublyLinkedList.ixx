export module DoublyLinkedList;

import DoublyLinkedListNode;

import std;

export template <typename T, typename F>
class DoublyLinkedList {

private:
    DoublyLinkedListNode<T>* head;
    DoublyLinkedListNode<T>* tail;
    F compare;

public:

    explicit DoublyLinkedList(const F& comparator) {
        this->head = nullptr;
        this->tail = nullptr;
        this->compare = comparator;
    }

    DoublyLinkedList& prepend(const T& value) {
        // 创建新节点，next指向当前head，previous为nullptr
        DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(value, this->head, nullptr);
        
        if (this->head) {
            // 如果head存在，更新其previous指针指向新节点
            this->head->previous = newNode;
        }
        
        // 更新head为新节点
        this->head = newNode;

        // 如果链表为空，设置tail也为新节点
        if (this->tail == nullptr) {
            this->tail = newNode;
        }

        return *this;
    }

    DoublyLinkedList& append(const T& value) {
        // 创建新节点，next为nullptr，previous指向当前tail
        DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(value, nullptr, this->tail);

        if (this->head == nullptr) {
            // 空链表情况
            this->head = newNode;
            this->tail = newNode;
            return *this;
        }

        // 更新tail的next指针和tail本身
        this->tail->next = newNode;
        this->tail = newNode;

        return *this;
    }

    DoublyLinkedList& insert(const T& value, const int& rawIndex) {
        const int index = rawIndex < 0 ? 0 : rawIndex;
        
        if (index == 0) {
            this->prepend(value);
        }
        else {
            int count = 1;
            DoublyLinkedListNode<T>* currentNode = this->head;
            
            // 找到插入位置的前一个节点
            while (currentNode && count < index) {
                currentNode = currentNode->next;
                count++;
            }

            if (currentNode) {
                // 在currentNode之后插入
                DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(value, currentNode->next, currentNode);
                
                // 更新后一个节点的previous指针
                if (currentNode->next) {
                    currentNode->next->previous = newNode;
                }
                
                // 更新currentNode的next指针
                currentNode->next = newNode;
                
                // 如果是在尾部插入，更新tail
                if (newNode->next == nullptr) {
                    this->tail = newNode;
                }
            }
            else {
                // 索引超出范围，在尾部追加
                this->append(value);
            }
        }
        return *this;
    }

    std::optional<DoublyLinkedListNode<T>> remove(const T& value) {
        if (this->head == nullptr) {
            return std::nullopt;
        }

        DoublyLinkedListNode<T>* removedNode = nullptr;

        // 处理头部节点匹配的情况
        while (this->head && this->compare.equal(this->head->value, value)) {
            removedNode = this->head;
            this->head = this->head->next;
            
            if (this->head) {
                this->head->previous = nullptr;
            } else {
                // 如果链表变空，更新tail
                this->tail = nullptr;
            }
        }

        DoublyLinkedListNode<T>* currentNode = this->head;

        if (currentNode) {
            // 处理中间节点
            while (currentNode->next) {
                if (this->compare.equal(currentNode->next->value, value)) {
                    removedNode = currentNode->next;
                    
                    // 更新指针连接
                    if (removedNode->next) {
                        removedNode->next->previous = currentNode;
                    } else {
                        // 如果删除的是尾部节点，更新tail
                        this->tail = currentNode;
                    }
                    
                    currentNode->next = removedNode->next;
                }
                else {
                    currentNode = currentNode->next;
                }
            }
        }

        return removedNode ? std::optional<DoublyLinkedListNode<T>>(*removedNode) : std::nullopt;
    }

    std::optional<DoublyLinkedListNode<T>> find(const T& value, std::function<bool(const T&, const T&)> callback) {
        if (this->head == nullptr) {
            return std::nullopt;
        }

        DoublyLinkedListNode<T>* currentNode = this->head;

        while (currentNode) {
            if (callback && callback(currentNode->value, value)) {
                return *currentNode;
            }

            if (this->compare.equal(currentNode->value, value)) {
                return *currentNode;
            }

            currentNode = currentNode->next;
        }

        return std::nullopt;
    }

    std::optional<DoublyLinkedListNode<T>> removeTail() {
        if (this->tail == nullptr) {
            return std::nullopt;
        }
        
        const DoublyLinkedListNode<T>* removedTail = this->tail;

        if (this->head == this->tail) {
            // 只有一个节点的情况
            this->head = nullptr;
            this->tail = nullptr;
        } else {
            // 多个节点的情况，使用previous指针直接找到前一个节点
            this->tail = this->tail->previous;
            if (this->tail) {
                this->tail->next = nullptr;
            }
        }

        return removedTail ? std::optional<DoublyLinkedListNode<T>>(*removedTail) : std::nullopt;
    }

    std::optional<DoublyLinkedListNode<T>> removeHead() {
        if (this->head == nullptr) {
            return std::nullopt;
        }

        const DoublyLinkedListNode<T>* removedHead = this->head;

        if (this->head->next) {
            // 多个节点的情况
            this->head = this->head->next;
            this->head->previous = nullptr;
        } else {
            // 只有一个节点的情况
            this->head = nullptr;
            this->tail = nullptr;
        }

        return removedHead ? std::optional<DoublyLinkedListNode<T>>(*removedHead) : std::nullopt;
    }

    DoublyLinkedList& fromArray(std::vector<T> values) {
        for (T value : values) {
            this->append(value);
        }
        return *this;
    }

    std::vector<DoublyLinkedListNode<T>> toArray() {
        std::vector<DoublyLinkedListNode<T>> nodes = {};

        DoublyLinkedListNode<T>* currentNode = this->head;
        while (currentNode) {
            nodes.push_back(*currentNode);
            currentNode = currentNode->next;
        }

        return nodes;
    }

    std::string toString(std::function<std::string(const T&)> callback) {
        std::vector<DoublyLinkedListNode<T>> nodes = this->toArray();

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

    DoublyLinkedList& reverse() {
        if (this->head == nullptr) {
            return *this;
        }
        
        // 交换头尾指针
        DoublyLinkedListNode<T>* temp = this->head;
        this->head = this->tail;
        this->tail = temp;
        
        DoublyLinkedListNode<T>* current = this->head;
        
        while (current != nullptr) {
            // 交换 next 和 previous 指针
            temp = current->next;
            current->next = current->previous;
            current->previous = temp;
            
            // 移动到下一个节点（注意：此时next指向前一个节点）
            current = current->next;
        }
        
        return *this;
    }

    ~DoublyLinkedList() {
        // 迭代式删除所有节点，避免递归析构导致的栈溢出
        DoublyLinkedListNode<T>* current = this->head;
        while (current != nullptr) {
            DoublyLinkedListNode<T>* next = current->next;
            delete current;
            current = next;
        }
        
        // 将头尾指针置空，避免悬空指针
        this->head = nullptr;
        this->tail = nullptr;
    }
};