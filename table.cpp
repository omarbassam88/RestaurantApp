#include "table.h"

int Table::lastTableID = 0;

Table::Table() {
  lastTableID++;
  m_ID = lastTableID;
}
