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