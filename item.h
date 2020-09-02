#ifndef ITEM_H
#define ITEM_H
#include <QtDebug>
#include <string>
#include <functional>

enum ItemCategory { Food, Beverages, Other };

class Item {
public:
  Item(std::string name, ItemCategory category, float price);

  int getID() const { return m_ID; }
  std::string getName() const { return m_name; }
  float getPrice() const { return m_price; }
  ItemCategory getCategory() const { return m_category; }

  // Override Operator < to be able to find in std::map
  friend bool operator<(const Item &lhs, const Item &rhs) {
    return lhs.getName() < rhs.getName();
  }

  bool operator==(const Item &rhs) const { return (m_name == rhs.getName()); }

  static std::vector<std::string> getCategoryNames() {
    return {"Food", "Beverages", "Other"};
  }

private:
  int m_ID;
  std::string m_name;
  float m_price;
  ItemCategory m_category;
};

template <> struct std::hash<Item> {
  size_t operator()(const Item &k) const {
    return (std::hash<std::string>()(k.getName()));
  }
};
#endif // ITEM_H
