#include <iostream>
#include <string>
#include "env_nvm.h"

namespace rocksdb {

NVMWritableFile::NVMWritableFile(void) : WritableFile() {
  NVM_DEBUG("\n");
}

NVMWritableFile::NVMWritableFile(
  NVMFile *file,
  const EnvOptions& options
) : WritableFile() {
  NVM_DEBUG("fname(%p), options(?)\n", file);
}

NVMWritableFile::~NVMWritableFile(void) {
  NVM_DEBUG("\n");
}

bool NVMWritableFile::UseOSBuffer(void) const
{
  NVM_DEBUG("\n");

  return true;
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
  NVM_DEBUG("data(?), offset(%lu)\n", offset);

  return Status::IOError();
}

Status NVMWritableFile::Truncate(uint64_t size)
{
  NVM_DEBUG("size(%lu)\n", size);

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
  NVM_DEBUG("id(%s), max_size(%lu)\n", id, max_size);

  return true;
}

size_t NVMWritableFile::GetRequiredBufferAlignment(void) const {
  NVM_DEBUG("\n");

  return 0;
}

void NVMWritableFile::SetIOPriority(Env::IOPriority pri) {
  NVM_DEBUG("pri(%d)\n", pri);
}

Env::IOPriority NVMWritableFile::GetIOPriority(void) {
  NVM_DEBUG("\n");

  return io_priority_;
}

uint64_t NVMWritableFile::GetFileSize(void) {
  NVM_DEBUG("\n");

  return file_->GetFileSize();
}

Status NVMWritableFile::InvalidateCache(size_t offset, size_t length)
{
  NVM_DEBUG("offset(%lu), length(%lu)\n", offset, length);

  return Status::IOError();;
}

Status NVMWritableFile::RangeSync(uint64_t offset, uint64_t nbytes)
{
  NVM_DEBUG("offset(%lu), nbytes(%lu)\n", offset, nbytes);

  return Status::IOError();
}

void NVMWritableFile::PrepareWrite(size_t offset, size_t len)
{
  NVM_DEBUG("offset(%lu), len(%lu)\n", offset, len);
}

Status NVMWritableFile::Allocate(uint64_t offset, uint64_t len)
{
  NVM_DEBUG("offset(%lu), len(%lu)\n", offset, len);
  return Status::IOError();
}

}       // namespace rocksdb

