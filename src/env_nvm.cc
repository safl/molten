#include <iostream>
#include <string>
#include "env_nvm.h"

namespace rocksdb {

//
//      SEQUENTIAL FILE
//
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
  // TODO: put something into result

  result->reset(new NVMSequentialFile(fname, options));

  return Status::OK();
}

Status EnvNVM::NewRandomAccessFile(const std::string& fname,
                                   unique_ptr<RandomAccessFile>* result,
                                   const EnvOptions& options) {

  //result->reset(new NVMRandomAccessFile(fname, options));
  return Status::OK();
}

Status EnvNVM::NewWritableFile(const std::string& fname,
                       unique_ptr<WritableFile>* result,
                       const EnvOptions& options) {

  //result->reset(new NVMWritableFile(fname, options));
  return Status::OK();
}

Status EnvNVM::NewDirectory(const std::string& name,
                    unique_ptr<Directory>* result) {

  //result->reset(new NVMDirectory(name));
  return Status::OK();
}

Status EnvNVM::FileExists(const std::string& fname) {

  return Status::OK();
}

Status EnvNVM::GetChildren(const std::string& dir,
                   std::vector<std::string>* result) {
  std::string foo("bar");

  result->push_back(foo);
  return Status::OK();
}

Status EnvNVM::DeleteFile(const std::string& fname) {
  return Status::OK();
}

Status EnvNVM::CreateDir(const std::string& name) {
  return Status::OK();
}

Status EnvNVM::CreateDirIfMissing(const std::string& name) {
  return Status::OK();
}

Status EnvNVM::DeleteDir(const std::string& dirname) {
  return Status::OK();
}

Status EnvNVM::GetFileSize(const std::string& f, uint64_t* s) {
  return Status::OK();
}

Status EnvNVM::GetFileModificationTime(const std::string& fname,
                               uint64_t* file_mtime) {
  return Status::OK();
}

Status EnvNVM::RenameFile(const std::string& s, const std::string& t) {
  return Status::OK();
}

Status EnvNVM::LockFile(const std::string& f, FileLock** l) {
  return Status::OK();
}

Status EnvNVM::UnlockFile(FileLock* l) {
  return Status::OK();
}

void EnvNVM::Schedule(void (*f)(void* arg), void* a, Priority pri,
              void* tag, void (*u)(void* arg)) {
  return;
}

void EnvNVM::StartThread(void (*f)(void*), void* a) {
  return;
}

Status EnvNVM::GetTestDirectory(std::string* path) {

  *path = "/tmp/test";
  return Status::OK();
}

Status EnvNVM::NewLogger(const std::string& fname,
                 shared_ptr<Logger>* result) {
  // TODO: put something into result
  return Status::OK();
}

uint64_t EnvNVM::NowMicros() {
  return 0;
}

void EnvNVM::SleepForMicroseconds(int micros) {
  return;
}

Status EnvNVM::GetHostName(char* name, uint64_t len) {
  return Status::OK();
}

Status EnvNVM::GetCurrentTime(int64_t* unix_time) {
  return Status::OK();
}

Status EnvNVM::GetAbsolutePath(const std::string& db_path,
                       std::string* output_path) {
  return Status::OK();
}

void EnvNVM::SetBackgroundThreads(int num, Priority pri) {
  return;
}

std::string EnvNVM::TimeToString(uint64_t time) {
  return "23:59";
}

void EnvNVM::IncBackgroundThreadsIfNeeded(int num, Priority pri) {
  return;
}

}       // namespace rocksdb

