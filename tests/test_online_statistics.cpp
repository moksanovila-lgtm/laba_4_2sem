#include <gtest/gtest.h>
#include "OnlineStatistics.hpp"
#include "ReadOnlyStream.hpp"
#include "ArraySequence.hpp"

class OnlineStatisticsTest : public ::testing::Test {
protected:
    void SetUp() override {
        data = new ArraySequence<long long>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        stream = new ReadOnlyStream<long long>(data);
    }
    
    void TearDown() override {
        delete stream;
        delete data;
    }
    
    template<typename Ex, typename F>
    void expectThrow(F f, const std::string& ctx = "") {
        EXPECT_THROW(f(), Ex) << ctx;
    }
    
    ArraySequence<long long>* data;
    ReadOnlyStream<long long>* stream;
};

TEST_F(OnlineStatisticsTest, Min) {
    OnlineStatistics<long long> stats;
    stream->Open();
    while (!stream->IsEndOfStream()) {
        stats.Update(stream->Read());
    }
    stream->Close();
    long long min = stats.GetMin();
    EXPECT_EQ(min, 1) << "GetMin() on [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]: expected 1, got " << min;
}

TEST_F(OnlineStatisticsTest, Max) {
    OnlineStatistics<long long> stats;
    stream->Open();
    while (!stream->IsEndOfStream()) {
        stats.Update(stream->Read());
    }
    stream->Close();
    long long max = stats.GetMax();
    EXPECT_EQ(max, 10) << "GetMax() on [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]: expected 10, got " << max;
}

TEST_F(OnlineStatisticsTest, Average) {
    OnlineStatistics<long long> stats;
    stream->Open();
    while (!stream->IsEndOfStream()) {
        stats.Update(stream->Read());
    }
    stream->Close();
    double avg = stats.GetAverage();
    EXPECT_DOUBLE_EQ(avg, 5.5) << "GetAverage() on [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]: expected 5.5, got " << avg;
}

TEST_F(OnlineStatisticsTest, Count) {
    OnlineStatistics<long long> stats;
    stream->Open();
    while (!stream->IsEndOfStream()) {
        stats.Update(stream->Read());
    }
    stream->Close();
    size_t count = stats.GetCount();
    EXPECT_EQ(count, 10) << "GetCount() on [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]: expected 10, got " << count;
}

TEST_F(OnlineStatisticsTest, Variance) {
    OnlineStatistics<long long> stats;
    stream->Open();
    while (!stream->IsEndOfStream()) {
        stats.Update(stream->Read());
    }
    stream->Close();
    double variance = stats.GetVariance();
    EXPECT_NEAR(variance, 8.25, 0.1) << "GetVariance() on [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]: expected 8.25, got " << variance;
}

TEST_F(OnlineStatisticsTest, StdDeviation) {
    OnlineStatistics<long long> stats;
    stream->Open();
    while (!stream->IsEndOfStream()) {
        stats.Update(stream->Read());
    }
    stream->Close();
    double stdDev = stats.GetStdDeviation();
    EXPECT_NEAR(stdDev, 2.872, 0.01) << "GetStdDeviation() on [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]: expected 2.872, got " << stdDev;
}

TEST_F(OnlineStatisticsTest, RMS) {
    OnlineStatistics<long long> stats;
    for (int i = 1; i <= 10; ++i) {
        stats.Update(i);
    }
    double rms = stats.GetRMS();
    EXPECT_NEAR(rms, 6.2048, 0.01) << "GetRMS() on [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]: expected 6.2048, got " << rms;
}

TEST_F(OnlineStatisticsTest, Range) {
    OnlineStatistics<long long> stats;
    stream->Open();
    while (!stream->IsEndOfStream()) {
        stats.Update(stream->Read());
    }
    stream->Close();
    double range = stats.GetRange();
    EXPECT_EQ(range, 9) << "GetRange() on [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]: expected 9, got " << range;
}

TEST_F(OnlineStatisticsTest, CoefficientOfVariation) {
    OnlineStatistics<long long> stats;
    for (int i = 1; i <= 10; ++i) {
        stats.Update(i);
    }
    double cv = stats.GetCoefficientOfVariation();
    EXPECT_NEAR(cv, 0.522, 0.01) << "GetCoefficientOfVariation() on [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]: expected 0.522, got " << cv;
}

