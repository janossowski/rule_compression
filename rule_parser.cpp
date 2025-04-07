#include "rule_parser.h"
#include <fstream>
#include <sstream>

std::vector<Rule> load_rules(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Rule> rules;

    std::string line;
    while (std::getline(file, line)) {
        Rule rule;
        size_t arrow_pos = line.find("=>");
        if (arrow_pos == std::string::npos) continue;

        std::string lhs = line.substr(0, arrow_pos);
        std::stringstream ss(lhs);
        std::string token;

        while (std::getline(ss, token, ' ')) {
            if (token == "AND") continue;

            Predicate pred;
            if (token == "NOT") {
                std::getline(ss, token, ' ');
                pred.negated = true;
                pred.name = token;
            } else {
                pred.negated = false;
                pred.name = token;
            }
            rule.conditions.push_back(pred);
        }

        rules.push_back(rule);
    }

    return rules;
}
