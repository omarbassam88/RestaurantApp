#ifndef ITEM_H
#define ITEM_H

#include <string>

enum ItemCategory
{
    Food,
    Bevereges
};

class Item
{
public:
    Item(std::string name, ItemCategory category, float price);

private:
    int m_ID;
    std::string m_name;
    float m_price;
    ItemCategory m_category;
};

#endif // ITEM_H
