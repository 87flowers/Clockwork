#pragma once

#include "tuning/graph.hpp"
#include "tuning/value.hpp"
#include "util/types.hpp"

#include <atomic>
#include <exception>
#include <iostream>
#include <memory>

namespace Clockwork::Autograd {

class PairPlaceholder;

class Globals {
public:
    static Globals& get() {
        static Globals instance;
        return instance;
    }

    usize register_param(PairPlaceholder* param) {
        if (m_locked) {
            std::cerr << "Attempted to register new global parameter after Globals has been locked"
                      << std::endl;
            std::terminate();
        }
        usize index = m_pair_parameters.size();
        m_pair_parameters.push_back(param);
        return index;
    }

    void lock() {
        m_locked = true;
    }

    std::vector<PairPlaceholder*> get_pair_parameters() const {
        m_locked = true;
        return m_pair_parameters;
    }

private:
    mutable std::atomic<bool>     m_locked = false;
    std::vector<PairPlaceholder*> m_pair_parameters;
};

class PairPlaceholder {
public:
    explicit PairPlaceholder(f128 default_value, bool constant) :
        m_index(Globals::get().register_param(this)),
        m_default_value(default_value),
        m_constant(constant) {
    }

    static PairPlaceholder create_tunable(f64 a, f64 b) {
        return PairPlaceholder(f128::make(a, b), false);
    }

    static PairPlaceholder create(f64 a, f64 b) {
        return PairPlaceholder(f128::make(a, b), true);
    }

    operator PairPtr() const {
        return Graph::get()->get_pair_parameter(m_index);
    }

    usize index() const {
        return m_index;
    }

    f128 default_value() const {
        return m_default_value;
    }

    bool constant() const {
        return m_constant;
    }

private:
    usize m_index;
    f128  m_default_value;
    bool  m_constant;
};

inline std::ostream& operator<<(std::ostream& os, PairPlaceholder a) {
    os << static_cast<PairPtr>(a);
    return os;
}

inline PairPtr operator-(PairPlaceholder a) {
    return -static_cast<PairPtr>(a);
}

inline PairPtr operator+(PairPlaceholder a, PairPlaceholder b) {
    return static_cast<PairPtr>(a) + static_cast<PairPtr>(b);
}

inline PairPtr operator-(PairPlaceholder a, PairPlaceholder b) {
    return static_cast<PairPtr>(a) - static_cast<PairPtr>(b);
}

inline PairPtr operator*(PairPlaceholder a, i32 b) {
    return static_cast<PairPtr>(a) * b;
}

inline PairPtr operator/(PairPlaceholder a, i32 b) {
    return static_cast<PairPtr>(a) / b;
}

}  // namespace Clockwork::Autograd
