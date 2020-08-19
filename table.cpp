#include "table.h"

int Table::lastTableID = 0;

Table::Table()
{
    lastTableID++;
    m_ID = lastTableID;
}

void Table::setCurrentReceipt(Receipt &receipt)
{
    m_currentReceipt = &receipt;
}
