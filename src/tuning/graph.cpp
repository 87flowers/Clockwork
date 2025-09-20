#include "tuning/graph.hpp"
#include "tuning/globals.hpp"

namespace Clockwork::Autograd {

Graph::Graph() {
    for (PairPlaceholderPtr placeholder : Globals::get().m_pair_parameters) {
        register_param(
          std::make_shared<Pair>(placeholder->default_value(), placeholder->constant()));
    }
}

}  // namespace Clockwork::Autograd
