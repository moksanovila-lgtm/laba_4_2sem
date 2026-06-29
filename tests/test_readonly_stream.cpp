#include <gtest/gtest.h>
#include "ReadOnlyStream.hpp"
#include "ArraySequence.hpp"
#include "LazySequence.hpp"

class ReadOnlyStreamTest : public ::testing::Test {
protected:
    void SetUp() override {
        data = new ArraySequence<long long>{10, 20, 30, 40, 50};
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

TEST_F(ReadOnlyStreamTest, Read) {
    long long got1 = stream->Read();
    EXPECT_EQ(got1, 10) << "Read() on [10, 20, 30, 40, 50]: expected 10, got " << got1;
    long long got2 = stream->Read();
    EXPECT_EQ(got2, 20) << "Read() on [10, 20, 30, 40, 50] expected 20, got " << got2;
    long long got3 = stream->Read();
    EXPECT_EQ(got3, 30) << "Read() on [10, 20, 30, 40, 50]: expected 30, got " << got3;
}

TEST_F(ReadOnlyStreamTest, IsEndOfStream) {
    EXPECT_FALSE(stream->IsEndOfStream()) << "IsEndOfStream() should be false before reading all";
    for (int i = 0; i < 5; ++i) {
        stream->Read();
    }
    EXPECT_TRUE(stream->IsEndOfStream()) << "IsEndOfStream() should be true after reading all";
}

TEST_F(ReadOnlyStreamTest, GetPosition) {
    EXPECT_EQ(stream->GetPosition(), 0) << "GetPosition() on [10, 20, 30, 40, 50] after open: expected 0, got " << stream->GetPosition();
    stream->Read();
    EXPECT_EQ(stream->GetPosition(), 1) << "GetPosition() on [10, 20, 30, 40, 50] after first read: expected 1, got " << stream->GetPosition();
    stream->Read();
    EXPECT_EQ(stream->GetPosition(), 2) << "GetPosition() on [10, 20, 30, 40, 50] after second read: expected 2, got " << stream->GetPosition();
}

TEST_F(ReadOnlyStreamTest, Seek) {
    size_t pos = stream->Seek(2);
    EXPECT_EQ(pos, 2) << "Seek(2): return value expected 2, got " << pos;
    EXPECT_EQ(stream->GetPosition(), 2) << "Position after Seek(2): expected 2, got " << stream->GetPosition();
    long long val = stream->Read();
    EXPECT_EQ(val, 30) << "Read() on [10, 20, 30, 40, 50] after Seek(2): expected 30, got " << val;
}

TEST_F(ReadOnlyStreamTest, SeekThrowsOutOfRange) {
    expectThrow<IndexOutOfRangeException>(
        [this]() { stream->Seek(100); },
        "Seek(100) on stream with 5 elements"
    );
}

TEST_F(ReadOnlyStreamTest, Reset) {
    stream->Read();  
    stream->Read();  
    EXPECT_EQ(stream->GetPosition(), 2) << "Position before reset: expected 2, got " << stream->GetPosition();
    stream->Reset();
    EXPECT_EQ(stream->GetPosition(), 0) << "Position after reset: expected 0, got " << stream->GetPosition();
    long long val = stream->Read();
    EXPECT_EQ(val, 10) << "Read() on [10, 20, 30, 40, 50] after reset: expected 10, got " << val;
}

TEST_F(ReadOnlyStreamTest, Peek) {
    long long peeked = stream->Peek();
    EXPECT_EQ(peeked, 10) << "Peek(): expected 10, got " << peeked;
    EXPECT_EQ(stream->GetPosition(), 0) << "Position after Peek(): expected 0, got " << stream->GetPosition();
    long long read = stream->Read();
    EXPECT_EQ(read, 10) << "Read() on [10, 20, 30, 40, 50] after Peek(): expected 10, got " << read;
}

TEST_F(ReadOnlyStreamTest, PeekThrowsAtEnd) {
    for (int i = 0; i < 5; ++i) {
        stream->Read();
    }
    
    expectThrow<EndOfStreamException>(
        [this]() { stream->Peek(); },
        "Peek() at end of stream"
    );
}

TEST_F(ReadOnlyStreamTest, IsCanSeek) {
    EXPECT_TRUE(stream->IsCanSeek()) << "IsCanSeek() on ArraySequence stream: expected true";
}

TEST_F(ReadOnlyStreamTest, IsCanGoBack) {
    EXPECT_TRUE(stream->IsCanGoBack()) << "IsCanGoBack() on ArraySequence stream: expected true";
}