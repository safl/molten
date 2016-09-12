#include <iostream>
#include <string>
#include "env_nvm.h"

namespace rocksdb {

NVMWritableFile::NVMWritableFile(void) : WritableFile() {
  NVM_DEBUG("\n");
}

NVMWritableFile::NVMWritableFile(const std::string& fname,
                                 const EnvOptions& options) : WritableFile()
{
  NVM_DEBUG("fname(%s), options(?)\n", fname.c_str());
}

NVMWritableFile::~NVMWritableFile(void) {
  NVM_DEBUG("\n", fname.c_str());
}

bool NVMWritableFile::UseOSBuffer(void) const
{
  NVM_DEBUG("\n");

  return false;
}

Status NVMWritableFile::Append(const Slice& data)
{
  NVM_DEBUG("data(?)\n");

  return Status::IOError();
}

Status NVMWritableFile::Close(void)
{
  NVM_DEBUG("\n");

  return Status::IOError();
}

Status NVMWritableFile::PositionedAppend(const Slice& data, uint64_t offset)
{
  NVM_DEBUG("data(?), offset(%d)\n", offset);

  return Status::IOError();
}

Status NVMWritableFile::Truncate(uint64_t size)
{
  NVM_DEBUG("size(%d)\n", size);

  return Status::IOError();
}

Status NVMWritableFile::Flush(void)
{
  NVM_DEBUG("\n");

  return Status::IOError();
}

Status NVMWritableFile::Sync(void)
{
  NVM_DEBUG("\n");

  return Status::IOError();
}

Status NVMWritableFile::Fsync(void)
{
  NVM_DEBUG("\n");

  return Status::IOError();
}


bool NVMWritableFile::IsSyncThreadSafe(void) const
{
  NVM_DEBUG("\n");

  return true;
}


bool NVMWritableFile::UseDirectIO(void) const
{
  NVM_DEBUG("\n");

  return true;
}

size_t NVMWritableFile::GetUniqueId(char* id, size_t max_size) const
{
  NVM_DEBUG("id(%s), max_size(%d)\n", id, max_size);

  return true;
}

Status NVMWritableFile::InvalidateCache(size_t offset, size_t length)
{
  NVM_DEBUG("offset(%d), length(%d)\n", offset, length);

  return Status::IOError();;
}

Status NVMWritableFile::RangeSync(uint64_t offset, uint64_t nbytes)
{
  NVM_DEBUG("offset(%d), nbytes(%d)\n", offset, nbytes);

  return Status::IOError();
}

void NVMWritableFile::PrepareWrite(size_t offset, size_t len)
{
  NVM_DEBUG("offset(%d), len(%d)\n", offset, len);
}

Status NVMWritableFile::Allocate(uint64_t offset, uint64_t len)
{
  NVM_DEBUG("offset(%d), len(%d)\n", offset, len);
  return Status::IOError();
}

}       // namespace rocksdb

