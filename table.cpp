#include "table.h"

int Table::lastTableID = 0;

Table::Table() {
  lastTableID++;
  m_ID = lastTableID;
}

void Table::addItem(const Item &item) {
  // check if Table already has a Receipt
  if (!m_currentReceipt) {
    m_currentReceipt = new Receipt(*this);
    qDebug("New Receipt %d is Created", m_currentReceipt->getID());
  } else {
    qDebug("Receipt %d already Exists", m_currentReceipt->getID());
  }

  m_currentReceipt->addItem(item);
}
