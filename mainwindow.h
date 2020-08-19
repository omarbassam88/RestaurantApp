#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void go_to_PreviousPage();

private slots:
    void on_CreateTables_Button_clicked();
    void UpdateTime();
    void TablePage();

    void on_BackButton_clicked();

    void on_BackButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer_1s;
    int m_max_column_count = 4;

    void CreateTables(uint16_t Number);
};
#endif // MAINWINDOW_H
