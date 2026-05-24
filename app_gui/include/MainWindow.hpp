#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QSplitter>

class LazySequenceWidget;
class StatisticsWidget;
class TestingWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QTabWidget* tabWidget;
    LazySequenceWidget* lazyWidget;
    StatisticsWidget* statsWidget;
    TestingWidget* testingWidget;
    
    void setupUI();

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};