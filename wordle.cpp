#ifndef RECCHECK
#endif

#include "wordle.h"
#include "dict-eng.h"
#include <string>
#include <set>
#include <vector>

using namespace std;

// Recursively check if a word w matches fixed pattern 'in' and is lowercase
static bool matchesFixed(const string& w, const string& in, size_t pos) {
    if (pos == in.size()) return true;
    char c = w[pos];
    if (c < 'a' || c > 'z') return false;
    if (in[pos] != '-' && in[pos] != c) return false;
    return matchesFixed(w, in, pos + 1);
}

// Recursively filter dictionary into 'valid' words matching fixed pattern
static void filterDict(set<string>::const_iterator it,
                       set<string>::const_iterator end,
                       const string& in,
                       vector<string>& valid) {
    if (it == end) return;
    const string& w = *it;
    if (w.size() == in.size() && matchesFixed(w, in, 0)) {
        valid.push_back(w);
    }
    auto next = it; ++next;
    filterDict(next, end, in, valid);
}

// Recursively scan valid words and insert those fulfilling floating constraints
static void recValid(const vector<string>& valid,
                     const string& floating,
                     size_t idx,
                     set<string>& results) {
    if (idx == valid.size()) return;
    const string& w = valid[idx];
    if (hasAll(w, floating)) {
        results.insert(w);
    }
    recValid(valid, floating, idx + 1, results);
}(const vector<string>& valid,
                     const string& floating,
                     size_t idx,
                     set<string>& results) {
    if (idx == valid.size()) return;
    const string& w = valid[idx];
    if (hasAll(w, floating)) {
        results.insert(w);
    }
    recValid(valid, floating, idx + 1, results);
}

set<string> wordle(const string& in,
                   const string& floating,
                   const set<string>& dict) {
    vector<string> valid;
    filterDict(dict.begin(), dict.end(), in, valid);
    set<string> results;
    recValid(valid, floating, 0, results);
    return results;
}
