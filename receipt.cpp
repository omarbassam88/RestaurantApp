#include "receipt.h"
#include "table.h"
#include <QDebug>

int Receipt::lastReceiptID = 0;

Receipt::Receipt(Table *&table) {
  lastReceiptID++;
  m_ID = lastReceiptID;
  table->setCurrentReceipt(this);
}

void Receipt::addItem(Item &item, int count) {
  if (m_itemsList.find(&item) != m_itemsList.end()) // Check if Item exists
  {
    m_itemsList[&item] += count;
  } else {
    m_itemsList.insert(std::make_pair(&item, 1));
  }
}

void Receipt::removeItem(Item &item) {
  if (m_itemsList.find(&item) != m_itemsList.end()) // Check if Item exists
  {
    m_itemsList.erase(&item);
  } else {
    qDebug("Item is not in the receipt");
  }
}
