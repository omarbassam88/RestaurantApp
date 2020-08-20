#ifndef ITEM_H
#define ITEM_H
#include <QtDebug>
#include <string>

enum ItemCategory { Food, Bevereges };

class Item {
public:
  Item(std::string name, ItemCategory category, float price);

  int getID() { return m_ID; }
  std::string getName() const { return m_name; }
  float getPrice() { return m_price; }
  ItemCategory getCategory() { return m_category; }

  // Override Operator < to be able to find in std::map
  friend bool operator<(const Item &lhs, const Item &rhs) {
    qDebug("Comparing Two Items");
    return lhs.getName() < rhs.getName();
  }

private:
  int m_ID;
  std::string m_name;
  float m_price;
  ItemCategory m_category;
};

#endif // ITEM_H
