#pragma once

#include "util/types.hpp"
#include <string_view>

#ifndef CLOCKWORK_IS_TUNING
    #define CLOCKWORK_IS_TUNING 0
#endif

namespace Clockwork::tuned {

#define CLOCKWORK_TUNABLES(TUNE, NO_TUNE)                         \
                                                                  \
    /* RFP Values */                                              \
    TUNE(rfp_margin, 147, 40, 160, 4, 0.002)                      \
    NO_TUNE(rfp_depth, 7, 4, 10, .5, 0.002)                       \
                                                                  \
    /* NMP Values */                                              \
    NO_TUNE(nmp_depth, 3, 1, 10, .5, 0.002)                       \
    NO_TUNE(nmp_base_r, 3, 1, 10, .5, 0.002)                      \
    NO_TUNE(nmp_verif_min_depth, 14, 1, 40, .5, 0.002)            \
    TUNE(nmp_beta_margin, 23, 10, 60, 3, 0.002)                   \
    TUNE(nmp_beta_diff, 402, 200, 800, 38, 0.002)                 \
                                                                  \
    /* ProbCut Values */                                          \
    TUNE(probcut_margin, 372, 100, 500, 10, 0.002)                \
    TUNE(probcut_see, 204, 0, 200, 10, 0.002)                     \
    NO_TUNE(probcut_min_depth, 5, 1, 20, 0.5, 0.002)              \
                                                                  \
    /* SEE Values */                                              \
    TUNE(quiesce_see_threshold, 26, -1000, 100, 20, 0.002)        \
    TUNE(movepicker_see_capthist_divisor, 47, 16, 192, 10, 0.002) \
                                                                  \
    /* Stat Bonus */                                              \
    TUNE(stat_bonus_max, 2387, 948, 3792, 142, 0.002)             \
    TUNE(stat_bonus_quad, 8, 2, 8, .5, 0.002)                     \
    TUNE(stat_bonus_lin, 82, 60, 240, 9, 0.002)                   \
    TUNE(stat_bonus_sub, 82, 60, 240, 9, 0.002)                   \
                                                                  \
    /* Stat Malus */                                              \
    TUNE(stat_malus_max, 816, 948, 3792, 142, 0.002)              \
    TUNE(stat_malus_quad, 4, 2, 8, .5, 0.002)                     \
    TUNE(stat_malus_lin, 134, 60, 240, 9, 0.002)                  \
    TUNE(stat_malus_sub, 118, 60, 240, 9, 0.002)                  \
                                                                  \
    /* Search Params */                                           \
    TUNE(asp_window_delta, 22, 25, 100, 4, 0.002)                 \
    NO_TUNE(razor_depth, 7, 1, 20, 0.5, 0.002)                    \
    TUNE(razor_margin, 502, 353, 1414, 53, 0.002)                 \
    NO_TUNE(lmp_depth_mult, 3, 1, 20, 0.5, 0.002)                 \
                                                                  \
    /* Futility Pruning */                                        \
    TUNE(ffp_margin_base, 335, 250, 1000, 38, 0.002)              \
    TUNE(ffp_margin_mult, 95, 50, 200, 8, 0.002)                  \
    TUNE(ffp_hist_div, 16, 16, 64, 3, 0.002)                      \
    NO_TUNE(ffp_depth, 8, 1, 20, 0.5, 0.002)                      \
                                                                  \
    /* Quiet History Pruning */                                   \
    NO_TUNE(qhp_depth, 4, 1, 20, 0.5, 0.002)                      \
    TUNE(qhp_threshold, -1424, -4096, -1024, 154, 0.002)          \
                                                                  \
    /* SEE PVS */                                                 \
    TUNE(see_pvs_quiet, -48, -134, -33, 5, 0.002)                 \
    TUNE(see_pvs_noisy_quad, -10, -44, -11, 2, 0.002)             \
    TUNE(see_pvs_hist_mult, 10, 10, 40, 2, 0.002)                 \
                                                                  \
    /* Singular Extensions */                                     \
    NO_TUNE(sing_min_depth, 6, 1, 20, 0.5, 0.002)                 \
    NO_TUNE(sing_depth_margin, 3, 1, 20, 0.5, 0.002)              \
    TUNE(sing_beta_margin, 5, 2, 10, 1, 0.002)                    \
    TUNE(dext_margin, 27, 20, 80, 3, 0.002)                       \
    TUNE(dext_hist_div, 248, 256, 1024, 39, 0.002)                \
    TUNE(triext_margin, 117, 60, 240, 9, 0.002)                   \
    TUNE(triext_hist_div, 742, 256, 1024, 39, 0.002)              \
                                                                  \
    /* LMR */                                                     \
    TUNE(lmr_quiet_base, 583, 394, 1576, 59, 0.002)               \
    TUNE(lmr_quiet_div, 165, 104, 416, 16, 0.002)                 \
    TUNE(lmr_noisy_base, 247, 128, 512, 20, 0.002)                \
    TUNE(lmr_noisy_div, 180, 98, 394, 15, 0.002)                  \
    TUNE(lmr_pv_node_red, 632, 512, 2048, 77, 0.002)              \
    TUNE(lmr_alpha_raise_red, 370, 256, 1024, 38, 0.002)          \
    TUNE(lmr_not_improving_red, 1012, 256, 1024, 38, 0.002)       \
    TUNE(lmr_in_check_red, 610, 512, 2048, 77, 0.002)             \
    TUNE(lmr_cutnode_red, 994, 512, 2048, 77, 0.002)              \
    TUNE(lmr_no_tt_red, 1204, 512, 2048, 77, 0.002)               \
    TUNE(lmr_ttpv_red, 1410, 512, 2048, 77, 0.002)                \
    TUNE(lmr_tt_capture_red, 1143, 512, 2048, 77, 0.002)          \
    TUNE(lmr_fail_high_red, 1313, 512, 2048, 77, 0.002)           \
    TUNE(lmr_quiet_hist_base, 1220, 512, 2048, 77, 0.002)         \
    TUNE(lmr_hist_div, 9, 4, 16, 2, 0.002)                        \
    TUNE(lmr_fut_red_base, 422, 250, 1000, 38, 0.002)             \
    TUNE(lmr_fut_red_mult, 55, 50, 200, 8, 0.002)                 \
    TUNE(lmr_fut_red, 701, 512, 2048, 77, 0.002)                  \
    TUNE(lmr_max_red, 3569, 1536, 6144, 231, 0.002)               \
                                                                  \
    /* TIME MANAGEMENT */                                         \
    NO_TUNE(time_hard_limit, 256, 128, 512, 19, 0.002)            \
    NO_TUNE(time_soft_limit, 51, 25, 100, 3, 0.002)               \
    NO_TUNE(time_soft_increment, 512, 256, 1024, 38, 0.002)       \
    NO_TUNE(nodetm_min_factor, 512, 256, 1024, 38, 0.002)         \
    NO_TUNE(nodetm_avg_factor, 2048, 1024, 4096, 153, 0.002)      \
    NO_TUNE(nodetm_frac_factor, 1895, 948, 3792, 142, 0.002)      \
    NO_TUNE(d1plexity_base, 788, 394, 1576, 59, 0.002)            \
    NO_TUNE(d1plexity_max_complexity, 200, 100, 400, 15, 0.002)   \
    NO_TUNE(d1plexity_divisor, 386, 193, 772, 29, 0.002)          \
                                                                  \
    /* End of Tunables */

#define DEFINE_VARIABLE(NAME, DEFAULT, ...) inline i32 NAME = DEFAULT;
#define DEFINE_CONSTANT(NAME, DEFAULT, ...) constexpr i32 NAME = DEFAULT;

#if CLOCKWORK_IS_TUNING
// TUNEs are defined as variables, NO_TUNEs are defined as constexpr constants.
CLOCKWORK_TUNABLES(DEFINE_VARIABLE, DEFINE_CONSTANT)
#else
// Both TUNEs and NO_TUNEs are defined as constexpr constants.
CLOCKWORK_TUNABLES(DEFINE_CONSTANT, DEFINE_CONSTANT)
#endif

#undef DEFINE_VARIABLE
#undef DEFINE_CONSTANT

void uci_print_tunable_options();
void uci_print_tunable_values();
bool uci_parse_tunable(std::string_view name, std::string_view value);

}  // namespace Clockwork::tuned
