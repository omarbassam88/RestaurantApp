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
  ui->tableReceiptItemsList->setSortingEnabled(false);

  // Add Button for Each Category
  for (std::string categoryName : Item::getCategoryNames()) {

    QPushButton *button = new QPushButton(QString::fromStdString(categoryName));
    ui->categoritesListSelection->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(CategoryItemsPage()));
  }

  // Create Inventory
  /* ========================================================== */
  // Add Dummy Beverage to the Inventory
  Item *item = new Item("Pepsi", ItemCategory::Beverages, 20.0);
  m_itemsInventory.push_back(item);
  item = new Item("Sandwich", ItemCategory::Food, 20.0);
  m_itemsInventory.push_back(item);
  item = new Item("Orange Juice", ItemCategory::Beverages, 20.0);
  m_itemsInventory.push_back(item);
  item = new Item("Pasta", ItemCategory::Food, 20.0);
  m_itemsInventory.push_back(item);
  item = new Item("Apple Juice", ItemCategory::Beverages, 20.0);
  m_itemsInventory.push_back(item);
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
      current_row++;
    } else {
      current_column++;
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
    ui->tableReceiptItemsList->clear();
  } else {
    receipt = m_selectedTable->getCurrentReceipt();
    qDebug("Receipt %d already Exists", receipt->getID());
    ShowReceiptItems(receipt);
  }

  // Update UI
  ui->stackedWidget->setCurrentIndex(2);
  ui->TablePageLabel->setText("Table no." + buttonSender->text());
}

void MainWindow::CategoriesPage() { ui->stackedWidget->setCurrentIndex(3); }

void MainWindow::CategoryItemsPage() {
  QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
  std::string categoryName = buttonSender->text().toStdString();

  ItemCategory category;

  if (categoryName == "Food") {
    category = ItemCategory::Food;
  } else if (categoryName == "Beverages") {
    category = ItemCategory::Beverages;
  } else {
    category = ItemCategory::Other;
  }

  // update UI
  ShowCategoryItems(category);
  ui->SelectedCategoryLabel->setText(buttonSender->text());
  ui->stackedWidget->setCurrentIndex(4);
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
  if (!receipt) {
    qDebug("Table no. %d has no Receipt", m_selectedTable->getID());
  } else {
    for (auto &[key, value] : receipt->getItemsList()) {

      QTreeWidgetItem *newItem = new QTreeWidgetItem(ui->tableReceiptItemsList);
      newItem->setText(0, QString::fromStdString(key.getName()));
      newItem->setText(1, QString::number(value));
      ui->tableReceiptItemsList->addTopLevelItem(newItem);
    }

    ui->SubTotalLabelValue->setText(QString::number(
        m_selectedTable->getCurrentReceipt()->getSubTotal(), 'f', 2));
    ui->TotalLabelValue->setText(QString::number(
        m_selectedTable->getCurrentReceipt()->getTotal(), 'f', 2));
  }
}

void MainWindow::ShowCategoryItems(ItemCategory category) {

  std::vector<Item *> categoryItems;

  // Set Category Items
  if (category == ItemCategory::Other) {
    categoryItems = m_itemsInventory;
  } else {
    for (Item *&itm : m_itemsInventory) {
      if (itm->getCategory() == category) {
        categoryItems.push_back(itm);
      }
    }
  }

  // Update UI
  ui->CateryItemsList->clear();
  for (Item *&itm : categoryItems) {
    QTreeWidgetItem *newItem = new QTreeWidgetItem(ui->CateryItemsList);
    newItem->setText(0, QString::fromStdString(itm->getName()));
    ui->CateryItemsList->addTopLevelItem(newItem);
  }
}

void MainWindow::on_BackButton_clicked() { go_to_PreviousPage(); }

void MainWindow::on_BackButton_2_clicked() { go_to_PreviousPage(); }

void MainWindow::on_BackButton_3_clicked() { go_to_PreviousPage(); }

void MainWindow::on_AddItemButton_clicked() { CategoriesPage(); }

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

void MainWindow::on_AddSelectedItemButton_clicked() {
  if (ui->CateryItemsList->selectedItems().empty()) {
    qDebug("No Item is selected");
  } else {
    QString selected_item =
        ui->CateryItemsList->selectedItems().first()->text(0);

    qDebug("%s Item is Selected", qUtf8Printable(selected_item));

    for (Item *&itm : m_itemsInventory) {
      if (itm->getName() == selected_item.toStdString()) {
        m_selectedTable->addItem(*itm);
      }
    }

    // Update UI
    ShowReceiptItems(m_selectedTable->getCurrentReceipt());
    ui->stackedWidget->setCurrentIndex(2);
  }
}

void MainWindow::on_CancelButton_clicked() {
  ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_PrintReceiptButton_clicked() {
  if (!m_selectedTable->getCurrentReceipt()) {
    qDebug("No Receipt assigned for selected Table");
  } else {
    m_selectedTable->getCurrentReceipt()->print();
    m_selectedTable->setCurrentReceipt(nullptr);
    ui->stackedWidget->setCurrentIndex(1);
  }
}
