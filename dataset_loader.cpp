#include "dataset_loader.h"
#include <fstream>
#include <sstream>

std::vector<Record> load_dataset(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Record> records;

    std::string line;
    std::vector<std::string> headers;

    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string col;
        while (std::getline(ss, col, '\t')) {
            headers.push_back(col);
        }
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        Record record;
        size_t i = 0;

        while (std::getline(ss, value, '\t') && i < headers.size()) {
            record[headers[i]] = value;
            ++i;
        }

        records.push_back(record);
    }

    return records;
}
