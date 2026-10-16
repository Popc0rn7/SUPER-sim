#pragma once
// Optional diagnostic only: clocks are local to this process/host.
#include <ctime>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <mutex>
namespace stage3_profile {
inline int64_t ns(clockid_t clock) {
    timespec value{};
    return clock_gettime(clock, &value)==0 ? int64_t(value.tv_sec)*1000000000LL+value.tv_nsec : -1;
}
struct Sink {
    std::ofstream stream;
    std::mutex mutex;
    Sink() {
        const char *path=std::getenv("SUPER_PROFILE_PATH");
        if (path && *path) stream.open(path, std::ios::app);
    }
};
inline Sink &sink() { static Sink value; return value; }
class Scope {
    const char *name_;
    bool enabled_;
    int64_t wall_=0, cpu_=0, process_=0;
public:
    explicit Scope(const char *name): name_(name), enabled_(sink().stream.is_open()) {
        if (enabled_) {
            wall_=ns(CLOCK_MONOTONIC);
            cpu_=ns(CLOCK_THREAD_CPUTIME_ID);
            process_=ns(CLOCK_PROCESS_CPUTIME_ID);
        }
    }
    ~Scope() {
        if (!enabled_) return;
        const auto cpu=ns(CLOCK_THREAD_CPUTIME_ID), process=ns(CLOCK_PROCESS_CPUTIME_ID), end=ns(CLOCK_MONOTONIC);
        auto &output=sink();
        std::lock_guard<std::mutex> guard(output.mutex);
        output.stream << "{\"scope\":\"" << name_ << "\",\"start_ns\":" << wall_
            << ",\"end_ns\":" << end << ",\"wall_ns\":" << end-wall_
            << ",\"thread_cpu_ns\":" << (cpu_<0 || cpu<0 ? -1 : cpu-cpu_)
            << ",\"process_cpu_ns\":" << (process_<0 || process<0 ? -1 : process-process_) << "}\n";
    }
};
}
