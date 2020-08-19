#ifndef TABLE_H
#define TABLE_H
#include "receipt.h"

class Table {
public:
  Table();

  void setCurrentReceipt(Receipt &receipt);
  int getID() { return m_ID; }
  Receipt *getCurrentReceipt() { return m_currentReceipt; }
  void setCurrentReceipt(Receipt *rec) { m_currentReceipt = rec; }

  static int lastTableID;

private:
  int m_ID;
  Receipt *m_currentReceipt;
};

#endif // TABLE_H
