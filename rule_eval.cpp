#include "rule_eval.h"
#include <iostream>
#include <algorithm>

bool Predicate::evaluate(const Record& record) const {
    auto it = record.find(name);
    if (it == record.end()) return false;

    std::string value = it->second;
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);

    bool result = (value == "true");

    return negated ? !result : result;
}


bool Rule::evaluate(const Record& record) const {
    for (const auto& pred : conditions) {
        if (!pred.evaluate(record))
            return false;
    }
    return true;
}

double rule_support(const Rule& rule, const std::vector<Record>& dataset) {
    int matched = 0;
    int matched_and_old = 0;

    for (const auto& record : dataset) {
        if (rule.evaluate(record)) {
            matched++;

            auto it = record.find("donor_is_old");
            if (it != record.end()) {
                std::string val = it->second;
                std::transform(val.begin(), val.end(), val.begin(), ::tolower);
                if (val == "true") {
                    matched_and_old++;
                }
            }
        }
    }

    return matched == 0 ? 0.0 : static_cast<double>(matched_and_old) / matched;
}

double base_old_proportion(const std::vector<Record>& dataset) {
    int total = 0;
    int old = 0;

    for (const auto& record : dataset) {
        auto it = record.find("donor_is_old");
        if (it != record.end()) {
            std::string val = it->second;
            std::transform(val.begin(), val.end(), val.begin(), ::tolower);
            if (val == "true") old++;
            total++;
        }
    }

    return total == 0 ? 0.0 : static_cast<double>(old) / total;
}

std::string rule_to_string(const Rule& rule) {
    std::vector<std::string> parts;
    for (const auto& pred : rule.conditions) {
        parts.push_back((pred.negated ? "NOT " : "") + pred.name);
    }
    std::sort(parts.begin(), parts.end());
    std::string out;
    for (size_t i = 0; i < parts.size(); ++i) {
        out += parts[i];
        if (i + 1 < parts.size()) out += " AND ";
    }
    return out;
}

bool is_more_specific_than(const Rule& a, const Rule& b) {
    // b must contain all of a's predicates, and strictly more
    if (b.conditions.size() <= a.conditions.size()) return false;

    for (const auto& pred_a : a.conditions) {
        bool found = false;
        for (const auto& pred_b : b.conditions) {
            if (pred_a.name == pred_b.name && pred_a.negated == pred_b.negated) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }

    return true; // b is a strict superset of a
}