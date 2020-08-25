#include "receipt.h"
#include "table.h"
#include <QDebug>
#include <filesystem>
#include <fstream>
#include <iostream>

int Receipt::lastReceiptID = 0;

Receipt::Receipt(Table &table) {
  lastReceiptID++;
  m_ID = lastReceiptID;
  m_table = &table;
}

void Receipt::addItem(const Item &item, int count) {
  if (m_itemsList.count(item) > 0) // Check if Item exists
  {
    qDebug("Item Already Exists");
    m_itemsList[item] += count;
  } else {
    m_itemsList.insert(std::make_pair(item, 1));
    qDebug("Item Doesn't Exist Will be Added");
    qDebug("Total Number of Items: %d", m_itemsList.size());
  }

  calculate();
}

void Receipt::removeItem(const Item &item) {
  if (m_itemsList.find(item) != m_itemsList.end()) // Check if Item exists
  {
    m_itemsList.erase(item);
  } else {
    qDebug("Item is not in the receipt");
  }

  calculate();
}

void Receipt::calculate() {
  for (auto &[key, count] : m_itemsList) {
    m_subTotal += key.getPrice() * count;
  }

  m_Total = (1 + m_taxes) * m_subTotal;
}

void Receipt::print() {


  std::string cwd = std::filesystem::current_path();

  std::cout << "Subtotal is : " << m_subTotal << std::endl;
  std::cout << "Current Direcetory is : " << cwd << std::endl;

  std::ofstream outFile;

  outFile.open("TestPrintReceipt.txt");

  outFile << "Receipt No. : " << m_ID << std::endl;

  outFile << "===========================" << std::endl;

  for (auto &[key, count] : m_itemsList) {
    outFile << key.getName() << "   " << count << "   " << key.getPrice()
            << "   " << key.getPrice() * count << std::endl;
  }

  outFile << "===========================" << std::endl;
  outFile << std::setprecision(5) << "Subtotal  : " << m_subTotal << std::endl;
  outFile << "Total     : " << m_Total << std::endl;

  outFile.close();

  qDebug("File Was Successfully Printed");
}
