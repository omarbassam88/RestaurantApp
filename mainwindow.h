#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "table.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void go_to_PreviousPage();

  void clearModel();

  void UpdateReceiptTotal();

private slots:
  void on_CreateTables_Button_clicked();
  void UpdateTime();
  void TablePage();
  void CategoriesPage();
  void CategoryItemsPage();
  void UpdateReceipt(int num);

  void on_BackButton_clicked();

  void on_BackButton_2_clicked();

  void on_AddItemButton_clicked();

  void on_RemoveItemButton_clicked();

  void on_BackButton_3_clicked();

  void on_AddSelectedItemButton_clicked();

  void on_CancelButton_clicked();

  void on_PrintReceiptButton_clicked();

private:
  Ui::MainWindow *ui;
  QTimer *timer_1s;
  int m_max_column_count = 4;
  std::vector<Table *> m_tablesList;
  std::vector<Item *> m_itemsInventory;

  QStandardItemModel *m_model;

  Table *m_selectedTable;

  void CreateTables(uint16_t Number);
  void ShowReceiptItems(Receipt *receipt);
  void ShowCategoryItems(ItemCategory category);
};
#endif // MAINWINDOW_H
