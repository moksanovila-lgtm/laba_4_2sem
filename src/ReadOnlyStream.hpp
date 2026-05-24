#pragma once

#include "..\third_party\Lab_2\library\include\Sequence.hpp"
#include "..\third_party\Lab_2\library\include\ArraySequence.hpp"
#include "..\third_party\Lab_2\library\include\DynamicArray.hpp"
#include "..\third_party\Lab_2\library\include\exceptions.hpp"
#include "LazySequence.hpp"
#include "Optional.hpp"
#include <string>
#include <functional>
#include <fstream>
#include <memory>

template <typename T>
class ReadOnlyStream {
private:
    enum class SourceType {
        SEQUENCE,      
        LAZY_SEQUENCE, 
        FILE,         
        STRING,       
        STREAM       
    };
    
    SourceType sourceType;
    
    std::unique_ptr<Sequence<T>> sequenceSource;
    std::unique_ptr<LazySequence<T>> lazySequenceSource;
    std::ifstream fileSource;
    std::string stringSource;
    size_t stringPosition;
    std::unique_ptr<ReadOnlyStream<T>> streamSource;
    
    std::function<T(const std::string&)> deserializer;
    

    size_t position;         
    bool canSeek;            
    bool canGoBack;          
    
    DynamicArray<T> buffer;  
    size_t bufferStart;       
    
    T ReadFromSequence();
    T ReadFromLazySequence();
    T ReadFromFile();
    T ReadFromString();
    T ReadFromStream();
    
    void FillBuffer(size_t targetPosition);
    
public:
    ReadOnlyStream(Sequence<T>* seq);
    ReadOnlyStream(LazySequence<T>* lazySeq);
    ReadOnlyStream(const std::string& filename, std::function<T(const std::string&)> deserializer);
    ReadOnlyStream(const std::string& data, std::function<T(const std::string&)> deserializer, char delimiter = ' ');
    ReadOnlyStream(ReadOnlyStream<T>* stream);

    ReadOnlyStream(const ReadOnlyStream&) = delete;
    ReadOnlyStream& operator=(const ReadOnlyStream&) = delete;

    ReadOnlyStream(ReadOnlyStream&& other) noexcept;
    ReadOnlyStream& operator=(ReadOnlyStream&& other) noexcept;
    
    ~ReadOnlyStream();
    
    bool IsEndOfStream() const;
    T Read();
    size_t GetPosition() const { return position; }
    bool IsCanSeek() const { return canSeek; }
    size_t Seek(size_t index);
    bool IsCanGoBack() const { return canGoBack; }
    
    void Open();
    void Close();
    void Reset();
    T Peek();
};

#include "ReadOnlyStream.tpp"