//
//  Timer.hpp
//  test
//
//  Created by Freddis on 04.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef Timer_hpp
#define Timer_hpp

#include <stdio.h>
#include <chrono>
#include <sys/time.h>


class Timer
{
    int waitDurationMs;
    long startTimeMs = 0;
public:
    Timer(int ms)
    {
        this->waitDurationMs = ms;
    }
    void start()
    {
        struct timeval tp;
        gettimeofday(&tp, NULL);
        long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
        startTimeMs = ms;
    }
    bool isReady()
    {
        if(startTimeMs == 0)
        {
            return true;
        }
        struct timeval tp;
        gettimeofday(&tp, NULL);
        long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
        auto result = startTimeMs + waitDurationMs < ms;
        return result;
    }
    int getPassedTimeMs()
    {
        struct timeval tp;
        gettimeofday(&tp, NULL);
        long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
        auto result = ms - startTimeMs;
        return result;
    }
};
#endif /* Timer_hpp */
