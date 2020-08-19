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
    Item(std::string name, ItemCategory category);

private:
    std::string m_name;
    int m_ID;
    ItemCategory m_category;
};

#endif // ITEM_H
