#pragma once

#include "_prelude.h"

#include "cons.h"
#include "vec.h"

namespace Karm::Base {

template <typename K, typename V>
struct Map {
    Vec<Cons<K, V>> _els;

    Map() = default;

    Map(std::initializer_list<Cons<K, V>> &&list)
        : _els(std::move(list)) {}

    void put(K const &key, V const &value) {
        for (auto &i : _els) {
            if (i.car == key) {
                i.cdr = value;
                return;
            }
        }

        _els.push(Cons<K, V>{key, value});
    }

    auto get(K const &key) -> Opt<V> {
        for (auto &i : _els) {
            if (i.car == key) {
                return i.cdr;
            }
        }

        return NONE;
    }

    auto begin() const {
        return _els.begin();
    }

    auto end() const {
        return _els.end();
    }

    auto len() -> size_t {
        return _els.len();
    }

    void clear() {
        _els.clear();
    }
};

} // namespace Karm::Base