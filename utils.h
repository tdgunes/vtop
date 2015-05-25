//
// Created by Taha Doğan Güneş on 25/05/15.
//

#ifndef VTOP_UTILS_H
#define VTOP_UTILS_H


#include <sys/types.h>
#include <math.h>
#include <stdio.h>

// Adapted from http://www.mbeckler.org/blog/?p=114
static std::string pretty_bytes(uint64_t bytes) {
    char buf[256];
    const char* suffixes[7];
    suffixes[0] = "B";
    suffixes[1] = "KB";
    suffixes[2] = "MB";
    suffixes[3] = "GB";
    suffixes[4] = "TB";
    suffixes[5] = "PB";
    suffixes[6] = "EB";
    uint s = 0; // which suffix to use
    double count = bytes;
    while (count >= 1024 && s < 7)
    {
        s++;
        count /= 1024;
    }
    if (count - floor(count) == 0.0)
        sprintf(buf, "%d %s", (int)count, suffixes[s]);
    else
        sprintf(buf, "%.1f %s", count, suffixes[s]);
    return std::string(buf);
}


#endif //VTOP_UTILS_H
