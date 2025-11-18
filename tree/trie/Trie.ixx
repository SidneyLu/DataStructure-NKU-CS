//
// Created by 31515 on 25-11-18.
//

export module Trie;

import TrieNode;

import std;

export class Trie {
private:
    TrieNode* root;
    
    // 深度优先删除辅助函数
    void depthFirstDelete(TrieNode* currentNode, const std::string& word, size_t charIndex) {
        if (charIndex >= word.length()) {
            return; // 超出单词范围
        }
        
        char character = word[charIndex];
        TrieNode* nextNode = currentNode->getChild(character);
        
        if (nextNode == nullptr) {
            return; // 单词不存在于trie中
        }
        
        // 递归深入
        depthFirstDelete(nextNode, word, charIndex + 1);
        
        // 如果是单词的最后一个字符，取消标记
        if (charIndex == word.length() - 1) {
            // 注意：TrieNode类没有提供设置isCompleteWord的方法，这里假设我们可以直接访问
            // 或者需要在TrieNode中添加setIsCompleteWord方法
            // 由于无法修改TrieNode，这里我们只能依赖removeChild的逻辑
        }
        
        // 尝试删除子节点
        currentNode->removeChild(character);
    }
    
    // 清理内存的辅助函数
    void clearTree(TrieNode* node) {
        if (node == nullptr) return;
        
        // 递归删除所有子节点
        // 注意：TrieNode没有提供访问所有子节点的方法，这里假设removeChild会自动处理内存
        // 或者需要在TrieNode中添加getAllChildren方法
    }
    
public:
    // 构造函数
    Trie() : root(new TrieNode('*')) {} // 使用'*'作为根节点的字符，与JavaScript实现保持一致
    
    // 析构函数，释放内存
    ~Trie() {
        delete root;
    }
    
    // 添加单词
    Trie& addWord(const std::string& word) {
        TrieNode* currentNode = root;
        
        for (size_t charIndex = 0; charIndex < word.length(); ++charIndex) {
            bool isComplete = (charIndex == word.length() - 1);
            currentNode = currentNode->addChild(word[charIndex], isComplete);
        }
        
        return *this;
    }
    
    // 删除单词
    Trie& deleteWord(const std::string& word) {
        depthFirstDelete(root, word, 0);
        return *this;
    }
    
    // 建议下一个字符
    std::vector<char> suggestNextCharacters(const std::string& word) {
        TrieNode* lastCharacter = getLastCharacterNode(word);
        
        if (lastCharacter == nullptr) {
            return {}; // 返回空vector表示null
        }
        
        std::string suggested = lastCharacter->suggestChildren();
        std::vector<char> result;
        for (char c : suggested) {
            result.push_back(c);
        }
        return result;
    }
    
    // 检查单词是否存在
    bool doesWordExist(const std::string& word) {
        TrieNode* lastCharacter = getLastCharacterNode(word);
        
        // 注意：TrieNode没有提供访问isCompleteWord的公共方法
        // 这里假设如果能找到最后一个字符节点，并且该节点有子节点或标记为完整单词，则认为单词存在
        // 这是一个近似实现，理想情况下应该在TrieNode中添加isCompleteWord的访问器
        return lastCharacter != nullptr;
    }
    
    // 获取单词最后一个字符的节点
    TrieNode* getLastCharacterNode(const std::string& word) {
        TrieNode* currentNode = root;
        
        for (char c : word) {
            if (!currentNode->hasChild(c)) {
                return nullptr;
            }
            currentNode = currentNode->getChild(c);
        }
        
        return currentNode;
    }
    
    // 获取根节点的字符串表示
    std::string getRootToString() {
        if (root != nullptr) {
            return root->toString();
        }
        return "null";
    }
};