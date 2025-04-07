# Rule Compression for Donor Classification

This tool compresses a set of logical rules used to classify blood donors as "old" based on biomarker expressions. It removes redundant, overly specific, or weak rules to produce a more compact and interpretable rule set.

## 🔧 Build Instructions

You need a C++17 compiler and Make. Run:

```bash
make
```

This builds the executable `rule_compress.exe`.

To clean build files:

```bash
make clean
```

## Usage

```bash
./rule_compress.exe dataset.tsv rules.txt
```

- `dataset.tsv`: Tab-separated file with donor records (TRUE/FALSE/NA values)
- `rules.txt`: List of logical rules in the form `LHS => donor_is_old`

Output is written to `compressed_rules.txt`.

## Rule Format

Each rule has the form:

```
LHS => donor_is_old
```

Only conjunctions (AND) and optional negations (NOT) are allowed in the LHS.

## Compression Heuristics

1. **Synergy-Based Compression**  
   More specific rules (e.g. `A AND B`) are removed if their error is not better than expected from simpler components:

   ```
   1 - support(A AND B) > (1 - support(A)) * (1 - support(B))
   ```

2. **Lift Threshold Filtering**  
   After structural compression, remaining rules are filtered by lift:

   ```
   Lift = support(rule) / base rate of old donors
   ```

   Only rules with **lift ≥ 1.2** are kept.

## Notes

- `NA` values in the dataset are treated as unknown and excluded from rule matches.
- All rules in `rules.txt` must follow the correct format (see above).