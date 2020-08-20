#ifndef RECEIPT_H
#define RECEIPT_H

#include "item.h"
#include <map>
#include <vector>

class Table;

class Receipt {
public:
  Receipt(Table *&table);

  void addItem(Item &item, int count = 1);
  void removeItem(const Item &item);

  int getID() { return m_ID; }
  std::map<Item, int> getItemsList() { return m_itemsList; }

  static int lastReceiptID;

private:
  int m_ID;
  Table *m_table;
  std::map<Item, int> m_itemsList; // List of Items and their count
  float m_subTotal;
  float m_taxes = 0.12;
  float m_Total;
};

#endif // RECEIPT_H
