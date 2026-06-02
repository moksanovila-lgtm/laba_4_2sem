#include "ReadOnlyStream.hpp"

template <typename T>
class ReadOnlyStream<T>::SequenceSource : public ReadOnlyStream<T>::ISource {
private:
    std::unique_ptr<Sequence<T>> data;
    size_t position;
    bool isOpen;
    
public:
    explicit SequenceSource(Sequence<T>* seq)
        : data(std::make_unique<ArraySequence<T>>())
        , position(0)
        , isOpen(false) {
        IEnumerator<T>* enumerator = seq->GetEnumerator();
        while (enumerator->MoveNext()) {
        data->Append(enumerator->Current());
        }
        delete enumerator;
        }
    
    bool IsEndOfStream() const override {
        return position >= data->GetCount();
    }
    
    T Read() override {
        if (!isOpen) throw StreamNotOpenException("Stream not open");
        if (IsEndOfStream()) throw EndOfStreamException("End of sequence");
        return data->Get(position++);
    }
    
    size_t GetPosition() const override { return position; }
    bool IsCanSeek() const override { return true; }
    bool IsCanGoBack() const override { return true; }
    
    size_t Seek(size_t index) override {
    if (!isOpen) throw StreamNotOpenException("Stream not open");
    if (index > data->GetCount()) {
        throw IndexOutOfRangeException("Seek position " + std::to_string(index) + 
        " exceeds stream size " + std::to_string(data->GetCount())
        );
    }
    position = index;
    return position;
    }
    
    void Open() override { 
        isOpen = true;  
        position = 0; 
    }
    
    void Close() override { 
        isOpen = false;  
    }

    T Peek() override {
    if (!isOpen) throw StreamNotOpenException("Stream not open");
    if (IsEndOfStream()) throw EndOfStreamException("Peek at end of stream");
    return data->Get(position);
    }
    void Reset() override { position = 0; }
};

template <typename T>
class ReadOnlyStream<T>::LazySequenceSource : public ReadOnlyStream<T>::ISource {
private:
    std::unique_ptr<LazySequence<T>> data;
    size_t position;
    bool isOpen;
    
public:
    explicit LazySequenceSource(LazySequence<T>* lazySeq)
        : data(std::make_unique<LazySequence<T>>(*lazySeq))
        , position(0)
        , isOpen(false) {}
    
    bool IsEndOfStream() const override {
        if (data->IsInfinite()) return false;
        return position >= data->GetCount();
    }
    
    T Read() override {
        if (!isOpen) throw StreamNotOpenException("Stream not open");
        if (IsEndOfStream()) throw EndOfStreamException("End of lazy sequence");
        return data->Get(position++);
    }
    
    size_t GetPosition() const override { return position; }
    bool IsCanSeek() const override { return true; }
    bool IsCanGoBack() const override { return true; }

    size_t Seek(size_t index) override {
    if (!isOpen) throw StreamNotOpenException("Stream not open");
    if (!data->IsInfinite() && index > data->GetCount()) {
        throw IndexOutOfRangeException("Seek position " + std::to_string(index) + 
        " exceeds stream size"
        );
    }
    position = index;
    return position;
}
    
    void Open() override { 
        isOpen = true; 
        position = 0; 
    }
    
    void Close() override { 
        isOpen = false; 
    }
    T Peek() override { 
    if (!isOpen) throw StreamNotOpenException("Stream not open");      
    if (IsEndOfStream()) throw EndOfStreamException("Peek at end of stream");  
    return data->Get(position);
    }
    void Reset() override { position = 0; }
};

template <typename T>
class ReadOnlyStream<T>::FileSource : public ReadOnlyStream<T>::ISource {
private:
    std::ifstream file;
    std::function<T(const std::string&)> deserializer;
    size_t position;
    std::string filename;
    bool isOpen;
    
public:
    FileSource(const std::string& fname, std::function<T(const std::string&)> deserializer)
        : deserializer(deserializer)
        , position(0)
        , filename(fname)
        , isOpen(false) {}
    
