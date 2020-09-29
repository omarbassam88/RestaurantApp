#ifndef DATABASE_H
#define DATABASE_H

#include "receipt.h"

/*
  ==========================================================
  Database Class a Database Sigleton
  used as an abstraction layer for SQLite3
  to handle creating , reading and writing from the database
  =========================================================
*/
class Database {
public:
  Database();
  static Database *getInstance();
  static bool create();
  bool addReceipt(Receipt *receipt);
  int getLastReceiptId();

private:
  static Database *s_instance;
  static int m_lastReceiptId;
  static int callback(void *NotUsed, int argc, char **argv, char **azColName);
};

#endif // DATABASE_H
