#pragma once

#include <map>
#include <list>
#include <utility>
#include <algorithm>

#include <cassert>

namespace lru
{
template <typename K, typename V>
class lru_cache
{
private:
    using cache_map  = std::map<K, V>;
    using cache_list = std::list<K>;

    std::size_t capacity_;

    cache_map  searcher_;
    cache_list list_;

    void insert(K &&key, V &&val)
    {
        // FIXME find_if
        auto iter = std::find(list_.begin(), list_.end(), key);

        if (iter != list_.end()) {
            // A old value
            searcher_[*iter] = val;
            auto nk          = std::move(*iter);

            list_.erase(iter);
            list_.push_front(std::move(nk));
        } else {
            list_.push_front(std::forward<K>(key));
            searcher_.emplace(std::forward<K>(key), std::forward<K>(val));
        }
    }

    void adjust()
    {
        assert(!list_.empty());
        searcher_.erase(list_.back());
        list_.pop_back();
    }

    void update(K &&key)
    {
        assert(!list_.empty());

        auto iter = list_.begin();
        for (; iter != list_.end(); iter++) {
            if ((*iter) == key)
                break;
        }

        assert(iter != list_.end());
        auto nk = std::move(*iter);
        list_.erase(iter);
        list_.push_front(std::move(nk));
    }

public:
    lru_cache() :
        capacity_(16)
    {}

    lru_cache(std::size_t c) :
        capacity_(c)
    {}

    void set_capacity(std::size_t c)
    {
        capacity_ = c;
    }

    V    get(K &&key);
    V &  get_ref(K &&key);
    void put(K &&key, V &&v);
};

template <typename K, typename V>
inline V lru_cache<K, V>::get(K &&key)
{
    if (searcher_.find(key) != searcher_.end()) {
        update(std::forward<K>(key));
        return searcher_.at(std::forward<K>(key));
    }
    return V();
}

template <typename K, typename V>
inline V &lru_cache<K, V>::get_ref(K &&key)
{
    if (searcher_.find(key) != searcher_.end()) {
        update(std::forward<K>(key));
        return searcher_.at(std::forward<K>(key));
    } else {
        throw std::exception();
    }
}

template <typename K, typename V>
inline void lru_cache<K, V>::put(K &&key, V &&val)
{
    auto list_size = list_.size();
    if (searcher_.find(key) == searcher_.end() && list_size == capacity_) {
        adjust();
    }

    insert(std::forward<K>(key), std::forward<V>(val));
}
} // namespace lru
