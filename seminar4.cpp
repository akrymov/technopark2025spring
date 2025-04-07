#include <iostream>
#include <string>
#include <assert.h>


size_t str_hash(const std::string &data) {
    size_t hash = 0;
    for (char i : data)
        hash = hash * 13 + i;
    return hash;
}

template<typename Key> struct Hasher;

template<> struct Hasher<std::string> {
    size_t operator()(const std::string& s) const {
        return str_hash(s);
    }
};

template<> struct Hasher<int> {
    size_t operator()(const int& i) const {
        return i;
    }
};

static constexpr size_t BUCKETS_SIZES_LIST[] = {
    7, 17, 37, 73, 149, 251, 509, 1021, 2027, 5003, 10837 //....
};

template<typename Key, typename Value, typename Hash = Hasher<Key>>
class HashTable {
    struct Node {
        Node(const Key& k, const Value& v) : key(k), value(v), next(nullptr) {
        }

        Key key;
        Value value;

        Node* next;
    };
public:
    HashTable(Hash h = Hash()) : buckets(nullptr), buckets_idx(0), buckets_size(0), items_count(0), hasher(h) {
    }
    ~HashTable() {
        for (int idx = 0; idx < buckets_size; ++idx) {
            Node* cur = buckets[idx];
            while (cur) {
                Node* tmp = cur;
                cur = cur->next;
                delete tmp;
            }
        }
        delete[] buckets;
    }

    HashTable(const HashTable&) = delete;
    HashTable(HashTable&&) = delete;

    HashTable& operator=(const HashTable&) = delete;
    HashTable& operator=(HashTable&&) = delete;

    Value* Find(const Key& k) {
        if (!buckets) {
            return nullptr;
        }

        size_t idx = hasher(k) % buckets_size;
        Node* cur = buckets[idx];
        while (cur) {
            if (cur->key == k) {
                return &cur->value;
            }
            cur = cur->next;
        }
        return nullptr;
    }

    bool Add(const Key& k, const Value& v) {
        if (Find(k)) {
            return false;
        }

        if ((buckets_size * 10 < items_count) || (buckets_size == 0)) {
            rehash();
        }

        size_t idx = hasher(k) % buckets_size;
        Node* node = new Node(k, v);
        node->next = buckets[idx];
        buckets[idx] = node;

        items_count++;
        return true;
    }

    bool Delete(const Key& k) {
        if (!Find(k)) {
            return false;
        }

        size_t idx = hasher(k) % buckets_size;
        Node* cur = buckets[idx];
        Node* prev = nullptr;
        while (cur) {
            if (cur->key == k) {
                if (prev == nullptr) {
                    buckets[idx] = cur->next;
                } else {
                    prev->next = cur->next;
                }
                delete cur;
                items_count++;

                return true;
            }
            prev = cur;
            cur = cur->next;
        }

        return false;
    }




private:
    void rehash() {
        size_t new_buckets_size = BUCKETS_SIZES_LIST[buckets_idx++];
        Node** new_buckets = new Node*[new_buckets_size];
        for (int i = 0; i < new_buckets_size; i++) {
            new_buckets[i] = nullptr;
        }

        for (int idx = 0; idx < buckets_size; ++idx) {
            Node* cur = buckets[idx];
            while (cur) {
                Node* tmp = cur;
                cur = cur->next;

                size_t new_idx = hasher(tmp->key) % new_buckets_size;
                tmp->next = new_buckets[new_idx];
                new_buckets[new_idx] = tmp;
            }
        }

        delete[] buckets;
        buckets = new_buckets;
        buckets_size = new_buckets_size;
    }

    Node** buckets;
    size_t buckets_idx; // BUCKETS_SIZES_LIST[buckets_idx]
    size_t buckets_size;
    size_t items_count;

    Hash hasher;
};

void tests() {
    {
        HashTable<int, int> ht;
        assert(ht.Add(1,1) == true);
        assert(*ht.Find(1) == 1);
        assert(ht.Find(2) == nullptr);
        assert(ht.Delete(2) == false);
        assert(ht.Delete(1) == true);
        assert(ht.Find(1) == nullptr);
    }
    {
        HashTable<std::string, int> ht;
        ht.Add("hello", 2);
        ht.Add("bye", 0);
        ht.Find("bye");
        ht.Add("bye", 0);
        ht.Delete("bye");
        ht.Find("bye");
        assert(*ht.Find("hello") == 2);
    }

    std::cout << "tests ok" << std::endl;
}

int main() {
    // tests();
    HashTable<std::string, int> ht;
    char operation = '\0';
    std::string key;

    while (std::cin >> operation >> key) {
        bool operation_result = false;
        switch (operation)
        {
        case '+':
            operation_result = ht.Add(key, 1);
            break;
        case '-':
            operation_result = ht.Delete(key);
            break;
        case '?':
            operation_result = (ht.Find(key) != nullptr);
            break;
        default:
            break;
        }
        if (operation_result) {
            std::cout << "OK" << std::endl;
        } else {
            std::cout << "FAIL" << std::endl;
        }
    }


    return 0;
}
