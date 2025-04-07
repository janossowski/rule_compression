#ifndef RULE_EVAL_H
#define RULE_EVAL_H

#include <string>
#include <vector>
#include <unordered_map>

// A single row in the dataset
using Record = std::unordered_map<std::string, std::string>;

// Represents a single predicate in a rule
struct Predicate {
    std::string name;
    bool negated;

    bool evaluate(const Record& record) const;
};

// Represents a rule: a conjunction of predicates => donor_is_old
struct Rule {
    std::vector<Predicate> conditions;

    bool evaluate(const Record& record) const;
};

// Compute proportion of records where the rule is true AND donor_is_old is true
// (i.e., support of the rule)
double rule_support(const Rule& rule, const std::vector<Record>& dataset);

// Compute overall percentage of old donors in the dataset
double base_old_proportion(const std::vector<Record>& dataset);

using SupportCache = std::unordered_map<std::string, double>;

// Helper to serialize a rule's LHS to string for caching
std::string rule_to_string(const Rule& rule);

bool is_more_specific_than(const Rule& a, const Rule& b);

#endif
