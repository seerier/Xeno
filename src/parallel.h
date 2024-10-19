/*
 * parallel.h
 *
 * This file is based on PBRT-v3.
 * More information can be found at: http://www.pbrt.org/
 */


#pragma once

#include "xeno.h"
#include "vecmath.h"
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

namespace xeno {

// Parallel Declarations
class AtomicFloat {
  public:
    // Atomicfloat Public Methods
    explicit AtomicFloat(float v = 0) { bits = floatToBits(v); }
    operator float() const { return bitsToFloat(bits); }
    float operator=(float v) {
        bits = floatToBits(v);
        return v;
    }
    void add(float v) {
        uint32_t oldBits = bits, newBits;
        do {
            newBits = floatToBits(bitsToFloat(oldBits) + v);
        } while (!bits.compare_exchange_weak(oldBits, newBits));
    }

  private:
// Atomicfloat Private Data
    std::atomic<uint32_t> bits;
};

// Simple one-use barrier; ensures that multiple threads all reach a
// particular point of execution before allowing any of them to proceed
// past it.
//
// Note: this should be heap allocated and managed with a shared_ptr, where
// all threads that use it are passed the shared_ptr. This ensures that
// memory for the Barrier won't be freed until all threads have
// successfully cleared it.
class Barrier {
  public:
    Barrier(int count) : count(count) { CHECK_GT(count, 0); }
    ~Barrier() { CHECK_EQ(count, 0); }
    void Wait();

  private:
    std::mutex mutex;
    std::condition_variable cv;
    int count;
};

void ParallelFor(std::function<void(int64_t)> func, int64_t count,
                 int chunkSize = 1);
extern thread_local int ThreadIndex;
void ParallelFor2D(std::function<void(Point2i)> func, const Point2i &count);
int MaxThreadIndex();
int NumSystemCores();

void ParallelInit();
void ParallelCleanup();
void MergeWorkerThreadStats();

}  // namespace xeno