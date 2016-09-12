#include <iostream>
#include <string>
#include "env_nvm.h"

namespace rocksdb {

NVMSequentialFile::NVMSequentialFile(void) : SequentialFile() {
  NVM_DEBUG("\n");
}

NVMSequentialFile::NVMSequentialFile(
  const std::string& fname,
  const EnvOptions& options) : SequentialFile() {
  NVM_DEBUG("fname(%s), options(?)\n", fname.c_str());
}

NVMSequentialFile::~NVMSequentialFile(void) {
  NVM_DEBUG("\n");
}

Status NVMSequentialFile::Read(size_t n, Slice* result, char* scratch) {
  NVM_DEBUG("n(%lu), result(?), scratch(?)\n", n);

  return Status::IOError();
}

Status NVMSequentialFile::Skip(uint64_t n) {
  NVM_DEBUG("n(%lu)\n", n);

  return Status::IOError();
}

Status NVMSequentialFile::InvalidateCache(size_t offset, size_t length) {
  NVM_DEBUG("offset(%lu), length(%lu)\n", offset, length);

  return Status::IOError();
}

}       // namespace rocksdb