    bool IsEndOfStream() const override {
        if (!isOpen) return true;
        return file.eof() || !file.good();
    }
    
    T Read() override {
        if (!isOpen) throw StreamNotOpenException("File not open");
        if (IsEndOfStream()) throw EndOfStreamException("End of file");
        
        std::string token;
        if (file >> token) {
            position++;
            return deserializer(token);
        }
        throw EndOfStreamException("End of file");
    }
    
    size_t GetPosition() const override { return position; }
    bool IsCanSeek() const override { return true; }
    bool IsCanGoBack() const override { return true; }

    size_t Seek(size_t index) override {
    if (!isOpen) throw StreamNotOpenException("File not open");
    
    file.clear();
    file.seekg(0);
    position = 0;
    
    while (position < index && !IsEndOfStream()) {
        std::string token;
        if (file >> token) {
            position++;
        } else {
            break;  
        }
    }
    
    return position;
    }
    
    void Open() override {
        if (!isOpen) {
            file.open(filename);
            if (!file.is_open()) {
                throw InvalidArgumentException("Cannot open file: " + filename);
            }
            isOpen = true;
            position = 0;
        }
    }
    
    void Close() override {
        if (isOpen && file.is_open()) {
            file.close();
            isOpen = false;
        }
    }
    
    T Peek() override {
        if (!isOpen) throw StreamNotOpenException("File not open");
        if (IsEndOfStream()) throw EndOfStreamException("End of file");
        
        size_t oldPos = position;
        std::streampos filePos = file.tellg();
        
        std::string token;
        if (file >> token) {
            file.seekg(filePos);
            return deserializer(token);
        }
        throw EndOfStreamException("End of file");
    }
    
    void Reset() override { Seek(0); }
};

template <typename T>
class ReadOnlyStream<T>::StringSource : public ReadOnlyStream<T>::ISource {
private:
    std::string data;
    char delimiter;
    std::function<T(const std::string&)> deserializer;
    size_t stringPosition;
    size_t position;
    bool isOpen; 
    
public:
    StringSource(char delim, const std::string& str, 
                 std::function<T(const std::string&)> deserializer)
        : data(str)
        , delimiter(delim)
        , deserializer(deserializer)
        , stringPosition(0)
        , position(0) 
        , isOpen(false) {}
    
    bool IsEndOfStream() const override {
        return stringPosition >= data.length();
    }
    
    T Read() override {
        if (!isOpen) throw StreamNotOpenException("Stream not open");  
        if (IsEndOfStream()) throw EndOfStreamException("End of string");
    
        size_t end = data.find(delimiter, stringPosition);
        if (end == std::string::npos) end = data.length();
        
        std::string token = data.substr(stringPosition, end - stringPosition);
        stringPosition = end + 1;
        position++;
        
        return deserializer(token);
    }
    
    size_t GetPosition() const override { return position; }
    bool IsCanSeek() const override { return true; }
    bool IsCanGoBack() const override { return true; }
    
    size_t Seek(size_t index) override {
        if (!isOpen) throw StreamNotOpenException("Stream not open");
        stringPosition = 0;
        position = 0;
        
        for (size_t i = 0; i < index && stringPosition < data.length(); ++i) {
            size_t end = data.find(delimiter, stringPosition);
            if (end == std::string::npos) end = data.length();
            stringPosition = end + 1;
            position++;
        }
        return position;
    }
    
    void Open() override { 
        isOpen = true; 
        position = 0; 
        stringPosition = 0; 
    }

    void Close() override {isOpen = false;}
    
