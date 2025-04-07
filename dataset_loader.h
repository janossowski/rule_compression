#ifndef DATASET_LOADER_H
#define DATASET_LOADER_H

#include "rule_eval.h"
#include <vector>
#include <string>

std::vector<Record> load_dataset(const std::string& filename);

#endif
