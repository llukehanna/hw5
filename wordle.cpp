#ifndef RECCHECK
// For debugging
#include <iostream>
#endif

#include "wordle.h"
#include "dict-eng.h"
#include <vector>
#include <string>
#include <set>

using namespace std;

std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    std::set<std::string> results;
    size_t n = in.size();

    // Build frequency of floating letters
    vector<int> floatFreq(26, 0);
    for (char c : floating) {
        if (c >= 'a' && c <= 'z')
            floatFreq[c - 'a']++;
    }

    // Iterate through dictionary once
    for (const auto& w : dict) {
        // only consider words of correct length
        if (w.size() != n) continue;
        // skip any word with non-lowercase letters
        bool allLower = true;
        for (char c : w) {
            if (c < 'a' || c > 'z') {
                allLower = false;
                break;
            }
        }
        if (!allLower) continue;

        bool match = true;
        // Check fixed positions
        for (size_t i = 0; i < n; ++i) {
            if (in[i] != '-' && in[i] != w[i]) {
                match = false;
                break;
            }
        }
        if (!match) continue;

        // Count letters in candidate word
        vector<int> wordFreq(26, 0);
        for (char c : w) {
            wordFreq[c - 'a']++;
        }
        // Ensure we have all floating letters
        for (int letter = 0; letter < 26; ++letter) {
            if (wordFreq[letter] < floatFreq[letter]) {
                match = false;
                break;
            }
        }
        if (!match) continue;

        results.insert(w);
    }

    return results;
}
