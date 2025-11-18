// HashTable.ixx
export module HashTable;

import LinkedList;
import LinkedListNode;
import std;


constexpr int defaultHashTableSize = 32;


template <typename T>
class DefaultHashComparator {
    bool equal(const std::pair<std::string, T>& a, const std::pair<std::string, T>& b) {
        return a.first == b.first;  // 按key比较相等性
    }
};


export template <typename T, typename F = DefaultHashComparator<T>>
class HashTable {
private:
    std::vector<LinkedList<std::pair<std::string, T>, F>> buckets;
    std::unordered_map<std::string, int> keys;
    F comparator;

public:
    explicit HashTable(int hashTableSize = defaultHashTableSize, const F& comparator = F{})
        : buckets(hashTableSize, comparator),  // 每个桶的链表都传入比较器
          keys(),
          comparator(comparator) {}

    [[nodiscard]] int hash(const std::string& key) const {
        int hashValue = std::accumulate(
            key.begin(), key.end(), 0,
            [](int acc, char c) { return acc + static_cast<int>(c); }
        );
        return hashValue % buckets.size();
    }


    void set(const std::string& key, const T& value) {
        int keyHash = hash(key);
        keys[key] = keyHash;
        auto& bucketList = buckets[keyHash];

        // 目标键值对（用于查找）
        std::pair<std::string, T> targetKeyPair = {key, value};
        // 用callback查找：匹配节点value的key
        auto findCallback = [](const std::pair<std::string, T>& nodePair, const std::pair<std::string, T>& targetPair) {
            return nodePair.first == targetPair.first;  // 比较key
        };


        auto existingNode = bucketList.find(targetKeyPair, findCallback);
        if (existingNode.has_value()) {
            existingNode.value()->value.second = value;
        } else {
            bucketList.append(targetKeyPair);
        }
    }

    std::optional<T> erase(const std::string& key) {
        if (!has(key)) return std::nullopt;
        int keyHash = keys[key];
        keys.erase(key);
        auto& bucketList = buckets[keyHash];


        std::pair<std::string, T> targetKeyPair = {key, T{}};
        auto findCallback = [](const std::pair<std::string, T>& nodePair, const std::pair<std::string, T>& targetPair) {
            return nodePair.first == targetPair.first;
        };


        auto targetNode = bucketList.find(targetKeyPair, findCallback);
        if (targetNode.has_value()) {
            T deletedValue = targetNode.value()->value.second;
            bucketList.remove(targetNode.value()->value);
            return deletedValue;
        }

        return std::nullopt;
    }

    [[nodiscard]] std::optional<T> get(const std::string& key) {
        if (!has(key)) return std::nullopt;

        int keyHash = keys[key];
        const auto& bucketList = buckets[keyHash];

        std::pair<std::string, T> targetKeyPair = {key, T{}};
        auto findCallback = [](const std::pair<std::string, T>& nodePair, const std::pair<std::string, T>& targetPair) {
            return nodePair.first == targetPair.first;
        };

        auto foundNode = bucketList.find(targetKeyPair, findCallback);
        return foundNode.has_value() ? std::optional<T>(foundNode.value()->value.second) : std::nullopt;
    }


    [[nodiscard]] bool has(const std::string& key) const {
        return keys.contains(key);
    }


    [[nodiscard]] std::vector<std::string> getKeys() const {
        std::vector<std::string> result;
        std::ranges::copy(keys | std::views::keys, std::back_inserter(result));
        return result;
    }

    [[nodiscard]] std::vector<T> getValues() const {
        std::vector<T> result;
        for (const auto& bucketList : buckets) {
            auto nodes = bucketList.toArray();  // 获取链表所有节点指针
            for (const auto& node : nodes) {
                result.push_back(node->value.second);
            }
        }
        return result;
    }
};