#include <iostream>
#include <string>
#include "env_nvm.h"

namespace rocksdb {

NVMSequentialFile::NVMSequentialFile(void) : SequentialFile() {}

NVMSequentialFile::NVMSequentialFile(const std::string& fname,
                                     const EnvOptions& options) : SequentialFile() {
  fname_ = fname;
}

NVMSequentialFile::~NVMSequentialFile(void) { }

Status NVMSequentialFile::Read(size_t n, Slice* result, char* scratch) {

  return Status::OK();
}

Status NVMSequentialFile::Skip(uint64_t n) {

  return Status::OK();
}

Status NVMSequentialFile::InvalidateCache(size_t offset, size_t length) {

  return Status::OK();
}

Status EnvNVM::NewSequentialFile(const std::string& fname,
                                 unique_ptr<SequentialFile>* result,
                                 const EnvOptions& options) {
  unique_ptr<NVMSequentialFile> file(new NVMSequentialFile(fname, options));

  *result = std::move(file);

  return Status::OK();
}

}       // namespace rocksdb

