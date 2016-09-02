#include <iostream>
#include <string>
#include "env_nvm.h"

namespace rocksdb {

//
//      RANDOM ACCESS FILE
//
NVMRandomAccessFile::NVMRandomAccessFile(void) : RandomAccessFile() {}

NVMRandomAccessFile::NVMRandomAccessFile(const std::string& fname,
                                         const EnvOptions& options) : RandomAccessFile() {
  fname_ = fname;
}

NVMRandomAccessFile::~NVMRandomAccessFile(void) { }

Status NVMRandomAccessFile::Read(uint64_t offset, size_t n, Slice* result, char* scratch) const {

  // TODO: put something into result
  return Status::OK();
}

bool NVMRandomAccessFile::ShouldForwardRawRequest(void) const {
  // TODO: return state
  return false;
}

void NVMRandomAccessFile::EnableReadAhead(void) {
  // TODO: flip a switch
  return;
}

size_t NVMRandomAccessFile::GetUniqueId(char* id, size_t max_size) const {

  // TODO: implement IDs
  return 0;
}

void NVMRandomAccessFile::Hint(AccessPattern pattern)
{
  // TODO: store the hinted access pattern
}

Status NVMRandomAccessFile::InvalidateCache(size_t offset, size_t length)
{
  // TODO: implement cache and do the actual invalidation
  return Status::OK();
}

}       // namespace rocksdb

