#pragma once

#include "..\third_party\Lab_2\library\include\Sequence.hpp"
#include "..\third_party\Lab_2\library\include\exceptions.hpp"
#include "LazySequence.hpp"
#include <functional>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>

template <typename T>
class ReadOnlyStream {
private:
    class ISource {
    public:
        virtual ~ISource() = default;
        virtual bool IsEndOfStream() const = 0;
        virtual T Read() = 0;
        virtual size_t GetPosition() const = 0;
        virtual bool IsCanSeek() const = 0;
        virtual size_t Seek(size_t index) = 0;
        virtual bool IsCanGoBack() const = 0;
        virtual void Open() = 0;
        virtual void Close() = 0;
        virtual T Peek() = 0;
        virtual void Reset() = 0;
    };
    
    class SequenceSource;
    class LazySequenceSource;
    class FileSource;
    class StringSource;
    class StreamSource;
    
    std::unique_ptr<ISource> source;  
    bool isOpen;
    
public:
    ReadOnlyStream(Sequence<T>* seq);
    ReadOnlyStream(LazySequence<T>* lazySeq);
    ReadOnlyStream(const std::string& filename, std::function<T(const std::string&)> deserializer);
    ReadOnlyStream(char delimiter, const std::string& data, std::function<T(const std::string&)> deserializer);
    ReadOnlyStream(ReadOnlyStream<T>* stream);
    ReadOnlyStream(ReadOnlyStream&& other) noexcept;
    ReadOnlyStream(const ReadOnlyStream&) = delete;
    
    ReadOnlyStream& operator=(ReadOnlyStream&& other) noexcept;
    ReadOnlyStream& operator=(const ReadOnlyStream&) = delete;
    
    bool IsEndOfStream() const;
    T Read();
    size_t GetPosition() const;
    bool IsCanSeek() const;
    size_t Seek(size_t index);
    bool IsCanGoBack() const;
    void Open();
    void Close();
    T Peek();
    void Reset();
    
    ~ReadOnlyStream() = default;
};

#include "ReadOnlyStream.tpp"