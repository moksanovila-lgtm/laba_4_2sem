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

TEST_F(ReadOnlyStreamTest, OpenAndClose) {
    EXPECT_NO_THROW(stream->Open()) << "Open() should not throw";
    EXPECT_NO_THROW(stream->Close()) << "Close() should not throw";
}

TEST_F(ReadOnlyStreamTest, Read) {
    stream->Open();
    long long got1 = stream->Read();
    EXPECT_EQ(got1, 10) << "Read() on [10, 20, 30, 40, 50]: expected 10, got " << got1;
    long long got2 = stream->Read();
    EXPECT_EQ(got2, 20) << "Read() on [10, 20, 30, 40, 50] expected 20, got " << got2;
    long long got3 = stream->Read();
    EXPECT_EQ(got3, 30) << "Read() on [10, 20, 30, 40, 50]: expected 30, got " << got3;
    stream->Close();
}

TEST_F(ReadOnlyStreamTest, ReadThrowsWhenClosed) {
    expectThrow<StreamNotOpenException>(
        [this]() { stream->Read(); },
        "Read() on closed stream"
    );
}

TEST_F(ReadOnlyStreamTest, IsEndOfStream) {
    stream->Open();
    EXPECT_FALSE(stream->IsEndOfStream()) << "IsEndOfStream() should be false before reading all";
    for (int i = 0; i < 5; ++i) {
        stream->Read();
    }
    EXPECT_TRUE(stream->IsEndOfStream()) << "IsEndOfStream() should be true after reading all";
    stream->Close();
}

TEST_F(ReadOnlyStreamTest, GetPosition) {
    stream->Open();
    EXPECT_EQ(stream->GetPosition(), 0) << "GetPosition() on [10, 20, 30, 40, 50] after open: expected 0, got " << stream->GetPosition();
    stream->Read();
    EXPECT_EQ(stream->GetPosition(), 1) << "GetPosition() on [10, 20, 30, 40, 50] after first read: expected 1, got " << stream->GetPosition();
    stream->Read();
    EXPECT_EQ(stream->GetPosition(), 2) << "GetPosition() on [10, 20, 30, 40, 50] after second read: expected 2, got " << stream->GetPosition();
    stream->Close();
}

TEST_F(ReadOnlyStreamTest, Seek) {
    stream->Open();
    size_t pos = stream->Seek(2);
    EXPECT_EQ(pos, 2) << "Seek(2): return value expected 2, got " << pos;
    EXPECT_EQ(stream->GetPosition(), 2) << "Position after Seek(2): expected 2, got " << stream->GetPosition();
    long long val = stream->Read();
    EXPECT_EQ(val, 30) << "Read() on [10, 20, 30, 40, 50] after Seek(2): expected 30, got " << val;
    stream->Close();
}

TEST_F(ReadOnlyStreamTest, SeekThrowsOutOfRange) {
    stream->Open();
    expectThrow<IndexOutOfRangeException>(
        [this]() { stream->Seek(100); },
        "Seek(100) on stream with 5 elements"
    );
    stream->Close();
}

TEST_F(ReadOnlyStreamTest, SeekThrowsWhenClosed) {
    expectThrow<StreamNotOpenException>(
        [this]() { stream->Seek(1); },
        "Seek() on closed stream"
    );
}

TEST_F(ReadOnlyStreamTest, Reset) {
    stream->Open();
    stream->Read();  
    stream->Read();  
    EXPECT_EQ(stream->GetPosition(), 2) << "Position before reset: expected 2, got " << stream->GetPosition();
    stream->Reset();
    EXPECT_EQ(stream->GetPosition(), 0) << "Position after reset: expected 0, got " << stream->GetPosition();
    long long val = stream->Read();
    EXPECT_EQ(val, 10) << "Read() on [10, 20, 30, 40, 50] after reset: expected 10, got " << val;
    stream->Close();
}

TEST_F(ReadOnlyStreamTest, Peek) {
    stream->Open();
    long long peeked = stream->Peek();
    EXPECT_EQ(peeked, 10) << "Peek(): expected 10, got " << peeked;
    EXPECT_EQ(stream->GetPosition(), 0) << "Position after Peek(): expected 0, got " << stream->GetPosition();
    long long read = stream->Read();
    EXPECT_EQ(read, 10) << "Read() on [10, 20, 30, 40, 50] after Peek(): expected 10, got " << read;
    stream->Close();
}

TEST_F(ReadOnlyStreamTest, PeekThrowsWhenClosed) {
    expectThrow<StreamNotOpenException>(
        [this]() { stream->Peek(); },
        "Peek() on closed stream"
    );
}

TEST_F(ReadOnlyStreamTest, PeekThrowsAtEnd) {
    stream->Open();
    for (int i = 0; i < 5; ++i) {
        stream->Read();
    }
    
    expectThrow<EndOfStreamException>(
        [this]() { stream->Peek(); },
        "Peek() at end of stream"
    );
    stream->Close();
}

TEST_F(ReadOnlyStreamTest, IsCanSeek) {
    EXPECT_TRUE(stream->IsCanSeek()) << "IsCanSeek() on ArraySequence stream: expected true";
}

TEST_F(ReadOnlyStreamTest, IsCanGoBack) {
    EXPECT_TRUE(stream->IsCanGoBack()) << "IsCanGoBack() on ArraySequence stream: expected true";
}