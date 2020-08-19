#ifndef TABLE_H
#define TABLE_H
#include "receipt.h"

class Table
{
public:
    Table();

    void setCurrentReceipt(Receipt &receipt);

    static int lastTableID;

private:
    int m_ID;
    Receipt *m_currentReceipt;
};

#endif // TABLE_H
