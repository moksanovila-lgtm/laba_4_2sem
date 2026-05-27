#include "LazySequenceController.hpp"
#include <random>
#include <chrono>
#include <iostream>

LazySequenceController::LazySequenceController(QObject* parent)
    : QObject(parent)
    , currentType(GEN_ARITHMETIC)
    , param1(1)
    , param2(1) {
    rebuildSequence();
}

LazySequenceController::~LazySequenceController() {}

void LazySequenceController::setGenerator(GeneratorType type, long long p1, long long p2) {
    currentType = type;
    param1 = p1;
    param2 = p2;
    rebuildSequence();
    emit sequenceChanged();
}

void LazySequenceController::rebuildSequence() {
    ArraySequence<long long> empty;

    switch (currentType) {
        case GEN_ARITHMETIC: {
            struct ArithmeticFunctor {
                long long start;
                long long step;
                mutable size_t counter = 0;
                
                long long operator()(Sequence<long long>* prev) const {
                    (void)prev;
                    long long result = start + static_cast<long long>(counter) * step;
                    counter++;
                    return result;
                }
            };
            
            ArithmeticFunctor func{param1, param2, 0};
            auto lambda = [func](Sequence<long long>* prev) mutable -> long long {
                return func(prev);
            };
            
            lazySeq = std::make_unique<LazySequence<long long>>(lambda, &empty, 0);
            break;
        }
        case GEN_RANDOM: {
            struct RandomFunctor {
                long long minVal;
                long long maxVal;
                mutable std::mt19937_64 gen;
                mutable std::uniform_int_distribution<long long> dis;
                
                RandomFunctor(long long minV, long long maxV) 
                    : minVal(minV), maxVal(maxV)
                    , gen(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()))
                    , dis(minV, maxV) {}
                
                long long operator()(Sequence<long long>* prev) const {
                    (void)prev;
                    return dis(gen);
                }
            };
            
            RandomFunctor func(param1, param2);
            auto lambda = [func](Sequence<long long>* prev) mutable -> long long {
                return func(prev);
            };
            
            lazySeq = std::make_unique<LazySequence<long long>>(lambda, &empty, 0);
            break;
        }
        case GEN_FIBONACCI: {
            struct FibonacciFunctor {
                mutable unsigned long long a = 0;
                mutable unsigned long long b = 1;
                
                long long operator()(Sequence<long long>* prev) const {
                    (void)prev;
                    unsigned long long next = a + b;
                    a = b;
                    b = next;
                    return static_cast<long long>(next);
                }
            };
            
            FibonacciFunctor func;
            auto lambda = [func](Sequence<long long>* prev) mutable -> long long {
                return func(prev);
            };
            ArraySequence<long long> fibStart = {0, 1};
            lazySeq = std::make_unique<LazySequence<long long>>(lambda, &fibStart, 2);
            break;
        }

        case GEN_CONSTANT: {
            long long value = param1;
            auto lambda = [value](Sequence<long long>* prev) -> long long {
                (void)prev;
                return value;
            };
            lazySeq = std::make_unique<LazySequence<long long>>(lambda, &empty, 0);
            break;
        }
    }
    stream = std::make_unique<ReadOnlyStream<long long>>(lazySeq.get());
}

void LazySequenceController::generateFirstNElements(int n) {
    for (int i = 0; i < n; ++i) {
        try {
            lazySeq->Get(i);
        } catch (const std::overflow_error& e) {
            emit error(QString("Переполнение: %1").arg(e.what()));
            emit materializedCountChanged(lazySeq->GetMaterializedCount());
            return;
        }
    }
    emit materializedCountChanged(lazySeq->GetMaterializedCount());
}

long long LazySequenceController::getElement(size_t index) {
    try {
        return lazySeq->Get(index);
    } catch (const std::exception& e) {
        emit error(e.what());
        return 0;
    }
}

size_t LazySequenceController::getMaterializedCount() const {
    return lazySeq ? lazySeq->GetMaterializedCount() : 0;
}

bool LazySequenceController::isInfinite() const {
    return lazySeq ? lazySeq->IsInfinite() : true;
}

QString LazySequenceController::getGeneratorDescription() const {
    switch (currentType) {
        case GEN_ARITHMETIC:
            return QString("Арифметическая: %1, %2, %3, ...")
                .arg(param1).arg(param1 + param2).arg(param1 + 2 * param2);
        case GEN_RANDOM:
            return QString("Случайные [%1..%2]").arg(param1).arg(param2);
        case GEN_FIBONACCI:
            return "Фибоначчи: 0, 1, 1, 2, 3, 5, 8, ...";
        case GEN_CONSTANT:
            return QString("Постоянные: %1, %1, %1, ...").arg(param1);
        default:
            return "Неизвестный генератор";
    }
}