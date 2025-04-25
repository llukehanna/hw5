#ifndef RECCHECK
// For debugging
#include <iostream>
#endif

#include "wordle.h"
#include "dict-eng.h"

#include <vector>
#include <string>
#include <set>
#include <functional>

using namespace std;

std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    size_t n = in.size();

    // Gather dictionary words matching fixed positions
    std::vector<std::string> valid;
    for (const auto& w : dict) {             // loop1
        if (w.size() != n) continue;
        bool ok = true;
        for (size_t i = 0; i < n; ++i) {     // loop2
            char c = w[i];
            if (c < 'a' || c > 'z' || (in[i] != '-' && in[i] != c)) {
                ok = false;
                break;
            }
        }
        if (ok) valid.push_back(w);
    }

    // Recursive function to check floating letters with multiplicity
    std::function<bool(const std::string&, const std::string&)> hasAll =
        [&](const std::string& w, const std::string& need) -> bool {
            if (need.empty()) return true;
            char c = need[0];
            size_t p = w.find(c);
            if (p == std::string::npos) return false;
            std::string w2 = w;
            w2.erase(p, 1);
            std::string need2 = need.substr(1);
            return hasAll(w2, need2);
        };

    std::set<std::string> results;

    // Recursively scan 'valid' list to insert words satisfying floats
    std::function<void(size_t)> rec = [&](size_t idx) {
        if (idx == valid.size()) return;
        const std::string& w = valid[idx];
        if (hasAll(w, floating)) {
            results.insert(w);
        }
        rec(idx + 1);
    };

    rec(0);
    return results;
}
