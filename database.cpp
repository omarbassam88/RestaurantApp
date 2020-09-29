#include "database.h"
#include "table.h"
#include <iostream>
#include <qdebug.h>
#include <sqlite3.h>

Database *Database::s_instance = nullptr;
int Database::m_lastReceiptId = 0;

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

int Database::getLastReceiptId() {
  sqlite3 *DB;
  int exit = 0;
  char *messaggeError;
  exit = sqlite3_open("restaurant.db", &DB);
  if (exit) {
    std::cerr << "Failed to open File" << std::endl;
  } else {
    std::cout << "Opened Database Successfully" << std::endl;
  }

  std::string select_sql =
      "SELECT * FROM Receipts WHERE   ID = (SELECT MAX(ID)  FROM Receipts);";

  exit = sqlite3_exec(DB, select_sql.c_str(), callback, 0, &messaggeError);

  std::cout << messaggeError << std::endl;

  return m_lastReceiptId;
}

int Database::callback(void *NotUsed, int argc, char **argv, char **azColName) {

  // int argc: holds the number of results
  // (array) azColName: holds each column returned
  // (array) argv: holds each value

  for (int i = 0; i < argc; i++) {

    // Show column name, value, and newline
    std::cout << azColName[i] << " : " << argv[i] << " " << std::endl;
  }

  m_lastReceiptId = atoi(argv[0]);

  // Insert a newline
  std::cout << m_lastReceiptId << std::endl;

  // Return successful
  return 0;
}