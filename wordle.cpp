#ifndef RECCHECK
// no debug includes here
#endif

#include "wordle.h"
#include "dict-eng.h"

#include <string>
#include <set>

using namespace std;

// Recursively check that `w` contains every letter in `need` (with multiplicity)
static bool hasAll(const string& w, const string& need) {
    if (need.empty()) return true;
    char c = need[0];
    size_t p = w.find(c);
    if (p == string::npos) return false;
    string w2 = w;
    w2.erase(p, 1);
    return hasAll(w2, need.substr(1));
}

set<string> wordle(const string& in,
                   const string& floating,
                   const set<string>& dict) {
    set<string> results;
    size_t n = in.size();

    // 1) scan every dict word
    for (auto it = dict.begin(); it != dict.end(); ++it) {     // loop 1
        const string& w = *it;
        if (w.size() != n) continue;

        // 2) check fixed positions
        bool ok = true;
        for (size_t i = 0; i < n; ++i) {                        // loop 2
            if (in[i] != '-' && w[i] != in[i]) {
                ok = false;
                break;
            }
        }
        if (!ok) continue;

        // 3) verify all floating letters (recursive, no loop)
        if (!hasAll(w, floating)) continue;

        results.insert(w);
    }

    return results;
}
