#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QString>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    timer_1s = new QTimer(this);
    connect(timer_1s, SIGNAL(timeout()),this,SLOT(UpdateTime()));
    timer_1s->start(1000);



}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer_1s;
}


void MainWindow::on_CreateTables_Button_clicked()
{
    uint16_t Number = int(ui->NumTables_Input->value());
    ui->stackedWidget->setCurrentIndex(1);
    CreateTables(Number);
}

void MainWindow::UpdateTime()
{
    ui->lbl_Time->setText(QTime::currentTime().toString("hh:mm:ss"));
}

void MainWindow::CreateTables(uint16_t Number)
{
    qDebug("Total Number of Tables : %d", Number);

    int current_row = 0;
    int current_column = 0;

    for (int i = 0; i< Number; i++) {
        QString Button_Name;
        Button_Name = QString::number(i+1);
        QPushButton* Table_Button = new QPushButton(Button_Name);
        Table_Button->setMaximumSize(100,50);


        ui->TablesGrid->addWidget(Table_Button,current_row,current_column );

        connect(Table_Button, SIGNAL(clicked()),this,SLOT(TablePage()));

        if( current_column == (m_max_column_count-1) )
        {
            current_column = 0;
            ++current_row;
        }
        else
        {
            ++current_column;
        }

        qDebug("Table %d is Created", i+1);
    }
}

void MainWindow::TablePage()
{
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    qDebug("Table no %s is Selected",qUtf8Printable(buttonSender->text()));
    ui->stackedWidget->setCurrentIndex(2);
    ui->TablePageLabel->setText("Table no." + buttonSender->text());

}



void MainWindow::go_to_PreviousPage()
{
    int current_page = ui->stackedWidget->currentIndex();
    qDebug("%d", current_page);
    ui->stackedWidget->setCurrentIndex(current_page - 1);
}

void MainWindow::on_BackButton_clicked()
{
    go_to_PreviousPage();

}

void MainWindow::on_BackButton_2_clicked()
{
    go_to_PreviousPage();
}
