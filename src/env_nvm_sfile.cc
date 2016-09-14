#include <iostream>
#include <string>
#include "env_nvm.h"

namespace rocksdb {

NVMSequentialFile::NVMSequentialFile(void) : SequentialFile() {
  NVM_DEBUG("\n");
}

NVMSequentialFile::NVMSequentialFile(
  NVMFile *file, const EnvOptions& options
) : SequentialFile(), file_(file), pos_(0) {
  NVM_DEBUG("fname(%s), file(%p)\n", file_->GetName().c_str(), file_);
  file_->Ref();
}

NVMSequentialFile::~NVMSequentialFile(void) {
  NVM_DEBUG("\n");
  file_->Unref();
}

Status NVMSequentialFile::Read(size_t n, Slice* result, char* scratch) {
  NVM_DEBUG("fname(%s), n(%lu), pos_(%lu), result(%p), scratch(%p)\n",
            file_->GetName().c_str(), n, pos_, result, scratch);

  if (pos_ >= file_->GetFileSize()) {
    *result = Slice();
    return Status::OK();
  }

  Status s = file_->Read(pos_, n, result, scratch);
  if (s.ok()) {
    pos_ += result->size();
  }

  char *huh = (char*)malloc(sizeof(*huh)*(result->size()+1));
  memcpy(huh, result->data(), result->size());
  huh[result->size()] = '\0';

  NVM_DEBUG("HERE IS WHAT I READ(%s)\n", huh);

  return s;
}

Status NVMSequentialFile::Skip(uint64_t n) {
  NVM_DEBUG("fname(%s), n(%lu)\n", file_->GetName().c_str(), n);

  if (n + pos_ > file_->GetFileSize()) {
    // TODO: Verify this boundary
    return Status::IOError("Skipping beyond end of file");
  }

  pos_ += n;

  return Status::OK();
}

Status NVMSequentialFile::InvalidateCache(size_t offset, size_t length) {
  NVM_DEBUG("Forwarding\n");

  return file_->InvalidateCache(offset, length);
}

}       // namespace rocksdb