    T Peek() override {
        if (!isOpen) throw StreamNotOpenException("Stream not open");
        if (IsEndOfStream()) throw EndOfStreamException("End of string");
        
        size_t oldStringPos = stringPosition;
        size_t oldPos = position;
        
        size_t end = data.find(delimiter, stringPosition);
        if (end == std::string::npos) end = data.length();
        std::string token = data.substr(stringPosition, end - stringPosition);
        
        stringPosition = oldStringPos;
        position = oldPos;
        
        return deserializer(token);
    }
    
    void Reset() override { Seek(0); }
};

template <typename T>
class ReadOnlyStream<T>::StreamSource : public ReadOnlyStream<T>::ISource {
private:
    std::unique_ptr<ReadOnlyStream<T>> source;
    
public:
    explicit StreamSource(ReadOnlyStream<T>* stream)
        : source(std::make_unique<ReadOnlyStream<T>>(std::move(*stream))) {}
    
    bool IsEndOfStream() const override {
        return source->IsEndOfStream();
    }
    
    T Read() override {
        return source->Read();
    }
    
    size_t GetPosition() const override {
        return source->GetPosition();
    }
    
    bool IsCanSeek() const override {
        return source->IsCanSeek();
    }
    
    size_t Seek(size_t index) override {
        return source->Seek(index);
    }
    
    bool IsCanGoBack() const override {
        return source->IsCanGoBack();
    }
    
    void Open() override {
        source->Open();
    }
    
    void Close() override {
        source->Close();
    }
    
    T Peek() override {
        return source->Peek();
    }
    
    void Reset() override {
        source->Reset();
    }
};

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(Sequence<T>* seq)
    : source(std::make_unique<SequenceSource>(seq))
    , isOpen(false) {}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(LazySequence<T>* lazySeq)
    : source(std::make_unique<LazySequenceSource>(lazySeq))
    , isOpen(false) {}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(const std::string& filename, 
                                   std::function<T(const std::string&)> deserializer)
    : source(std::make_unique<FileSource>(filename, deserializer))
    , isOpen(false) {}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(char delimiter, const std::string& data,
                                   std::function<T(const std::string&)> deserializer)
    : source(std::make_unique<StringSource>(delimiter, data, deserializer))
    , isOpen(false) {}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(ReadOnlyStream<T>* stream)
    : source(std::make_unique<StreamSource>(stream))
    , isOpen(false) {}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(ReadOnlyStream&& other) noexcept
    : source(std::move(other.source))
    , isOpen(other.isOpen) {}

template <typename T>
ReadOnlyStream<T>& ReadOnlyStream<T>::operator=(ReadOnlyStream&& other) noexcept {
    if (this != &other) {  
        source = std::move(other.source);  
        isOpen = other.isOpen;              
    }
    return *this;
}

template <typename T>
bool ReadOnlyStream<T>::IsEndOfStream() const {
    return source->IsEndOfStream();
}

template <typename T>
T ReadOnlyStream<T>::Read() {
    if (!isOpen) throw StreamNotOpenException("Stream not open");
    return source->Read();
}

template <typename T>
size_t ReadOnlyStream<T>::GetPosition() const {
    return source->GetPosition();
}

template <typename T>
bool ReadOnlyStream<T>::IsCanSeek() const {
    return source->IsCanSeek();
}

template <typename T>
size_t ReadOnlyStream<T>::Seek(size_t index) {
    if (!isOpen) throw StreamNotOpenException("Stream not open");
    return source->Seek(index);
}

template <typename T>
bool ReadOnlyStream<T>::IsCanGoBack() const {
    return source->IsCanGoBack();
}

template <typename T>
void ReadOnlyStream<T>::Open() {
    isOpen = true;
    source->Open();
}

template <typename T>
void ReadOnlyStream<T>::Close() {
    isOpen = false;
    source->Close();
}

template <typename T>
T ReadOnlyStream<T>::Peek() {
    if (!isOpen) throw StreamNotOpenException("Stream not open");
    return source->Peek();
}

template <typename T>
void ReadOnlyStream<T>::Reset() {
    source->Reset();
}