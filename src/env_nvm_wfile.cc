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
) : WritableFile(), file_(file) {
  NVM_DEBUG("file(%p), dname(%s), fname(%s), options(?)\n",
            file, file->GetDir().c_str(), file->GetName().c_str());
  file_->Ref();
}

NVMWritableFile::~NVMWritableFile(void) {
  NVM_DEBUG("\n");
  file_->Unref();
}

Status NVMWritableFile::Close(void)
{
  NVM_DEBUG("forwarding\n");
  return file_->Close();
}

bool NVMWritableFile::UseOSBuffer(void) const
{
  NVM_DEBUG("forwarding\n");
  return file_->UseOSBuffer();
}

Status NVMWritableFile::Append(const Slice& data)
{
  NVM_DEBUG("forwarding\n");
  return file_->Append(data);
}

Status NVMWritableFile::PositionedAppend(const Slice& data, uint64_t offset)
{
  NVM_DEBUG("forwarding\n");
  return file_->PositionedAppend(data, offset);
}

Status NVMWritableFile::Truncate(uint64_t size)
{
  NVM_DEBUG("forwarding\n");
  return file_->Truncate(size);
}

Status NVMWritableFile::Flush(void)
{
  NVM_DEBUG("forwarding\n");
  return file_->Flush();
}

Status NVMWritableFile::Sync(void)
{
  NVM_DEBUG("forwarding\n");
  return file_->Sync();
}

Status NVMWritableFile::Fsync(void)
{
  NVM_DEBUG("forwarding\n");
  return file_->Fsync();
}

bool NVMWritableFile::IsSyncThreadSafe(void) const
{
  NVM_DEBUG("forwarding\n");
  return file_->IsSyncThreadSafe();
}

bool NVMWritableFile::UseDirectIO(void) const
{
  NVM_DEBUG("forwarding\n");
  return file_->UseDirectIO();
}

size_t NVMWritableFile::GetUniqueId(char* id, size_t max_size) const
{
  NVM_DEBUG("forwarding\n");
  return file_->GetUniqueId(id, max_size);
}

size_t NVMWritableFile::GetRequiredBufferAlignment(void) const {
  NVM_DEBUG("forwarding\n");
  return file_->GetRequiredBufferAlignment();
}

void NVMWritableFile::SetIOPriority(Env::IOPriority pri) {
  NVM_DEBUG("pri(%d)\n", pri);
}

Env::IOPriority NVMWritableFile::GetIOPriority(void) {
  NVM_DEBUG("%d\n", io_priority_);
  return io_priority_;
}

uint64_t NVMWritableFile::GetFileSize(void) {
  NVM_DEBUG("forwarding\n");
  return file_->GetFileSize();
}

Status NVMWritableFile::InvalidateCache(size_t offset, size_t length) {
  NVM_DEBUG("forwarding\n");
  return file_->InvalidateCache(offset, length);
}

Status NVMWritableFile::RangeSync(uint64_t offset, uint64_t nbytes)
{
  NVM_DEBUG("forwarding\n");
  return file_->RangeSync(offset, nbytes);;
}

void NVMWritableFile::PrepareWrite(size_t offset, size_t len)
{
  NVM_DEBUG("forwarding\n");
  file_->PrepareWrite(offset, len);
}

Status NVMWritableFile::Allocate(uint64_t offset, uint64_t len)
{
  NVM_DEBUG("offset(%lu), len(%lu)\n", offset, len);

  return file_->Allocate(offset, len);
}

}       // namespace rocksdb

