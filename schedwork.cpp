#ifndef RECCHECK
#include <vector>
#include <algorithm>
#include <iostream>
#endif

#include "schedwork.h"

using namespace std;

static const Worker_T INVALID_ID = (unsigned int)-1;

// forward‐declare helper
bool assignShifts(const AvailabilityMatrix& avail,
                  size_t dailyNeed,
                  size_t maxShifts,
                  DailySchedule& sched,
                  vector<size_t>& shiftCount,
                  size_t day,
                  size_t slot);

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
) {
    size_t n = avail.size();
    if (n == 0) return false;
    size_t k = avail[0].size();

    // initialize schedule to n days × dailyNeed slots
    sched.assign(n, vector<Worker_T>(dailyNeed, INVALID_ID));

    // track how many shifts each worker has
    vector<size_t> shiftCount(k, 0);

    // start recursion at day 0, slot 0
    return assignShifts(avail, dailyNeed, maxShifts, sched, shiftCount, 0, 0);
}

bool assignShifts(const AvailabilityMatrix& avail,
                  size_t dailyNeed,
                  size_t maxShifts,
                  DailySchedule& sched,
                  vector<size_t>& shiftCount,
                  size_t day,
                  size_t slot)
{
    size_t n = avail.size();
    size_t k = avail[0].size();

    // if we've filled all days, success
    if (day == n) return true;

    // if we've filled all slots for this day, move to next day
    if (slot == dailyNeed)
        return assignShifts(avail, dailyNeed, maxShifts, sched, shiftCount, day + 1, 0);

    // try every worker for this (day,slot)
    for (Worker_T w = 0; w < k; ++w) {
        // must be available today, under max shifts, and not already on this day
        if (!avail[day][w])               continue;
        if (shiftCount[w] >= maxShifts)   continue;
        // check duplicate on same day
        bool duplicate = false;
        for (size_t s = 0; s < slot; ++s) {
            if (sched[day][s] == w) {
                duplicate = true;
                break;
            }
        }
        if (duplicate) continue;

        // assign and recurse
        sched[day][slot] = w;
        ++shiftCount[w];
        if (assignShifts(avail, dailyNeed, maxShifts, sched, shiftCount, day, slot + 1))
            return true;
        // backtrack
        --shiftCount[w];
        sched[day][slot] = INVALID_ID;
    }

    // no valid worker found for this slot → backtrack
    return false;
}
