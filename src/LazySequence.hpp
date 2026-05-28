#pragma once

#include "..\third_party\Lab_2\library\include\Sequence.hpp"
#include "..\third_party\Lab_2\library\include\ArraySequence.hpp"
#include "..\third_party\Lab_2\library\include\exceptions.hpp"
#include "..\third_party\Lab_2\library\include\Optional.hpp"
#include "Cardinal.hpp"
#include <memory>
#include <functional>

template <typename T>
class LazySequence : public Sequence<T> {
private:
    class IGenerator {
    public:
        virtual ~IGenerator() = default;
        virtual T GetNext() = 0;
        virtual bool HasNext() const = 0;
        virtual Optional<T> TryGetNext() = 0;
    };
    
    class DefaultGenerator : public IGenerator {
    private:
        LazySequence<T>* owner;
        std::function<T(Sequence<T>*)> rule;
        size_t arity;
        
    public:
        DefaultGenerator(LazySequence<T>* owner, std::function<T(Sequence<T>*)> rule, size_t arity);
        T GetNext() override;
        bool HasNext() const override;
        Optional<T> TryGetNext() override;
    };
    
    class SkipGenerator : public IGenerator {
    private:
        LazySequence<T>* owner;
        size_t startSkip;
        size_t endSkip;
        LazySequence<T>* parentSeq;
        size_t currentPosition;
        
    public:
        SkipGenerator(LazySequence<T>* owner, size_t startSkip, size_t endSkip, LazySequence<T>* parentSeq);
        T GetNext() override;
        bool HasNext() const override;
        Optional<T> TryGetNext() override;
    };
    
    class MapGenerator : public IGenerator {
    private:
        std::unique_ptr<IGenerator> source;
        std::function<T(const T&)> func;
        
    public:
        MapGenerator(std::unique_ptr<IGenerator> src, std::function<T(const T&)> f);
        T GetNext() override;
        bool HasNext() const override;
        Optional<T> TryGetNext() override;
    };
    
    class WhereGenerator : public IGenerator {
    private:
        std::unique_ptr<IGenerator> source;
        std::function<bool(const T&)> predicate;
        Optional<T> nextValue;
        void FindNext();
        
    public:
        WhereGenerator(std::unique_ptr<IGenerator> src, std::function<bool(const T&)> pred);
        T GetNext() override;
        bool HasNext() const override;
        Optional<T> TryGetNext() override;
    };

    class ConcatGenerator : public IGenerator {
    private:
        std::unique_ptr<IGenerator> firstGen;
        std::unique_ptr<Sequence<T>> secondSeq;
        size_t secondIndex;
        bool firstIsInfinite;
        bool firstFinished;
        
    public:
        ConcatGenerator(std::unique_ptr<IGenerator> first, Sequence<T>* second, bool firstInfinite);
        T GetNext() override;
        bool HasNext() const override;
        Optional<T> TryGetNext() override;
    };
    
private:
    std::unique_ptr<Sequence<T>> materialized;  
    std::unique_ptr<IGenerator> generator;      
    bool isFinite;
    size_t finiteSize;
    size_t materializedCount;
    
    void Materialize(size_t index);
    
public:
    LazySequence();
    LazySequence(const T* items, int count);
    LazySequence(Sequence<T>* seq);
    LazySequence(std::function<T(Sequence<T>*)> rule, Sequence<T>* firstElements, size_t arity);
    LazySequence(const LazySequence& other);
    
    T GetFirst() const override;
    T GetLast() const override;
    T Get(size_t index) const override;
    LazySequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const;
    size_t GetCount() const override;
    size_t GetMaterializedCount() const { return materializedCount; }
    Cardinal GetLength() const;
    
    LazySequence<T>* Append(const T& item) override;
    LazySequence<T>* Prepend(const T& item) override;
    LazySequence<T>* InsertAt(const T& item, size_t index) override;
    LazySequence<T>* Concat(LazySequence<T>* list) const;
    Sequence<T>* Concat(Sequence<T>* other) const override;
    IEnumerator<T>* GetEnumerator() const override;
    
    LazySequence<T>* Map(std::function<T(const T&)> func) const;
    T Reduce(std::function<T(const T&, const T&)> func, const T& initial) const override;
    LazySequence<T>* Where(std::function<bool(const T&)> predicate) const;
    
    T operator[](size_t index) const { return Get(index); }
    bool IsEmpty() const { return GetCount() == 0; }
    bool IsInfinite() const { return !isFinite; }
    
    ~LazySequence() = default;
};

#include "LazySequence.tpp"