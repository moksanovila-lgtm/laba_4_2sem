// #pragma once

// #include <QMainWindow>
// #include <QTabWidget>
// #include <QSplitter>

// class LazySequenceWidget;
// class StatisticsWidget;
// class TestingWidget;

// class MainWindow : public QMainWindow {
//     Q_OBJECT

// private:
//     QTabWidget* tabWidget;
//     LazySequenceWidget* lazyWidget;
//     StatisticsWidget* statsWidget;
//     TestingWidget* testingWidget;
    
//     void setupUI();

// public:
//     MainWindow(QWidget* parent = nullptr);
//     ~MainWindow();
// };

#pragma once

#include <QMainWindow>
#include <QTabWidget>

class LazySequenceWidget;
class TestingWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QTabWidget* tabWidget;
    LazySequenceWidget* lazyWidget;
    TestingWidget* testingWidget;
    
    void setupUI();

private slots:
    void onTabClose(int index);  

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};