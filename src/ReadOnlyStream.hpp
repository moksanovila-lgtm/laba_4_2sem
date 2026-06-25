#pragma once

#include "..\third_party\Lab_2\library\include\exceptions.hpp"
#include "LazySequence.hpp"
#include <memory>
#include <functional>
#include <string>
#include "ISource.hpp"

template <typename T>
class ReadOnlyStream {
private:
    std::unique_ptr<ISource<T>> source;  
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