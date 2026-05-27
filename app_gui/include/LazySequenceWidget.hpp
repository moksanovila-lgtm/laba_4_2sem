#pragma once

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QTabWidget>
#include <QRadioButton>
#include <QButtonGroup>
#include "LazySequenceController.hpp"

class StatisticsWidget;

class LazySequenceWidget : public QWidget {
    Q_OBJECT

private:
    LazySequenceController* controller;
    QTabWidget* parentTabWidget;
    
    QComboBox* generatorCombo;
    QSpinBox* param1Spin;
    QSpinBox* param2Spin;
    QPushButton* applyGenBtn;
    
    QSpinBox* generateCountSpin;
    QPushButton* generateBtn;
    
    QRadioButton* firstRadio;
    QSpinBox* firstCountSpin;
    QRadioButton* rangeRadio;
    QSpinBox* rangeFromSpin;
    QSpinBox* rangeToSpin;
    QRadioButton* lastRadio;
    QSpinBox* lastCountSpin;
    QPushButton* collectStatsBtn;
    
    QLabel* infoLabel;
    QListWidget* elementsList;
    
    int statsTabCounter;
    
    void setupUI();
    void updateDisplay();
    void collectStatisticsForRange(int start, int end);
    void collectStatisticsForFirst(int count);
    void collectStatisticsForLast(int count);

private slots:
    void onApplyGenerator();
    void onGenerateElements();
    void onCollectStatistics();
    void onSequenceChanged();
    void onMaterializedCountChanged(size_t count);
    void onError(const QString& msg);
    void onGeneratorTypeChanged(int index);
    void onStatsModeChanged();

public:
    explicit LazySequenceWidget(QTabWidget* tabWidget, QWidget* parent = nullptr);
    ~LazySequenceWidget();
    
    LazySequenceController* getController() const { return controller; }
    ReadOnlyStream<long long>* getStream() const { return controller->getStream(); }
};