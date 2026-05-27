#pragma once

#include <QWidget>
#include <QLabel>
#include "ReadOnlyStream.hpp"

class StatisticsWidget : public QWidget {
    Q_OBJECT

private:
    ReadOnlyStream<long long>* sourceStream;
    int elementCount;
    
    QLabel* minLabel;
    QLabel* maxLabel;
    QLabel* rangeLabel;
    QLabel* countLabel;
    QLabel* avgLabel;
    QLabel* varianceLabel;
    QLabel* stdDevLabel;
    QLabel* rmsLabel;
    QLabel* cvLabel;
    
    long long minVal;
    long long maxVal;
    double range;
    double average;
    double variance;
    double stdDeviation;
    double rms;
    double cv;
    size_t count;
    
    void setupUI();
    void collectStatistics();
    void updateDisplay();

public:
    explicit StatisticsWidget(ReadOnlyStream<long long>* stream, int count, 
                              const QString& title, QWidget* parent = nullptr);
    ~StatisticsWidget();
};