#include <gtest/gtest.h>
#include "LazySequence.hpp"
#include "ArraySequence.hpp"

class LazySequenceTest : public ::testing::Test {
protected:
    void SetUp() override {
        data = new ArraySequence<long long>{1, 2, 3, 4, 5};
        fromArraySeq = new LazySequence<long long>(data);
        
        constData = new ArraySequence<long long>{42, 42, 42, 42, 42};
        constSeq = new LazySequence<long long>(constData);
        
        emptyData = new ArraySequence<long long>();
        emptySeq = new LazySequence<long long>(emptyData);
        
        arithData = new ArraySequence<long long>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        arithSeq = new LazySequence<long long>(arithData);
    }
    
    void TearDown() override {
        delete fromArraySeq;
        delete constSeq;
        delete emptySeq;
        delete arithSeq;
        delete data;
        delete constData;
        delete emptyData;
        delete arithData;
    }
    
    template<typename Ex, typename F>
    void expectThrow(F f, const std::string& ctx = "") {
        EXPECT_THROW(f(), Ex) << ctx;
    }
    
    ArraySequence<long long>* data;
    ArraySequence<long long>* constData;
    ArraySequence<long long>* emptyData;
    ArraySequence<long long>* arithData;
    LazySequence<long long>* fromArraySeq;
    LazySequence<long long>* constSeq;
    LazySequence<long long>* emptySeq;
    LazySequence<long long>* arithSeq;
};

TEST_F(LazySequenceTest, GetFromArithmetic) {
    long long got0 = arithSeq->Get(0);
    EXPECT_EQ(got0, 1) << "Get(0): expected 1, got " << got0;
    long long got4 = arithSeq->Get(4);
    EXPECT_EQ(got4, 5) << "Get(4): expected 5, got " << got4;
    long long got9 = arithSeq->Get(9);
    EXPECT_EQ(got9, 10) << "Get(9): expected 10, got " << got9;
}

TEST_F(LazySequenceTest, GetFromConstant) {
    long long got0 = constSeq->Get(0);
    EXPECT_EQ(got0, 42) << "Get(0) from constant: expected 42, got " << got0;
    long long got4 = constSeq->Get(4);
    EXPECT_EQ(got4, 42) << "Get(4) from constant: expected 42, got " << got4;
}

TEST_F(LazySequenceTest, GetFromArray) {
    long long got0 = fromArraySeq->Get(0);
    EXPECT_EQ(got0, 1) << "Get(0): expected 1, got " << got0;
    long long got4 = fromArraySeq->Get(4);
    EXPECT_EQ(got4, 5) << "Get(4): expected 5, got " << got4;
}

TEST_F(LazySequenceTest, GetThrowsOutOfRange) {
    expectThrow<IndexOutOfRangeException>(
        [this]() { arithSeq->Get(100); },
        "Get(100) on sequence with 10 elements"
    );
}

TEST_F(LazySequenceTest, GetFirst) {
    long long got = arithSeq->GetFirst();
    EXPECT_EQ(got, 1) << "GetFirst(): expected 1, got " << got;
}

TEST_F(LazySequenceTest, GetFirstThrowsOnEmpty) {
    expectThrow<EmptySequenceException>(
        [this]() { emptySeq->GetFirst(); },
        "GetFirst() on empty sequence"
    );
}

TEST_F(LazySequenceTest, GetLast) {
    long long got = fromArraySeq->GetLast();
    EXPECT_EQ(got, 5) << "GetLast() on [1,2,3,4,5]: expected 5, got " << got;
}

TEST_F(LazySequenceTest, GetCount) {
    size_t got = fromArraySeq->GetCount();
    EXPECT_EQ(got, 5) << "GetCount() on [1,2,3,4,5]: expected 5, got " << got;
}

TEST_F(LazySequenceTest, IsInfinite) {
    EXPECT_FALSE(arithSeq->IsInfinite()) << "Sequence from array should be finite";
    EXPECT_FALSE(fromArraySeq->IsInfinite()) << "Sequence from array should be finite";
}

TEST_F(LazySequenceTest, Append) {
    std::string before = std::string(*fromArraySeq);
    auto* result = fromArraySeq->Append(6);
    std::string got = std::string(*result);
    std::string expected = "[1, 2, 3, 4, 5, 6]";
    EXPECT_EQ(got, expected) << "Append(6): input=" << before << ", expected " << expected << ", got " << got;
    delete result;
}

TEST_F(LazySequenceTest, Prepend) {
    std::string before = std::string(*fromArraySeq);
    auto* result = fromArraySeq->Prepend(0);
    std::string got = std::string(*result);
    std::string expected = "[0, 1, 2, 3, 4, 5]";
    EXPECT_EQ(got, expected) << "Prepend(0): input=" << before << ", expected " << expected << ", got " << got;
    delete result;
}

TEST_F(LazySequenceTest, InsertAt) {
    std::string before = std::string(*fromArraySeq);
    auto* result = fromArraySeq->InsertAt(99, 2);
    std::string got = std::string(*result);
    std::string expected = "[1, 2, 99, 3, 4, 5]";
    EXPECT_EQ(got, expected) << "InsertAt(99,2): input=" << before << ", expected " << expected << ", got " << got;
    delete result;
}

TEST_F(LazySequenceTest, GetSubsequence) {
    std::string before = std::string(*fromArraySeq);
    auto* sub = fromArraySeq->GetSubsequence(1, 3);
    std::string got = std::string(*sub);
    std::string expected = "[2, 3, 4]";
    EXPECT_EQ(got, expected) << "GetSubsequence(1,3): input=" << before << ", expected " << expected << ", got " << got;
    delete sub;
}

TEST_F(LazySequenceTest, Concat) {
    std::string before = std::string(*fromArraySeq);
    ArraySequence<long long> otherData{6, 7, 8};
    LazySequence<long long> other(&otherData);
    auto* result = fromArraySeq->Concat(&other);
    std::string got = std::string(*result);
    std::string expected = "[1, 2, 3, 4, 5, 6, 7, 8]";
    EXPECT_EQ(got, expected) << "Concat: input=" << before << " + [6,7,8], expected " << expected << ", got " << got;
    delete result;
}

TEST_F(LazySequenceTest, Map) {
    std::string before = std::string(*fromArraySeq);
    auto doubleFunc = [](const long long& x) { return x * 2; };
    auto* result = fromArraySeq->Map(doubleFunc);
    std::string got = std::string(*result);
    std::string expected = "[2, 4, 6, 8, 10]";
    EXPECT_EQ(got, expected) << "Map(x*2): input=" << before << ", expected " << expected << ", got " << got;
    delete result;
}

TEST_F(LazySequenceTest, Where) {
    std::string before = std::string(*fromArraySeq);
    auto isEven = [](const long long& x) { return x % 2 == 0; };
    auto* result = fromArraySeq->Where(isEven);
    std::string got = std::string(*result);
    std::string expected = "[2, 4]";
    EXPECT_EQ(got, expected) << "Where(even): input=" << before << ", expected " << expected << ", got " << got;
    delete result;
}

TEST_F(LazySequenceTest, Reduce) {
    std::string before = std::string(*fromArraySeq);
    auto sum = [](const long long& a, const long long& b) { return a + b; };
    long long result = fromArraySeq->Reduce(sum, 0);
    EXPECT_EQ(result, 15) << "Reduce(sum): input=" << before << ", expected 15, got " << result;
}