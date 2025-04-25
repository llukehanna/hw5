#ifndef RECCHECK
#include <iostream>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// prototype for our recursive helper
static void wordleHelper(const string& pattern,
                         int index,
                         string& current,
                         string floating,
                         const set<string>& dict,
                         set<string>& result);

set<string> wordle(const string& in,
                   const string& floating,
                   const set<string>& dict) {
    set<string> result;
    string current;
    wordleHelper(in, 0, current, floating, dict, result);
    return result;
}

static void wordleHelper(const string& pattern,
                         int index,
                         string& current,
                         string floating,
                         const set<string>& dict,
                         set<string>& result) {
    // base case: full‐length candidate built
    if (index == (int)pattern.size()) {
        if (floating.empty() && dict.count(current))
            result.insert(current);
        return;
    }

    // fixed letter: just use it
    if (pattern[index] != '-') {
        current.push_back(pattern[index]);
        wordleHelper(pattern, index + 1, current, floating, dict, result);
        current.pop_back();
        return;
    }

    // blank spot: first try placing each floating letter
    for (int i = 0; i < (int)floating.size(); ++i) {
        char c = floating[i];
        current.push_back(c);
        string rem = floating;
        rem.erase(i, 1);          // consume that floating letter
        wordleHelper(pattern, index + 1, current, rem, dict, result);
        current.pop_back();
    }

    // then try every other letter a–z that isn't still needed by floating
    for (char c = 'a'; c <= 'z'; ++c) {
        if (floating.find(c) != string::npos) 
            continue;
        current.push_back(c);
        wordleHelper(pattern, index + 1, current, floating, dict, result);
        current.pop_back();
    }
}
