#ifndef RECEIPT_H
#define RECEIPT_H

#include <vector>
#include "item.h"


class Table;

class Receipt
{
public:
    Receipt(Table &table);

    void addItem(Item *item);
    void removeItem(Item item);

    static int lastReceiptID;

private:
    int m_ID;
    Table* m_table;
    std::vector<Item *> m_itemsList;
};

#endif // RECEIPT_H
