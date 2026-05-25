// #pragma once

// #include <QWidget>
// #include <QComboBox>
// #include <QSpinBox>
// #include <QPushButton>
// #include <QLabel>
// #include <QListWidget>
// #include "LazySequenceController.hpp"

// class LazySequenceWidget : public QWidget {
//     Q_OBJECT

// private:
//     LazySequenceController* controller;
//     QComboBox* generatorCombo;
//     QSpinBox* param1Spin;
//     QSpinBox* param2Spin;
//     QPushButton* applyGenBtn;
//     QSpinBox* generateCountSpin;
//     QPushButton* generateBtn;
//     QLabel* infoLabel;
//     QListWidget* elementsList;
    
//     void setupUI();
//     void updateDisplay();

// private slots:
//     void onApplyGenerator();
//     void onGenerateElements();
//     void onSequenceChanged();
//     void onMaterializedCountChanged(size_t count);
//     void onError(const QString& msg);

// public:
//     explicit LazySequenceWidget(QWidget* parent = nullptr);
//     ~LazySequenceWidget();
    
//     LazySequenceController* getController() const { return controller; }
//     ReadOnlyStream<int>* getStream() const { return controller->getStream(); }
// };

#pragma once

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include "LazySequenceController.hpp"

class LazySequenceWidget : public QWidget {
    Q_OBJECT

private:
    LazySequenceController* controller;
    
    QComboBox* generatorCombo;
    QSpinBox* param1Spin;
    QSpinBox* param2Spin;
    QPushButton* applyGenBtn;
    QSpinBox* generateCountSpin;
    QPushButton* generateBtn;
    QLabel* infoLabel;
    QListWidget* elementsList;
    
    void setupUI();
    void updateDisplay();

private slots:
    void onApplyGenerator();
    void onGenerateElements();
    void onSequenceChanged();
    void onMaterializedCountChanged(size_t count);
    void onError(const QString& msg);
    void onGeneratorTypeChanged(int index);

public:
    explicit LazySequenceWidget(QWidget* parent = nullptr);
    ~LazySequenceWidget();
    
    LazySequenceController* getController() const { return controller; }
    ReadOnlyStream<long long>* getStream() const { return controller->getStream(); }  
};