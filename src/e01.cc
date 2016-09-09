#include <iostream>

#include "rocksdb/db.h"
#include "env_nvm.h"

using namespace rocksdb;

std::string kDBPath = "/tmp/rks_e01";

int main() {
  DB* db;

  EnvNVM env;

  Options options;
  options.env = &env;
  options.IncreaseParallelism();
  options.create_if_missing = true;

  Status s = DB::Open(options, kDBPath, &db);
  assert(s.ok());

  s = db->Put(WriteOptions(), "key1", "value");
  assert(s.ok());
  std::string value;

  s = db->Get(ReadOptions(), "key1", &value);
  assert(s.ok());
  assert(value == "value");

  {
    WriteBatch batch;
    batch.Delete("key1");
    batch.Put("key2", value);
    s = db->Write(WriteOptions(), &batch);
  }

  s = db->Get(ReadOptions(), "key1", &value);
  assert(s.IsNotFound());

  db->Get(ReadOptions(), "key2", &value);
  assert(value == "value");

  delete db;

  return 0;
}

