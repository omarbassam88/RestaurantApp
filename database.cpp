#include "database.h"
#include "table.h"
#include <iostream>
#include <qdebug.h>
#include <sqlite3.h>

Database *Database::s_instance = nullptr;

Database::Database() {}

Database *Database::getInstance() { return s_instance; }

bool Database::create() {
  if (s_instance) {
    qDebug("A Databse instance already Exists");
    return false;
  }

  s_instance = new Database();
  return true;
}

bool Database::addReceipt(Receipt *receipt) {
  sqlite3 *DB;
  int exit = 0;
  char *messaggeError;
  exit = sqlite3_open("restaurant.db", &DB);
  if (exit) {
    std::cerr << "Failed to open File" << std::endl;
  } else {
    std::cout << "Opened Database Successfully" << std::endl;
  }

  // Create the Receipts table only if it doesn't exist

  std::string create_sql = "CREATE TABLE IF NOT EXISTS Receipts ("
                           "ID INT PRIMARY KEY NOT NULL, "
                           "TABLE_NO INT NOT NULL, "
                           "SUBTOTAL REAL NOT NULL, "
                           "TOTAL REAL NOT NULL); ";
  exit = sqlite3_exec(DB, create_sql.c_str(), NULL, 0, &messaggeError);

  std::cout << messaggeError << std::endl;

  if (exit != SQLITE_OK) {
    std::cerr << "Error Create Table" << std::endl;
    sqlite3_free(messaggeError);
  } else {
    std::cout << "Table created Successfully" << std::endl;
  }

  // Insert Receipt Data into the databse
  std::string insert_sql = "INSERT INTO Receipts VALUES(" +
                           std::to_string(receipt->getID()) + "," +
                           std::to_string(receipt->getTable()->getID()) + "," +
                           std::to_string(receipt->getSubTotal()) + "," +
                           std::to_string(receipt->getTotal()) + ");";
  exit = sqlite3_exec(DB, insert_sql.c_str(), NULL, 0, &messaggeError);

  std::cout << messaggeError << std::endl;

  if (exit != SQLITE_OK) {
    std::cerr << "Error Inserting into Receipts Table" << std::endl;
    sqlite3_free(messaggeError);
  } else {
    std::cout << "Receipt Successfully added to Database" << std::endl;
  }
  // Close the database before exit
  sqlite3_close(DB);
  return true;
}
