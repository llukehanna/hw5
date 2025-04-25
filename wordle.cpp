#ifndef RECCHECK
// For debugging
#include <iostream>
#endif

#include "wordle.h"
#include "dict-eng.h"
#include <set>
#include <string>

// Recursive helper:
// - in:      the pattern string with '-' and fixed letters
// - pos:     current index we are filling
// - current: working copy of the word being built
// - floating: letters that still must appear somewhere
// - dict:    the full dictionary
// - results: set to collect valid words
static void helper(const std::string& in,
                   int pos,
                   std::string& current,
                   std::string floating,
                   const std::set<std::string>& dict,
                   std::set<std::string>& results)
{
    int n = in.size();
    if (pos == n) {
        // once fully built, only accept if all floating letters used
        if (floating.empty() && dict.find(current) != dict.end()) {
            results.insert(current);
        }
        return;
    }

    if (in[pos] != '-') {
        // fixed letter, just advance
        helper(in, pos + 1, current, floating, dict, results);
    } else {
        // try every letter 'a'..'z' in this blank
        for (char c = 'a'; c <= 'z'; ++c) {
            current[pos] = c;
            // if this letter satisfies one of the floating ones, remove it
            std::string nextFloating = floating;
            auto it = nextFloating.find(c);
            if (it != std::string::npos) {
                nextFloating.erase(it, 1);
            }
            helper(in, pos + 1, current, nextFloating, dict, results);
        }
        // restore (not strictly necessary since next call will overwrite)
        current[pos] = '-';
    }
}

std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    std::set<std::string> results;
    std::string current = in;     // start with the pattern
    helper(in, 0, current, floating, dict, results);
    return results;
}
