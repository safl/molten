#include <iostream>
#include <string>
#include "env_nvm.h"

namespace rocksdb {

//
//      RANDOM ACCESS FILE
//
NVMRandomAccessFile::NVMRandomAccessFile(void) : RandomAccessFile() {
  NVM_DEBUG("\n");
}

NVMRandomAccessFile::NVMRandomAccessFile(
  const std::string& fname,
  const EnvOptions& options
) : RandomAccessFile() {
  NVM_DEBUG("fname(%s), options(?)\n", fname.c_str());
}

NVMRandomAccessFile::~NVMRandomAccessFile(void) {
  NVM_DEBUG("\n");
}

Status NVMRandomAccessFile::Read(
  uint64_t offset,
  size_t n,
  Slice* result,
  char* scratch
) const {
  NVM_DEBUG("offset(%lu), n(%lu), result(?), scratch(?)\n", offset, n);

  return Status::IOError();
}

bool NVMRandomAccessFile::ShouldForwardRawRequest(void) const {
  NVM_DEBUG("\n");

  return false;
}

void NVMRandomAccessFile::EnableReadAhead(void) {
  NVM_DEBUG("\n");

  return;
}

size_t NVMRandomAccessFile::GetUniqueId(char* id, size_t max_size) const {
  NVM_DEBUG("id(%s), max_size(%lu)\n", id, max_size);

  return 0;
}

void NVMRandomAccessFile::Hint(AccessPattern pattern)
{
  NVM_DEBUG("pattern(%d)\n", pattern);
}

Status NVMRandomAccessFile::InvalidateCache(size_t offset, size_t length)
{
  NVM_DEBUG("offset(%lu), length(%lu)\n", offset, length);

  return Status::IOError();
}

}       // namespace rocksdb

