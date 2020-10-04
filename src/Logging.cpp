/**
  *
  * Copyright 2020 Zoltan Bacskai
  *
  **/
#include <Logging.hpp>

#include <chrono>
#include <stdio.h>
#include <time.h>

namespace logging {

std::string getCurrentTimestamp()
{
    using std::chrono::system_clock;
    auto currentTime = std::chrono::system_clock::now();
    char buffer1[11];
    char buffer2[16];

    auto transformed = currentTime.time_since_epoch().count() / 1000000;

    auto millis = transformed % 1000;

    std::time_t tt;
    tt = system_clock::to_time_t ( currentTime );
    auto timeinfo = localtime (&tt);
    strftime (buffer1,80,"%F %H:%M:%S",timeinfo);
    sprintf(buffer2, "%s:%03d",buffer1, static_cast<int>(millis));

    return std::string(buffer2);
}


}
