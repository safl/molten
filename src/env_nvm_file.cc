#include <iostream>
#include <string>
#include "env_nvm.h"

namespace rocksdb {

NVMFile::NVMFile(void) : env_(NULL), name_(""), refs_(0) {
  NVM_DEBUG("env_(%p), name_(%s), refs_(%d)\n", env_, name_.c_str(), refs_);
}

NVMFile::NVMFile(
  Env* env,
  const std::string& fname
) : env_(env), name_(fname), refs_(0) {
  NVM_DEBUG("env_(%p), name_(%s), refs_(%d)\n", env_, name_.c_str(), refs_);
}

NVMFile::~NVMFile(void) {
  NVM_DEBUG("\n");
}

bool NVMFile::UseDirectIO(void) const {
  NVM_DEBUG("\n");

  return true;
}

bool NVMFile::UseOSBuffer(void) const {
  NVM_DEBUG("\n");

  return false;
}

Status NVMFile::Append(const Slice& data) {
  NVM_DEBUG("data(?)\n");

  return Status::IOError();
}

Status NVMFile::Close(void)
{
  NVM_DEBUG("\n");

  return Status::IOError();
}

Status NVMFile::PositionedAppend(const Slice& data, uint64_t offset)
{
  NVM_DEBUG("date(?), offset(%lu)\n", offset);

  return Status::IOError();
}

Status NVMFile::Truncate(uint64_t size)
{
  NVM_DEBUG("size(%lu)\n", size);

  return Status::IOError();
}

Status NVMFile::Flush(void)
{
  NVM_DEBUG("\n");

  return Status::IOError();
}

Status NVMFile::Sync(void)
{
  NVM_DEBUG("\n");

  return Status::IOError();
}

Status NVMFile::Fsync(void)
{
  NVM_DEBUG("\n");

  return Status::IOError();
}


bool NVMFile::IsSyncThreadSafe(void) const {
  NVM_DEBUG("\n");

  return true;
}

size_t NVMFile::GetUniqueId(char* id, size_t max_size) const {
  NVM_DEBUG("id(%s), max_size(%lu\n", id, max_size);

  return 0;
}

uint64_t NVMFile::GetFileSize(void) {
  NVM_DEBUG("\n");

  return 0;
}

Status NVMFile::InvalidateCache(size_t offset, size_t length)
{
  NVM_DEBUG("offset(%lu), length(%lu)\n", offset, length);

  return Status::IOError();
}

Status NVMFile::RangeSync(uint64_t offset, uint64_t nbytes)
{
  NVM_DEBUG("offset(%lu), nbytes(%lu)\n", offset, nbytes);

  return Status::IOError();
}

void NVMFile::PrepareWrite(size_t offset, size_t len)
{
  NVM_DEBUG("offset(%lu), len(%lu)\n", offset, len);
}

Status NVMFile::Allocate(uint64_t offset, uint64_t len)
{
  NVM_DEBUG("offset(%lu), len(%lu)\n", offset, len);

  return Status::IOError();
}

}       // namespace rocksdb

