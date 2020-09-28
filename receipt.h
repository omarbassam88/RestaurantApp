#ifndef RECEIPT_H
#define RECEIPT_H

#include "item.h"
#include <algorithm>
#include <unordered_map>
#include <vector>

class Table;

class Receipt {
public:
  Receipt(Table &table);

  void addItem(const Item &item, int count = 1);
  void updateItem(std::string itemName, int count);
  void removeItem(const Item &item);
  void calculate();
  void print();

  int getID() { return m_ID; }
  Table *getTable() { return m_table; }
  std::unordered_map<Item, int> getItemsList() { return m_itemsList; }
  float getSubTotal() { return m_subTotal; }
  float getTotal() { return m_Total; }
  float getTaxes() { return m_taxes; }

  static int lastReceiptID;

private:
  int m_ID;
  Table *m_table;
  std::unordered_map<Item, int> m_itemsList; // List of Items and their count
  float m_subTotal;
  float m_taxes = 0.12;
  float m_Total;
};

#endif // RECEIPT_H
