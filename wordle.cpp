#ifndef RECCHECK
// Debugging output disabled in production
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

    // Filter dictionary by fixed positions
    vector<string> valid;
    auto it = dict.begin();
    while (it != dict.end()) {
        const string& w = *it;
        ++it;
        if (w.size() != n) continue;
        bool ok = true;
        size_t idx = 0;
        while (idx < n) {
            char c = w[idx];
            if (c < 'a' || c > 'z' || (in[idx] != '-' && in[idx] != c)) {
                ok = false;
                break;
            }
            ++idx;
        }
        if (ok) valid.push_back(w);
    }

    // Check floating letters with recursion
    function<bool(const string&, const string&)> hasAll =
        [&](const string& w, const string& need) -> bool {
            if (need.empty()) return true;
            char c = need[0];
            size_t p = w.find(c);
            if (p == string::npos) return false;
            string w2 = w;
            w2.erase(p, 1);
            string need2 = need.substr(1);
            return hasAll(w2, need2);
        };

    set<string> results;
    // Recursively scan valid words
    function<void(size_t)> rec = [&](size_t idx) {
        if (idx >= valid.size()) return;
        const string& w = valid[idx];
        if (hasAll(w, floating)) {
            results.insert(w);
        }
        rec(idx + 1);
    };

    rec(0);
    return results;
}
