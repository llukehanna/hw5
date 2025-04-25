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

    // Check each dictionary word once
    for (const auto& w : dict) {
        // skip any word of wrong length
        if (w.size() != n) continue;
        // skip words with non-lowercase chars
        bool allLower = true;
        for (char c : w) {
            if (c < 'a' || c > 'z') {
                allLower = false;
                break;
            }
        }
        if (!allLower) continue;

        // Check fixed positions and build letter freq
        vector<int> wordFreq(26, 0);
        bool match = true;
        for (size_t i = 0; i < n; ++i) {
            char wch = w[i];
            if (in[i] != '-' && in[i] != wch) {
                match = false;
                break;
            }
            wordFreq[wch - 'a']++;
        }
        if (!match) continue;

        // Verify all floating letters are covered
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
