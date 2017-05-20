#pragma once

#include <thread>
#include <vector>


struct EasyThreads {

    template <typename F> EasyThreads& Async(F function) {
        auto t = new std::thread(function);
        return *this;
    }

};
