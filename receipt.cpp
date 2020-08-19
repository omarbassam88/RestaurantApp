#include "receipt.h"

int Receipt::lastReceiptID = 0;

Receipt::Receipt(Table &table)
{
    lastReceiptID++;
    m_ID = lastReceiptID;
    m_table = &table;
}

void Receipt::addItem(Item* item)
{
    m_itemsList.push_back(item);
}

void Receipt::removeItem(Item item)
{
    
}
