#include "receipt.h"
#include "table.h"
#include <QDebug>

int Receipt::lastReceiptID = 0;

Receipt::Receipt(Table &table) {
  lastReceiptID++;
  m_ID = lastReceiptID;
  m_table = &table;
}

void Receipt::addItem(const Item &item, int count) {
  if (m_itemsList.count(item) > 0) // Check if Item exists
  {
    qDebug("Item Already Exists");
    m_itemsList[item] += count;
  } else {
    m_itemsList.insert(std::make_pair(item, 1));
    qDebug("Item Doesn't Exist Will be Added");
    qDebug("Total Number of Items: %d", m_itemsList.size());
  }
}

void Receipt::removeItem(const Item &item) {
  if (m_itemsList.find(item) != m_itemsList.end()) // Check if Item exists
  {
    m_itemsList.erase(item);
  } else {
    qDebug("Item is not in the receipt");
  }
}
