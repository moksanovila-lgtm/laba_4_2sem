#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QTextEdit>

class TestingWidget : public QWidget {
    Q_OBJECT

private:
    QTableWidget* testTable;
    QPushButton* autoTestsBtn;
    QPushButton* largeDataBtn;
    QPushButton* manualBtn;
    QTextEdit* manualInput;
    QTextEdit* manualResult;
    
    void setupUI();

private slots:
    void onRunAutoTests();
    void onRunLargeData();
    void onRunManual();

public:
    explicit TestingWidget(QWidget* parent = nullptr);
    ~TestingWidget();
};