#include "mainwindow.h"

#include "delegate.h"
#include <QDebug>
#include <QStandardItemModel>
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

  // Set Receipt Items Table View
  // ----------------------------
  m_model = new QStandardItemModel(this);
  ui->tableReceiptItemsList->setModel(m_model);
  clearModel();

  // Add Button for Each Category
  for (std::string categoryName : Item::getCategoryNames()) {

    QPushButton *button = new QPushButton(QString::fromStdString(categoryName));
    ui->categoritesListSelection->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(CategoryItemsPage()));
  }

  // Setup Database

  Database::create();

  m_db = Database::getInstance();

// TODO Set up last receipt Id from Database
  Receipt::lastReceiptID=300;

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
    clearModel();
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

void MainWindow::clearModel() {
  m_model->clear();
  m_model->setHorizontalHeaderItem(0, new QStandardItem("items"));
  m_model->setHorizontalHeaderItem(1, new QStandardItem("count"));
  ui->tableReceiptItemsList->verticalHeader()->hide();
  ui->tableReceiptItemsList->setColumnWidth(0, 400);
  ui->tableReceiptItemsList->setColumnWidth(1, 50);
  ui->tableReceiptItemsList->setSortingEnabled(false);
  ui->tableReceiptItemsList->horizontalHeader()->setStretchLastSection(true);
  ui->tableReceiptItemsList->setSelectionBehavior(
      QAbstractItemView::SelectRows);
  ui->tableReceiptItemsList->setSelectionMode(
      QAbstractItemView::SingleSelection);
  SpinBoxDelegate *spinBox = new SpinBoxDelegate(this);
  ui->tableReceiptItemsList->setItemDelegateForColumn(1, spinBox);
  connect(spinBox, SIGNAL(valueEmitted(int)), this, SLOT(UpdateReceipt(int)));
}

void MainWindow::ShowReceiptItems(Receipt *receipt) {
  // TODO
  // Clear Items List from Previous Receipt
  clearModel();
  // Add current Receipt Items to the TableView
  if (!receipt) {
    qDebug("Table no. %d has no Receipt", m_selectedTable->getID());
    clearModel();
  } else {
    for (auto &[key, value] : receipt->getItemsList()) {

      QStandardItem *nameItem =
          new QStandardItem(QString::fromStdString(key.getName()));

      QStandardItem *countItem = new QStandardItem();
      countItem->setData(value, Qt::EditRole);

      m_model->appendRow({nameItem, countItem});
    }

    // Update Total and SubTotal
    UpdateReceiptTotal();
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
  // Clear the Buttons list from previous categories
  QLayoutItem *child;
  while ((child = ui->CategoryItemsButtons->takeAt(0)) != nullptr) {

    delete child->widget(); // delete the widget
    delete child;           // delete the layout item
  }
  // Add list of items for selected Category
  for (Item *&itm : categoryItems) {
    QPushButton *item_button =
        new QPushButton(QString::fromStdString(itm->getName()));
    item_button->setText(QString::fromStdString(itm->getName()));
    ui->CategoryItemsButtons->addWidget(item_button);
    // setup the button call Back function
    connect(item_button, SIGNAL(clicked(bool)), this,
            SLOT(on_AddSelectedItemButton_clicked()));
  }
}

void MainWindow::on_BackButton_clicked() { go_to_PreviousPage(); }

void MainWindow::on_BackButton_2_clicked() { go_to_PreviousPage(); }

void MainWindow::on_BackButton_3_clicked() { go_to_PreviousPage(); }

void MainWindow::on_AddItemButton_clicked() { CategoriesPage(); }

void MainWindow::on_RemoveItemButton_clicked() {
  QItemSelectionModel *selected_item =
      ui->tableReceiptItemsList->selectionModel();

  if (!selected_item) {
    qDebug("No Item is selected");
  } else {

    qDebug() << selected_item->selectedRows(0).value(0).data().toString();
    std::string itemName = qUtf8Printable(
        selected_item->selectedRows(0).value(0).data().toString());

    for (auto &[key, value] :
         m_selectedTable->getCurrentReceipt()->getItemsList()) {
      if (key.getName() == itemName) {
        m_selectedTable->getCurrentReceipt()->removeItem(key);
      }
    }
  }

  // Update UI
  ShowReceiptItems(m_selectedTable->getCurrentReceipt());
}

void MainWindow::on_AddSelectedItemButton_clicked() {

  QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());

  QString selected_item = buttonSender->text();

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

void MainWindow::on_CancelButton_clicked() {
  ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_PrintReceiptButton_clicked() {
  if (!m_selectedTable->getCurrentReceipt()) {
    qDebug("No Receipt assigned for selected Table");
  } else {
    m_selectedTable->getCurrentReceipt()->print();
    // Write the receipt to Database
    m_db->addReceipt(m_selectedTable->getCurrentReceipt());
    m_selectedTable->setCurrentReceipt(nullptr);
    ui->stackedWidget->setCurrentIndex(1);
  }
}

void MainWindow::UpdateReceipt(int num) {
  qDebug("Receipt is Updating..");
  qDebug("%d", num);

  QItemSelectionModel *select = ui->tableReceiptItemsList->selectionModel();
  qDebug() << select->selectedRows(0).value(0).data().toString();
  std::string itemName =
      qUtf8Printable(select->selectedRows(0).value(0).data().toString());
  m_selectedTable->getCurrentReceipt()->updateItem(itemName, num);

  // Update Calculated Total and Subtotal
  UpdateReceiptTotal();
}

void MainWindow::UpdateReceiptTotal() {
  ui->SubTotalLabelValue->setText(QString::number(
      m_selectedTable->getCurrentReceipt()->getSubTotal(), 'f', 2));
  ui->TotalLabelValue->setText(QString::number(
      m_selectedTable->getCurrentReceipt()->getTotal(), 'f', 2));
}
