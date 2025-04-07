#include "rule_eval.h"
#include "rule_parser.h"
#include "dataset_loader.h"
#include <fstream>
#include <iomanip>
#include <unordered_map>
#include <set>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " dataset.tsv rules.txt\n";
        return 1;
    }

    const std::string dataset_path = argv[1];
    const std::string rules_path = argv[2];
    const std::string output_path = "compressed_rules.txt";

    auto dataset = load_dataset(dataset_path);
    auto rules = load_rules(rules_path);

    double base_rate = base_old_proportion(dataset);
    const double min_lift = 1.2;

    SupportCache support_cache;
    std::vector<bool> keep(rules.size(), true);

    // Cache all rule supports
    for (size_t i = 0; i < rules.size(); ++i) {
        support_cache[rule_to_string(rules[i])] = rule_support(rules[i], dataset);
    }

    // Synergy-based compression
    for (size_t i = 0; i < rules.size(); ++i) {
        if (!keep[i]) continue;

        for (size_t j = 0; j < rules.size(); ++j) {
            if (i == j || !keep[j]) continue;

            const Rule& r1 = rules[i];
            const Rule& r2 = rules[j];

            if (!is_more_specific_than(r1, r2)) continue;

            double supp1 = support_cache[rule_to_string(r1)];
            double supp2 = support_cache[rule_to_string(r2)];
            double err2 = 1.0 - supp2;

            std::set<std::string> lhs_r1;
            for (const auto& p : r1.conditions)
                lhs_r1.insert((p.negated ? "NOT " : "") + p.name);

            double combined_error = 1.0 - supp1;

            for (const auto& p : r2.conditions) {
                std::string pred_str = (p.negated ? "NOT " : "") + p.name;
                if (lhs_r1.count(pred_str) == 0) {
                    Rule single_pred_rule{ { p } };
                    std::string key = rule_to_string(single_pred_rule);

                    if (support_cache.find(key) == support_cache.end()) {
                        support_cache[key] = rule_support(single_pred_rule, dataset);
                    }

                    double pred_error = 1.0 - support_cache[key];
                    combined_error *= pred_error;
                }
            }

            if (err2 >= combined_error) {
                keep[j] = false;
            }
        }
    }

    // Lift filtering
    std::vector<std::string> final_rules;
    for (size_t i = 0; i < rules.size(); ++i) {
        if (!keep[i]) continue;

        const Rule& rule = rules[i];
        double support = support_cache[rule_to_string(rule)];
        double lift = base_rate > 0.0 ? support / base_rate : 0.0;

        if (lift >= min_lift) {
            std::string rule_str;
            for (size_t j = 0; j < rule.conditions.size(); ++j) {
                const auto& p = rule.conditions[j];
                rule_str += (p.negated ? "NOT " : "") + p.name;
                if (j + 1 < rule.conditions.size()) rule_str += " AND ";
            }
            rule_str += " => donor_is_old";
            final_rules.push_back(rule_str);
        }
    }

    // Save to file
    std::ofstream out(output_path);
    for (const auto& line : final_rules) {
        out << line << "\n";
    }

    return 0;
}