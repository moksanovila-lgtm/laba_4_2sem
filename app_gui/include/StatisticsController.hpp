// #pragma once

// #include <QObject>
// #include "OnlineStatistics.hpp"

// class StatisticsController : public QObject {
//     Q_OBJECT

// public:
//     explicit StatisticsController(QObject* parent = nullptr);
//     ~StatisticsController();
    
//     void updateFromValue(int value);
//     void reset();
    
//     int getMin() const;
//     int getMax() const;
//     double getAverage() const;
//     double getVariance() const;
//     double getStdDeviation() const;
//     double getMedian();
//     size_t getCount() const;
    
//     QString getStatsAsString() const;
    
// signals:
//     void statsUpdated();
//     void processingProgress(int percent);
//     void error(const QString& msg);
    
// private:
//     OnlineStatistics<int> stats;
//     bool isProcessing;
// };

#pragma once

#include <QObject>
#include "OnlineStatistics.hpp"

class StatisticsController : public QObject {
    Q_OBJECT

private:
    OnlineStatistics<long long> stats;
    
public:
    explicit StatisticsController(QObject* parent = nullptr);
    ~StatisticsController();
    
    void updateFromValue(long long value);
    void reset();
    
    long long getMin() const;
    long long getMax() const;
    double getRange() const;          
    double getAverage() const;
    double getVariance() const;
    double getStdDeviation() const;
    double getRMS() const;            
    double getCoefficientOfVariation() const;  
    size_t getCount() const;
    
signals:
    void statsUpdated();
    void processingProgress(int percent);
    void error(const QString& msg);
};