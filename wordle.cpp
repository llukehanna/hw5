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

// Recursively build list of valid words
static void filterValid(const vector<string>& all,
                        const string& in,
                        size_t idx,
                        vector<string>& valid) {
    if (idx == all.size()) return;
    const string& w = all[idx];
    if (w.size() == in.size() && matchesFixed(w, in, 0)) {
        valid.push_back(w);
    }
    filterValid(all, in, idx + 1, valid);
}

// Recursively check that w contains all letters in 'need' with correct multiplicity
static bool hasAll(const string& w, const string& need) {
    if (need.empty()) return true;
    char c = need[0];
    size_t p = w.find(c);
    if (p == string::npos) return false;
    string w2 = w;
    w2.erase(p, 1);
    return hasAll(w2, need.substr(1));
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
}

set<string> wordle(const string& in,
                   const string& floating,
                   const set<string>& dict) {
    // copy all dict entries into a vector
    vector<string> all(dict.begin(), dict.end());
    vector<string> valid;
    filterValid(all, in, 0, valid);
    set<string> results;
    recValid(valid, floating, 0, results);
    return results;
}
