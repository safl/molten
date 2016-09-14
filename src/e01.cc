#include <iostream>

#include "rocksdb/db.h"
#include "env_nvm.h"

using namespace rocksdb;

std::string kDBPath = "/tmp/testdb";

int main() {
  DB* db;

  EnvNVM env;

  Options options;
  options.env = &env;
  options.IncreaseParallelism();
  options.create_if_missing = true;

  std::cout << "*** OPENING ***" << std::endl;

  Status s = DB::Open(options, kDBPath, &db);
  std::cout << s.ToString() << std::endl;
  assert(s.ok());

  std::cout << "*** PUT ***" << std::endl;

  s = db->Put(WriteOptions(), "key1", "value");
  assert(s.ok());
  std::string value;

  std::cout << "*** GET ***" << std::endl;

  s = db->Get(ReadOptions(), "key1", &value);
  assert(s.ok());
  assert(value == "value");

  std::cout << "*** BATCH ***" << std::endl;
  {
    WriteBatch batch;
    batch.Delete("key1");
    batch.Put("key2", value);
    s = db->Write(WriteOptions(), &batch);
  }


  std::cout << "*** GET ***" << std::endl;

  s = db->Get(ReadOptions(), "key1", &value);
  assert(s.IsNotFound());

  std::cout << "*** GET ***" << std::endl;

  db->Get(ReadOptions(), "key2", &value);
  assert(value == "value");

  delete db;

  return 0;
}

