// #pragma once

// #include <QObject>
// #include <memory>
// #include <functional>
// #include "LazySequence.hpp"
// #include "ReadOnlyStream.hpp"

// class LazySequenceController : public QObject {
//     Q_OBJECT

// public:
//     enum GeneratorType {
//         GEN_ARITHMETIC = 0,
//         GEN_RANDOM = 1,
//         GEN_FIBONACCI = 2,
//         GEN_CONSTANT = 3
//     };
    
//     explicit LazySequenceController(QObject* parent = nullptr);
//     ~LazySequenceController();
    
//     void setGenerator(GeneratorType type, int p1 = 0, int p2 = 0);
//     void generateFirstNElements(int n);
    
//     int getElement(size_t index);
//     size_t getMaterializedCount() const;
//     bool isInfinite() const;
//     QString getGeneratorDescription() const;
    
//     ReadOnlyStream<int>* getStream() const { return stream.get(); }
    
// signals:
//     void sequenceChanged();
//     void materializedCountChanged(size_t count);
//     void error(const QString& msg);
    
// private:
//     std::unique_ptr<LazySequence<int>> lazySeq;
//     std::unique_ptr<ReadOnlyStream<int>> stream;
//     GeneratorType currentType;
//     int param1;
//     int param2;
    
//     void rebuildSequence();
// };

#pragma once

#include <QObject>
#include <memory>
#include <functional>
#include "LazySequence.hpp"
#include "ReadOnlyStream.hpp"

class LazySequenceController : public QObject {
    Q_OBJECT

public:
    enum GeneratorType {
        GEN_ARITHMETIC = 0,
        GEN_RANDOM = 1,
        GEN_FIBONACCI = 2,
        GEN_CONSTANT = 3
    };
    
    explicit LazySequenceController(QObject* parent = nullptr);
    ~LazySequenceController();
    
    void setGenerator(GeneratorType type, long long p1 = 0, long long p2 = 0);
    void generateFirstNElements(int n);
    
    long long getElement(size_t index);
    size_t getMaterializedCount() const;
    bool isInfinite() const;
    QString getGeneratorDescription() const;
    
    ReadOnlyStream<long long>* getStream() const { return stream.get(); }
    
signals:
    void sequenceChanged();
    void materializedCountChanged(size_t count);
    void error(const QString& msg);
    
private:
    std::unique_ptr<LazySequence<long long>> lazySeq;
    std::unique_ptr<ReadOnlyStream<long long>> stream;
    GeneratorType currentType;
    long long param1;
    long long param2;
    
    void rebuildSequence();
};