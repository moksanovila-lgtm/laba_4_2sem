#include "ReadOnlyStream.hpp"

template <typename T>
class SequenceSource : public ISource<T>{
private:
    std::unique_ptr<Sequence<T>> data;
    size_t position;
    
public:
    SequenceSource(Sequence<T>* seq)
        : data(std::make_unique<ArraySequence<T>>())
        , position(0) {
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
        if (IsEndOfStream()) throw EndOfStreamException("End of sequence");
        return data->Get(position++);
    }
    
    size_t GetPosition() const override { return position; }
    bool IsCanSeek() const override { return true; }
    bool IsCanGoBack() const override { return true; }
    
    size_t Seek(size_t index) override {
    if (index > data->GetCount()) {
        throw IndexOutOfRangeException("Seek position " + std::to_string(index) + 
        " exceeds stream size " + std::to_string(data->GetCount())
        );
    }
    position = index;
    return position;
    }

    T Peek() override {
    if (IsEndOfStream()) throw EndOfStreamException("Peek at end of stream");
    return data->Get(position);
    }
    void Reset() override { position = 0; }
};

template <typename T>
class LazySequenceSource : public ISource<T> {
private:
    std::unique_ptr<LazySequence<T>> data;
    size_t position;
    
public:
    LazySequenceSource(LazySequence<T>* lazySeq)
        : data(std::make_unique<LazySequence<T>>(*lazySeq))
        , position(0) {}
    
    bool IsEndOfStream() const override {
        if (data->IsInfinite()) return false;
        return position >= data->GetCount();
    }
    
    T Read() override {
        if (IsEndOfStream()) throw EndOfStreamException("End of lazy sequence");
        return data->Get(position++);
    }
    
    size_t GetPosition() const override { return position; }
    bool IsCanSeek() const override { return true; }
    bool IsCanGoBack() const override { return true; }

    size_t Seek(size_t index) override {
    if (!data->IsInfinite() && index > data->GetCount()) {
        throw IndexOutOfRangeException("Seek position " + std::to_string(index) + 
        " exceeds stream size"
        );
    }
    position = index;
    return position;
}
    
    T Peek() override {      
    if (IsEndOfStream()) throw EndOfStreamException("Peek at end of stream");  
    return data->Get(position);
    }
    void Reset() override { position = 0; }
};

template <typename T>
class FileSource : public ISource<T> {
private:
    static constexpr size_t BUFFER_SIZE = 1000;

    FILE* file;
    std::function<T(const std::string&)> deserializer;
    size_t position;
    std::string filename;

public:
    FileSource(const std::string& fname, std::function<T(const std::string&)> deserializer)
        : file(nullptr)
        , deserializer(deserializer)
        , position(0)
        , filename(fname) {
        file = fopen(filename.c_str(), "r");
        if (file == nullptr) {
            throw InvalidArgumentException("Cannot open file: " + filename);
        }
    }
    
    ~FileSource() {
        if (file != nullptr) {
            fclose(file);
            file = nullptr;
        }
    }

bool IsEndOfStream() const override {
        if (file == nullptr) return true;
        return feof(file) != 0 || ferror(file) != 0;
    }
    
    T Read() override {
        if (file == nullptr) throw StreamNotOpenException("File not open");
        if (IsEndOfStream()) throw EndOfStreamException("End of file");
        
        char buffer[BUFFER_SIZE];
        if (fgets(buffer, sizeof(buffer), file) != nullptr) {
            std::string token(buffer);
            if (!token.empty() && token.back() == '\n') {
                token.pop_back();
            }
            position++;
            return deserializer(token);
        }
        throw EndOfStreamException("End of file");
    }
    
    size_t GetPosition() const override { return position; }
    bool IsCanSeek() const override { return true; }
    bool IsCanGoBack() const override { return true; }

    size_t Seek(size_t index) override {
        if (file == nullptr) throw StreamNotOpenException("File not open");
        rewind(file);
        position = 0;
        char buffer[BUFFER_SIZE];
        while (position < index && fgets(buffer, sizeof(buffer), file) != nullptr) {
            position++;
        }
        return position;
    }
    
    T Peek() override {
        if (file == nullptr) throw StreamNotOpenException("File not open");
        if (IsEndOfStream()) throw EndOfStreamException("End of file");
        long pos = ftell(file);
        char buffer[BUFFER_SIZE];
        if (fgets(buffer, sizeof(buffer), file) != nullptr) {
            fseek(file, pos, SEEK_SET);
            
            std::string token(buffer);
            if (!token.empty() && token.back() == '\n') {
                token.pop_back();
            }
            return deserializer(token);
        }
        throw EndOfStreamException("End of file");
    }
    
    void Reset() override { 
        Seek(0); 
    }
};

template <typename T>
class StringSource : public ISource<T> {
private:
    std::string data;
    char delimiter;
    std::function<T(const std::string&)> deserializer;
    size_t stringPosition;
    size_t position;
    
public:
    StringSource(char delim, const std::string& str, 
                 std::function<T(const std::string&)> deserializer)
        : data(str)
        , delimiter(delim)
        , deserializer(deserializer)
        , stringPosition(0)
        , position(0) {}
    
    bool IsEndOfStream() const override {
        return stringPosition >= data.length();
    }
    
    T Read() override {
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
    
    T Peek() override {
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
class StreamSource : public ISource<T> {
private:
    std::unique_ptr<ReadOnlyStream<T>> source;
    
public:
    StreamSource(ReadOnlyStream<T>* stream)
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
    
    T Peek() override {
        return source->Peek();
    }
    
    void Reset() override {
        source->Reset();
    }
};

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(Sequence<T>* seq)
    : source(std::make_unique<SequenceSource<T>>(seq)) {}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(LazySequence<T>* lazySeq)
    : source(std::make_unique<LazySequenceSource<T>>(lazySeq)) {}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(const std::string& filename, 
                                   std::function<T(const std::string&)> deserializer)
    : source(std::make_unique<FileSource<T>>(filename, deserializer)) {}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(char delimiter, const std::string& data,
                                   std::function<T(const std::string&)> deserializer)
    : source(std::make_unique<StringSource<T>>(delimiter, data, deserializer)) {}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(ReadOnlyStream<T>* stream)
    : source(std::make_unique<StreamSource<T>>(stream)) {}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(ReadOnlyStream&& other) noexcept
    : source(std::move(other.source)) {}

template <typename T>
ReadOnlyStream<T>& ReadOnlyStream<T>::operator=(ReadOnlyStream&& other) noexcept {
    if (this != &other) {  
        source = std::move(other.source);              
    }
    return *this;
}

template <typename T>
bool ReadOnlyStream<T>::IsEndOfStream() const {
    return source->IsEndOfStream();
}

template <typename T>
T ReadOnlyStream<T>::Read() {
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
    return source->Seek(index);
}

template <typename T>
bool ReadOnlyStream<T>::IsCanGoBack() const {
    return source->IsCanGoBack();
}

template <typename T>
T ReadOnlyStream<T>::Peek() {
    return source->Peek();
}

template <typename T>
void ReadOnlyStream<T>::Reset() {
    source->Reset();
}