TEST_F(OnlineStatisticsTest, EmptySequenceThrows) {
    OnlineStatistics<long long> stats;
    expectThrow<EmptySequenceException>(
        [&stats]() { stats.GetMin(); },
        "GetMin() on empty statistics"
    );
    
    expectThrow<EmptySequenceException>(
        [&stats]() { stats.GetMax(); },
        "GetMax() on empty statistics"
    );
    
    expectThrow<EmptySequenceException>(
        [&stats]() { stats.GetAverage(); },
        "GetAverage() on empty statistics"
    );
    
    expectThrow<EmptySequenceException>(
        [&stats]() { stats.GetVariance(); },
        "GetVariance() on empty statistics"
    );
    
    expectThrow<EmptySequenceException>(
        [&stats]() { stats.GetRMS(); },
        "GetRMS() on empty statistics"
    );
    
    expectThrow<EmptySequenceException>(
        [&stats]() { stats.GetRange(); },
        "GetRange() on empty statistics"
    );
    
    expectThrow<EmptySequenceException>(
        [&stats]() { stats.GetCoefficientOfVariation(); },
        "GetCoefficientOfVariation() on empty statistics"
    );
}

TEST_F(OnlineStatisticsTest, Reset) {
    OnlineStatistics<long long> stats;
    stats.Update(10);
    stats.Update(20);
    stats.Update(30);
    EXPECT_EQ(stats.GetCount(), 3) << "Before reset: count expected 3, got " << stats.GetCount();
    EXPECT_EQ(stats.GetMin(), 10) << "Before reset: min expected 10, got " << stats.GetMin();
    EXPECT_EQ(stats.GetMax(), 30) << "Before reset: max expected 30, got " << stats.GetMax();
    EXPECT_DOUBLE_EQ(stats.GetAverage(), 20) << "Before reset: avg expected 20, got " << stats.GetAverage();
    stats.Reset();
    EXPECT_EQ(stats.GetCount(), 0) << "After reset: count expected 0, got " << stats.GetCount();
    expectThrow<EmptySequenceException>(
        [&stats]() { stats.GetMin(); },
        "GetMin() after reset"
    );
}

TEST_F(OnlineStatisticsTest, ConstantSequence) {
    OnlineStatistics<long long> stats;
    for (int i = 0; i < 10; ++i) {
        stats.Update(42);
    }
    EXPECT_EQ(stats.GetMin(), 42) << "Min of constant sequence: expected 42, got " << stats.GetMin();
    EXPECT_EQ(stats.GetMax(), 42) << "Max of constant sequence: expected 42, got " << stats.GetMax();
    EXPECT_DOUBLE_EQ(stats.GetAverage(), 42) << "Average of constant sequence: expected 42, got " << stats.GetAverage();
    EXPECT_DOUBLE_EQ(stats.GetVariance(), 0) << "Variance of constant sequence: expected 0, got " << stats.GetVariance();
    EXPECT_DOUBLE_EQ(stats.GetStdDeviation(), 0) << "StdDev of constant sequence: expected 0, got " << stats.GetStdDeviation();
    EXPECT_DOUBLE_EQ(stats.GetRMS(), 42) << "RMS of constant sequence: expected 42, got " << stats.GetRMS();
    EXPECT_DOUBLE_EQ(stats.GetRange(), 0) << "Range of constant sequence: expected 0, got " << stats.GetRange();
    EXPECT_DOUBLE_EQ(stats.GetCoefficientOfVariation(), 0) << "CV of constant sequence: expected 0, got " << stats.GetCoefficientOfVariation();
}

TEST_F(OnlineStatisticsTest, SingleElement) {
    OnlineStatistics<long long> stats;
    stats.Update(100); 
    EXPECT_EQ(stats.GetCount(), 1) << "Count: expected 1, got " << stats.GetCount();
    EXPECT_EQ(stats.GetMin(), 100) << "Min: expected 100, got " << stats.GetMin();
    EXPECT_EQ(stats.GetMax(), 100) << "Max: expected 100, got " << stats.GetMax();
    EXPECT_DOUBLE_EQ(stats.GetAverage(), 100) << "Average: expected 100, got " << stats.GetAverage();
    EXPECT_DOUBLE_EQ(stats.GetVariance(), 0) << "Variance: expected 0, got " << stats.GetVariance();
    EXPECT_DOUBLE_EQ(stats.GetRMS(), 100) << "RMS: expected 100, got " << stats.GetRMS();
    EXPECT_DOUBLE_EQ(stats.GetRange(), 0) << "Range: expected 0, got " << stats.GetRange();
}