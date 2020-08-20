#include <QDebug>
#include "item.h"

Item::Item(std::string name, ItemCategory category, float price)
    : m_name(name), m_category(category), m_price(price) {}

