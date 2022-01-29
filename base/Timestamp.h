#ifndef BASE_TIMESTAMP_H
#define BASE_TIMESTAMP_H

#include <string>
#include <stdint.h>

using std::string;

class Timestamp {
    public:
        Timestamp()
            : microSecondsSinceEpoch_(0)
        {}

        explicit Timestamp(int64_t microSecond)
            : microSecondsSinceEpoch_(microSecond)
        {}

        void swap(Timestamp &that) {
            std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
        }

        string toString() const;

        string toFormattedString(bool showMicroseconds) const;

        int64_t microSecondsSinceEpoch() const {
            return microSecondsSinceEpoch_;
        }

        bool valid() const {
            return microSecondsSinceEpoch_ > 0;
        }

        time_t secondSinceEpoch() const {
            return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
        }

        static Timestamp now();

        static Timestamp invalid() {
            return Timestamp();
        }

        static Timestamp fromUnixTime(time_t t) {
            return fromUnixTime(t, 0);
        }

        static Timestamp fromUnixTime(time_t t, int microseconds) {
            return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microseconds);
        }
        
        static const int kMicroSecondsPerSecond = 1000 * 1000;
    private:
        int64_t microSecondsSinceEpoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs) {
  return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs) {
  return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

/**
 * @brief Gets time difference of two timestamps, result in seconds
 * 
 * @param high 
 * @param low 
 * @return (high-low) in seconds 
 */

inline double timeDifference(Timestamp high, Timestamp low) {
  int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
  return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

/**
 * @brief Add seconds to given timestamp
 * 
 * @param timestamp 
 * @param seconds 
 * @return timestamp+seconds as Timestamp 
 */

inline Timestamp addTime(Timestamp timestamp, double seconds)
{
  int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}

#endif