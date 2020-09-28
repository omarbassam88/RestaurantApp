#ifndef DATABASE_H
#define DATABASE_H

#include "receipt.h"
class Database {
public:
  Database();
  static Database *getInstance();
  static bool create();
  bool addReceipt(Receipt * receipt);

private:
  static Database *s_instance;
};

#endif // DATABASE_H
