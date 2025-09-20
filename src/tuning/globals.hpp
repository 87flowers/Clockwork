#pragma once

#include "tuning/graph.hpp"
#include "tuning/value.hpp"
#include "util/types.hpp"

#include <memory>

namespace Clockwork::Autograd {

class PairPlaceholder;
using PairPlaceholderPtr = std::shared_ptr<PairPlaceholder>;

class Globals {
public:
    static Globals& get() {
        static Globals instance;
        return instance;
    }

    usize register_param(PairPlaceholderPtr param) {
        usize index = m_pair_parameters.size();
        m_pair_parameters.push_back(param);
        return index;
    }

    std::vector<PairPlaceholderPtr> m_pair_parameters;
};

class PairPlaceholder : public std::enable_shared_from_this<PairPlaceholder> {
public:
    explicit PairPlaceholder(f128 default_value, bool constant) :
        m_index(Globals::get().register_param(shared_from_this())),
        m_default_value(default_value),
        m_constant(constant) {
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

}  // namespace Clockwork::Autograd
