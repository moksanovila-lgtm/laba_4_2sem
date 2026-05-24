#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QProgressBar>
#include "ReadOnlyStream.hpp"
#include "StatisticsController.hpp"

class StatisticsWidget : public QWidget {
    Q_OBJECT

private:
    StatisticsController* controller;
    ReadOnlyStream<int>* currentStream;
    
    QSpinBox* countSpin;
    QPushButton* collectBtn;
    QPushButton* resetBtn;
    QProgressBar* progressBar;
    
    QLabel* minLabel;
    QLabel* maxLabel;
    QLabel* avgLabel;
    QLabel* medianLabel;
    QLabel* varianceLabel;
    QLabel* stdDevLabel;
    QLabel* countLabel;
    
    void setupUI();
    void updateDisplay();

private slots:
    void onCollect();
    void onReset();
    void onStatsUpdated();
    void onProgress(int percent);
    void onError(const QString& msg);

public:
    explicit StatisticsWidget(QWidget* parent = nullptr);
    ~StatisticsWidget();
    
    void setStream(ReadOnlyStream<int>* stream);
};