#pragma once

#include "ReadOnlyStream.hpp"
#include <sstream>
#include <algorithm>

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(Sequence<T>* seq)
    : sourceType(SourceType::SEQUENCE)
    , sequenceSource(std::make_unique<ArraySequence<T>>())
    , position(0)
    , isOpen(false)
    , canSeek(true)
    , canGoBack(true)
    , bufferStart(0)
{
    for (size_t i = 0; i < seq->GetCount(); ++i) {
        sequenceSource->Append(seq->Get(i));
    }
}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(LazySequence<T>* lazySeq)
    : sourceType(SourceType::LAZY_SEQUENCE)
    , lazySequenceSource(std::make_unique<LazySequence<T>>(*lazySeq))
    , position(0)
    , isOpen(false)
    , canSeek(true)
    , canGoBack(true)
    , bufferStart(0)
{
}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(const std::string& filename, std::function<T(const std::string&)> deserializer)
    : sourceType(SourceType::FILE)
    , deserializer(deserializer)
    , position(0)
    , isOpen(false)
    , canSeek(true)
    , canGoBack(true)
    , bufferStart(0)
{
    fileSource.open(filename);
    if (!fileSource.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(const std::string& data, std::function<T(const std::string&)> deserializer, char delimiter)
    : sourceType(SourceType::STRING)
    , stringSource(data)
    , stringPosition(0)
    , deserializer(deserializer)
    , position(0)
    , isOpen(false)
    , canSeek(true)
    , canGoBack(true)
    , bufferStart(0)
{
    (void)delimiter; 
}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(ReadOnlyStream<T>* stream)
    : sourceType(SourceType::STREAM)
    , streamSource(std::make_unique<ReadOnlyStream<T>>(std::move(*stream)))
    , position(0)
    , isOpen(false)
    , canSeek(stream->IsCanSeek())
    , canGoBack(stream->IsCanGoBack())
    , bufferStart(0)
{
}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(ReadOnlyStream&& other) noexcept
    : sourceType(other.sourceType)
    , sequenceSource(std::move(other.sequenceSource))
    , lazySequenceSource(std::move(other.lazySequenceSource))
    , stringSource(std::move(other.stringSource))
    , stringPosition(other.stringPosition)
    , streamSource(std::move(other.streamSource))
    , deserializer(std::move(other.deserializer))
    , position(other.position)
    , isOpen(other.isOpen)
    , canSeek(other.canSeek)
    , canGoBack(other.canGoBack)
    , bufferStart(other.bufferStart)
{
    buffer = std::move(other.buffer);
    
    if (other.fileSource.is_open()) {
        fileSource = std::move(other.fileSource);
    }
}

template <typename T>
ReadOnlyStream<T>& ReadOnlyStream<T>::operator=(ReadOnlyStream&& other) noexcept {
    if (this != &other) {
        Close();
        
        sourceType = other.sourceType;
        sequenceSource = std::move(other.sequenceSource);
        lazySequenceSource = std::move(other.lazySequenceSource);
        stringSource = std::move(other.stringSource);
        stringPosition = other.stringPosition;
        streamSource = std::move(other.streamSource);
        deserializer = std::move(other.deserializer);
        position = other.position;
        isOpen = other.isOpen;
        canSeek = other.canSeek;
        canGoBack = other.canGoBack;
        bufferStart = other.bufferStart;
    
        buffer = std::move(other.buffer);
        
        if (other.fileSource.is_open()) {
            fileSource = std::move(other.fileSource);
        }
    }
    return *this;
}

template <typename T>
ReadOnlyStream<T>::~ReadOnlyStream() {
    Close();
}

template <typename T>
T ReadOnlyStream<T>::ReadFromSequence() {
    if (position >= sequenceSource->GetCount()) {
        throw EndOfStreamException();
    }
    return sequenceSource->Get(position++);
}

template <typename T>
T ReadOnlyStream<T>::ReadFromLazySequence() {
    T value = lazySequenceSource->Get(position);
    position++;
    return value;
}

template <typename T>
T ReadOnlyStream<T>::ReadFromFile() {
    if (!fileSource.is_open()) {
        throw std::runtime_error("File not open");
    }
    
    std::string token;
    if (fileSource >> token) {
        position++;
        return deserializer(token);
    }
    
    throw EndOfStreamException();
}

template <typename T>
T ReadOnlyStream<T>::ReadFromString() {
    if (stringPosition >= stringSource.length()) {
        throw EndOfStreamException();
    }
    
    size_t end = stringSource.find(' ', stringPosition);
    if (end == std::string::npos) end = stringSource.length();
    
    std::string token = stringSource.substr(stringPosition, end - stringPosition);
    stringPosition = end + 1;
    position++;
    
    return deserializer(token);
}

template <typename T>
T ReadOnlyStream<T>::ReadFromStream() {
    if (!streamSource) {
        throw std::runtime_error("Stream source is null");
    }
    T value = streamSource->Read();
    position++;
    return value;
}

template <typename T>
void ReadOnlyStream<T>::FillBuffer(size_t targetPosition) {
    if (targetPosition >= bufferStart && targetPosition < bufferStart + buffer.GetCount()) {
        return;
    }
    
    buffer.Clear();
    bufferStart = targetPosition;
    
    size_t oldPosition = position;
    
    if (targetPosition < oldPosition && canGoBack) {
        if (sourceType == SourceType::SEQUENCE || sourceType == SourceType::LAZY_SEQUENCE) {
            position = 0;
            while (position < targetPosition) {
                Read();
            }
        }
    }
    try {
        while (buffer.GetCount() < 100 && !IsEndOfStream()) {
            buffer.Append(Read());  
        }
    } catch (const EndOfStreamException&) {
    }
    position = oldPosition;
}

template <typename T>
bool ReadOnlyStream<T>::IsEndOfStream() const {
    switch (sourceType) {
        case SourceType::SEQUENCE:
            return position >= sequenceSource->GetCount();
        case SourceType::LAZY_SEQUENCE:
            if (lazySequenceSource->IsInfinite()) return false;
            return position >= lazySequenceSource->GetCount();
        case SourceType::FILE:
            return fileSource.eof() || !fileSource.good();
        case SourceType::STRING:
            return stringPosition >= stringSource.length();
        case SourceType::STREAM:
            return streamSource && streamSource->IsEndOfStream();
        default:
            return true;
    }
}

template <typename T>
T ReadOnlyStream<T>::Read() {
    if (!isOpen) {
        throw std::runtime_error("Stream is not open");
    }
    if (IsEndOfStream()) {
        throw EndOfStreamException();
    }
    switch (sourceType) {
        case SourceType::SEQUENCE:
            return ReadFromSequence();
        case SourceType::LAZY_SEQUENCE:
            return ReadFromLazySequence();
        case SourceType::FILE:
            return ReadFromFile();
        case SourceType::STRING:
            return ReadFromString();
        case SourceType::STREAM:
            return ReadFromStream();
        default:
            throw std::runtime_error("Unknown source type");
    }
}

template <typename T>
size_t ReadOnlyStream<T>::Seek(size_t index) {
    if (!canSeek) {
        throw std::runtime_error("Seek not supported for this stream");
    }
    if (!isOpen) {
        throw std::runtime_error("Stream is not open");
    }
    switch (sourceType) {
        case SourceType::SEQUENCE:
            if (index <= sequenceSource->GetCount()) {
                position = index;
                return position;
            }
            break;
        case SourceType::LAZY_SEQUENCE:
            if (!lazySequenceSource->IsInfinite() && index <= lazySequenceSource->GetCount()) {
                position = index;
                return position;
            }
            if (lazySequenceSource->IsInfinite()) {
                position = index;
                return position;
            }
            break;
        case SourceType::FILE:
            if (fileSource.is_open()) {
                fileSource.clear();
                fileSource.seekg(0);
                position = 0;
                while (position < index && !IsEndOfStream()) {
                    try {
                        Read();
                    } catch (const EndOfStreamException&) {
                        break;
                    }
                }
                return position;
            }
            break;  
        case SourceType::STRING:
            if (index <= stringSource.length()) {
                stringPosition = 0;
                position = 0;
                for (size_t i = 0; i < index && stringPosition < stringSource.length(); ++i) {
                    size_t end = stringSource.find(' ', stringPosition);
                    if (end == std::string::npos) end = stringSource.length();
                    stringPosition = end + 1;
                    position++;
                }
                return position;
            }
            break;
        case SourceType::STREAM:
            if (streamSource && streamSource->IsCanSeek()) {
                return streamSource->Seek(index);
            }
            break;
    }
    throw IndexOutOfRangeException("Cannot seek to position " + std::to_string(index));
}

template <typename T>
void ReadOnlyStream<T>::Open() {
    if (isOpen) return;
    
    switch (sourceType) {
        case SourceType::FILE:
            if (!fileSource.is_open()) {
            }
            break;
        default:
            break;
    }
    isOpen = true;
    position = 0;
    buffer.Clear();
    bufferStart = 0;
}

template <typename T>
void ReadOnlyStream<T>::Close() {
    if (!isOpen) return;
    
    switch (sourceType) {
        case SourceType::FILE:
            if (fileSource.is_open()) {
                fileSource.close();
            }
            break;
        case SourceType::STREAM:
            if (streamSource) {
                streamSource->Close();
            }
            break;
        default:
            break;
    }
    isOpen = false;
}

// template <typename T>
// void ReadOnlyStream<T>::Reset() {
//     if (!canSeek) {
//         throw std::runtime_error("Cannot reset this stream");
//     }
//     Seek(0);
// }

template <typename T>
T ReadOnlyStream<T>::Peek() {
    if (!isOpen) {
        throw std::runtime_error("Stream is not open");
    }
    if (IsEndOfStream()) {
        throw EndOfStreamException();
    }
    size_t oldPosition = position;
    T value = Read();
    if (canGoBack) {
        Seek(oldPosition);
    } else {
        position = oldPosition;
    }
    return value;
}

// template <typename T>
// void ReadOnlyStream<T>::Reset() {
//     if (!canSeek) {
//         throw std::runtime_error("Cannot reset this stream");
//     }
//     buffer.Clear();
//     bufferStart = 0;
//     Seek(0);
// }

template <typename T>
void ReadOnlyStream<T>::Reset() {
    position = 0;
    buffer.Clear();
    bufferStart = 0;
}