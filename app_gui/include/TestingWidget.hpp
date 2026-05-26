#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QProgressBar>
#include <QLabel>

class TestingWidget : public QWidget {
    Q_OBJECT

private:
    QTableWidget* testTable;
    QPushButton* autoTestsBtn;
    QPushButton* largeDataBtn;
    QPushButton* manualBtn;
    QComboBox* largeDataTypeCombo;
    QSpinBox* largeDataSizeSpin;
    QTextEdit* manualInput;
    QTextEdit* manualResult;
    QProgressBar* progressBar;
     
    void createDataFile();
    void setupUI();

private slots:
    void onRunAutoTests();
    void onRunLargeData();
    void onRunManual();

public:
    explicit TestingWidget(QWidget* parent = nullptr);
    ~TestingWidget();
};