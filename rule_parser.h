#ifndef RULE_PARSER_H
#define RULE_PARSER_H

#include "rule_eval.h"
#include <vector>
#include <string>

std::vector<Rule> load_rules(const std::string& filename);

#endif
