#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QProgressBar>
#include <QLabel>
#include <QLineEdit>      
#include <QListWidget>   

class TestingWidget : public QWidget {
    Q_OBJECT

private:
    QTableWidget* testTable;
    QPushButton* autoTestsBtn;
    QPushButton* largeDataBtn;
    QComboBox* largeDataTypeCombo;
    QSpinBox* largeDataSizeSpin;
    QTextEdit* manualResult;
    QProgressBar* progressBar;
    QLineEdit* numberInput;
    QPushButton* addNumberBtn;
    QListWidget* numbersList;
    QPushButton* clearNumbersBtn;
    QPushButton* calcFromListBtn;
    QVector<double> userNumbers;
     
    void createDataFile();
    void setupUI();

private slots:
    void onAddNumber();
    void onClearNumbers();
    void onCalculateFromList();
    void onRunAutoTests();
    void onRunLargeData();
    void onDataTypeChanged(int index);

public:
    explicit TestingWidget(QWidget* parent = nullptr);
    ~TestingWidget();
};