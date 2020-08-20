#include "mainwindow.h"

#include <QDebug>
#include <QString>
#include <QTime>
#include <QTimer>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->stackedWidget->setCurrentIndex(0);
  timer_1s = new QTimer(this);
  connect(timer_1s, SIGNAL(timeout()), this, SLOT(UpdateTime()));
  timer_1s->start(1000);

  // Set Receipt Items List View
  ui->tableReceiptItemsList->headerItem()->setText(0, "Item");
  ui->tableReceiptItemsList->headerItem()->setText(1, "Count");
  ui->tableReceiptItemsList->setColumnWidth(0, 800);
  ui->tableReceiptItemsList->setColumnWidth(1, 50);
  ui->tableReceiptItemsList->header()->setSectionResizeMode(
      0, QHeaderView::Stretch);
}

MainWindow::~MainWindow() {
  delete ui;
  delete timer_1s;
}

void MainWindow::on_CreateTables_Button_clicked() {
  uint16_t Number = int(ui->NumTables_Input->value());
  ui->stackedWidget->setCurrentIndex(1);
  CreateTables(Number);
}

void MainWindow::UpdateTime() {
  ui->lbl_Time->setText(QTime::currentTime().toString("hh:mm:ss"));
}

void MainWindow::CreateTables(uint16_t Number) {
  qDebug("Total Number of Tables : %d", Number);

  int current_row = 0;
  int current_column = 0;

  for (int i = 0; i < Number; i++) {
    m_tablesList.push_back(new Table());

    // Create Table Button
    QString Button_Name;
    Button_Name = QString::number(i + 1);
    QPushButton *Table_Button = new QPushButton(Button_Name);
    Table_Button->setMaximumSize(100, 50);

    ui->TablesGrid->addWidget(Table_Button, current_row, current_column);

    connect(Table_Button, SIGNAL(clicked()), this, SLOT(TablePage()));

    if (current_column == (m_max_column_count - 1)) {
      current_column = 0;
      ++current_row;
    } else {
      ++current_column;
    }

    qDebug("Table %d is Created", i + 1);
  }
}

void MainWindow::TablePage() {
  // Get selected Table
  QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
  int table_index = buttonSender->text().toInt() - 1;
  m_selectedTable = m_tablesList.at(table_index);
  qDebug("Table no %d is Selected", m_selectedTable->getID());

  Receipt *receipt;

  // check if Table already has a Receipt
  if (m_selectedTable->getCurrentReceipt() == NULL) {
    receipt = new Receipt(m_selectedTable);
    qDebug("New Receipt %d is Created", receipt->getID());
  } else {
    receipt = m_selectedTable->getCurrentReceipt();
    qDebug("Receipt %d already Exists", receipt->getID());
  }

  // Update UI
  ui->stackedWidget->setCurrentIndex(2);
  ui->TablePageLabel->setText("Table no." + buttonSender->text());
  ShowReceiptItems(receipt);
}

void MainWindow::go_to_PreviousPage() {
  int current_page = ui->stackedWidget->currentIndex();
  qDebug("%d", current_page);
  ui->stackedWidget->setCurrentIndex(current_page - 1);
}

void MainWindow::ShowReceiptItems(Receipt *receipt) {
  // Clear Items List from Previous Receipt
  ui->tableReceiptItemsList->clear();

  // Add current Receipt Items to the ListView

  for (auto &[key, value] : receipt->getItemsList()) {

    QTreeWidgetItem *newItem = new QTreeWidgetItem(ui->tableReceiptItemsList);
    newItem->setText(0, QString::fromStdString(key.getName()));
    newItem->setText(1, QString::number(value));
    ui->tableReceiptItemsList->addTopLevelItem(newItem);
  }
}

void MainWindow::on_BackButton_clicked() { go_to_PreviousPage(); }

void MainWindow::on_BackButton_2_clicked() { go_to_PreviousPage(); }

void MainWindow::on_AddItemButton_clicked() {
  ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_RemoveItemButton_clicked() {
  if (ui->tableReceiptItemsList->selectedItems().empty()) {
    qDebug("No Item is selected");
  } else {
    QString selected_item =
        ui->tableReceiptItemsList->selectedItems().first()->text(0);

    for (auto &[key, value] :
         m_selectedTable->getCurrentReceipt()->getItemsList()) {
      if (key.getName() == selected_item.toStdString()) {
        m_selectedTable->getCurrentReceipt()->removeItem(key);
      }
    }
  }

  // Update UI
  ShowReceiptItems(m_selectedTable->getCurrentReceipt());
}

void MainWindow::on_BackButton_3_clicked() { go_to_PreviousPage(); }

void MainWindow::on_FoodCategoryButton_clicked() {
  /* ADD Dummy Food to Receipt */
  Item item("Sandwich", ItemCategory::Food, 40.0);

  m_selectedTable->getCurrentReceipt()->addItem(item);

  ShowReceiptItems(m_selectedTable->getCurrentReceipt());

  go_to_PreviousPage();
}

void MainWindow::on_BeveragesCategoryButton_clicked() {
  // Add Dummy Beverage to the Receipt
  Item item("Pepsi", ItemCategory::Bevereges, 20.0);

  m_selectedTable->getCurrentReceipt()->addItem(item);

  ShowReceiptItems(m_selectedTable->getCurrentReceipt());

  go_to_PreviousPage();
}
