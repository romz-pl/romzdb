#include <unordered_map>
#include <list>
#include <assert.h>

template<typename K, typename V>
class bimap {
    typedef typename std::unordered_map<K, V>::iterator iter;
    typedef typename std::unordered_map<K, V>::const_iterator const_iter;

    std::unordered_map<K, V> key_to_value;
    std::unordered_map<V, iter> value_to_key;

public:
    bimap() = default;

    bimap(bimap const& other) {
        for (auto key_value_pair : other) { 
            auto insert_to_key_value_map = key_to_value.insert(key_value_pair);
            if (!insert_to_key_value_map.second) 
                throw std::runtime_error("fail in copy ctor");
            auto insert_to_value_iter_map = value_to_key.emplace(key_value_pair.second, 
                    insert_to_key_value_map.first);
            if (!insert_to_value_iter_map.second)
                throw std::runtime_error("fail in copy ctor");
        }
    }

    size_t erase_by_key(K const& key) {
        auto iter = key_to_value.find(key);
        if (iter == key_to_value.end()) 
            return 0;
        // assert(1 == value_to_key.erase(iter->second));
        // assert(1 == key_to_value.erase(iter->first));
        value_to_key.erase(iter->second);
        key_to_value.erase(iter->first);
        return 1;
    }

    size_t erase_by_value(V const& value) {
        auto iter = value_to_key.find(value);
        if (iter == value_to_key.end())
            return 0;
        //assert(1 == key_to_value.erase(iter->second->first));
        //assert(1 == value_to_key.erase(iter->first));
        key_to_value.erase(iter->second->first);
        value_to_key.erase(iter->first);
        return 1;
    }

    bool insert(K const& key, V const& value) {
        assert(key_to_value.size() == value_to_key.size());
        auto insert_to_key_map = key_to_value.emplace(key, value);
        if (!insert_to_key_map.second) {
            return false;
        }
        auto insert_to_value_map = value_to_key.emplace(value, insert_to_key_map.first);
        if (!insert_to_value_map.second) {
            key_to_value.erase(insert_to_key_map.first);
            return false;
        }
        assert(key_to_value.size() == value_to_key.size());
        return true;
    }

    K const& get_key_by_value(V const& value) {
        return value_to_key.at(value)->first;
    }

    V const& get_value_by_key(K const& key) {
        return key_to_value.at(key);
    }

    void clear() {
        key_to_value.clear();
        value_to_key.clear();
    }

    bool empty() const noexcept {
        return key_to_value.empty();
    }

    size_t size() const noexcept {
        return key_to_value.size();
    }

    class iterator {
        const_iter map_iterator;

    public:
        typedef std::input_iterator_tag iterator_category;

        iterator(const_iter map_iterator) : map_iterator{map_iterator} {}
        
        iterator& operator++() {
            ++map_iterator;
            return *this; 
        }

        iterator operator++(int) {
            iterator old = this;
            this++;
            return old;
        }

        typename const_iter::value_type operator*() const {
            return *map_iterator;
        }
        
        typename const_iter::pointer operator->() const {
            return map_iterator.operator->();
        }

        bool operator==(iterator const& rhs) {
            return map_iterator == rhs.map_iterator;            
        }

        bool operator!=(iterator const& rhs) {
            return map_iterator != rhs.map_iterator;
        }

        void swap(iterator& rhs) {
            swap(map_iterator, rhs.map_iterator);
        }
    };

    iterator begin() const noexcept {
        return iterator(key_to_value.cbegin());
    }
    
    iterator end() const noexcept {
        return iterator(key_to_value.cend());
    }
};

