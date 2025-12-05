// wordle.cpp

#ifndef RECCHECK
#include <iostream>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// recursive helper
static void wordleHelper(const string& pattern,
                         int idx,
                         string& current,
                         string floating,
                         const set<string>& dict,
                         set<string>& result) {
    // ---- PREFIX PRUNING (new) ----
    if (!current.empty()) {
        // find first dict word >= current
        auto it = dict.lower_bound(current);
        // if none, or it doesn't actually start with 'current', bail out
        if (it == dict.end() ||
            it->compare(0, current.size(), current) != 0)
            return;
    }

    // base case: full length
    if (idx == (int)pattern.size()) {
        if (floating.empty() && dict.count(current))
            result.insert(current);
        return;
    }

    // fixed letter slot
    if (pattern[idx] != '-') {
        current.push_back(pattern[idx]);
        wordleHelper(pattern, idx + 1, current, floating, dict, result);
        current.pop_back();
        return;
    }

    // blank slot: first try each floating letter
    for (int i = 0; i < (int)floating.size(); ++i) {
        char c = floating[i];
        current.push_back(c);
        string rem = floating;
        rem.erase(i, 1);            // consume that floating letter
        wordleHelper(pattern, idx + 1, current, rem, dict, result);
        current.pop_back();
    }
    // then try every other a–z not in floating
    for (char c = 'a'; c <= 'z'; ++c) {
        if (floating.find(c) != string::npos) continue;
        current.push_back(c);
        wordleHelper(pattern, idx + 1, current, floating, dict, result);
        current.pop_back();
    }
}

set<string> wordle(const string& in,
                   const string& floating,
                   const set<string>& dict) {
    set<string> result;
    string current;
    wordleHelper(in, 0, current, floating, dict, result);
    return result;
}
