#pragma once

template <typename T>
class ISource {
public:
    virtual ~ISource() = default;
    
    virtual bool IsEndOfStream() const = 0;
    virtual T Read() = 0;
    virtual size_t GetPosition() const = 0;
    virtual bool IsCanSeek() const = 0;
    virtual size_t Seek(size_t index) = 0;
    virtual bool IsCanGoBack() const = 0;
    virtual T Peek() = 0;
    virtual void Reset() = 0;
};