#include <iostream>
#include <string>
#include "env_nvm.h"

namespace rocksdb {

NVMWritableFile::NVMWritableFile(void) : WritableFile() {}

NVMWritableFile::NVMWritableFile(const std::string& fname,
                                 const EnvOptions& options) : WritableFile()
{
  fname_ = fname;
}

NVMWritableFile::~NVMWritableFile(void) {}


bool NVMWritableFile::UseOSBuffer(void) const
{
  return false;
}

Status NVMWritableFile::Append(const Slice& data)
{
  return Status::OK();
}

Status NVMWritableFile::Close(void)
{
  return Status::OK();
}

Status NVMWritableFile::PositionedAppend(const Slice& data, uint64_t offset)
{
  return Status::OK();
}

Status NVMWritableFile::Truncate(uint64_t size)
{
  return Status::OK();
}

Status NVMWritableFile::Flush(void)
{
  return Status::OK();
}

Status NVMWritableFile::Sync(void)
{
  return Status::OK();
}

Status NVMWritableFile::Fsync(void)
{
  return Status::OK();
}


bool NVMWritableFile::IsSyncThreadSafe() const
{
  return true;
}


bool NVMWritableFile::UseDirectIO() const
{
  return true;
}

size_t NVMWritableFile::GetUniqueId(char* id, size_t max_size) const
{
  return true;
}

Status NVMWritableFile::InvalidateCache(size_t offset, size_t length)
{
  return Status::OK();;
}

Status NVMWritableFile::RangeSync(uint64_t offset, uint64_t nbytes)
{
  return Status::OK();
}

void NVMWritableFile::PrepareWrite(size_t offset, size_t len)
{
  
}

Status NVMWritableFile::Allocate(uint64_t offset, uint64_t len)
{
  return Status::OK();
}

}       // namespace rocksdb

