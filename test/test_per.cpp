#include "file_storage.h"
#include <iostream>


void test_file_storage_func(ara::per::SharedHandle<ara::per::FileStorage> fs) {
  auto file = fs->OpenFileReadWrite("file");
  if(file.HasValue()) {
    file.Value()->WriteText("test content");
    // fflush to disk, so GetSize() can get right size
    file.Value()->SyncToFile();
    file.Value()->SetPosition(0);
    auto text = file.Value()->ReadText();
    if(text.HasValue()) {
      std::cout << "file text : " <<  text.Value().size() << text.Value() << "\n";
    }
  }
  std::cout << "=======================  FileExists ==============================\n";
  {
    auto it = fs->FileExists("file");
    if(it.HasValue()) {
      std::cout << "file is exist " << it.Value() << "\n";
    } else {
      std::cout << "FileExists error " << it.Error().Message() << "\n";
    }
  }
  std::cout << "=======================  GetCurrentFileSize ==============================\n";
  {
    auto it = fs->GetCurrentFileSize("file");
    if(it.HasValue()) {
      std::cout << "file size " << it.Value() << "\n";
    } else {
      std::cout << "GetCurrentFileSize error " << it.Error().Message() << "\n";
    }
  }
  std::cout << "=======================  GetFileInfo ==============================\n";
  {
    auto it = fs->GetFileInfo("file");
    if(it.HasValue()) {
      std::cout << "file info " 
                << "\ncreationTime : " << it.Value().creationTime 
                << "\naccessTime : " << it.Value().accessTime 
                << "\nmodificationTime : " << it.Value().modificationTime << "\n";
    } else {
      std::cout << "FileExists error " << it.Error().Message() << "\n";
    }
  }
  std::cout << "=======================  DeleteFile ==============================\n";
  {
    auto it = fs->DeleteFile("file");
    if(it.HasValue()) {
      std::cout << "file is delete " << "\n";
    } else {
      std::cout << "FileExists error " << it.Error().Message() << "\n";
    }
  }
}


void test_file_storage(ara::core::String s) {
  ara::core::InstanceSpecifier ins(s);
  auto ret = ara::per::OpenFileStorage(ins);
  if(!ret.HasValue()) { 
    std::cout << "OpenFileStorage return error " << ret.Error().Message() << "\n";
    return;
  }
  
  auto fs = ret.Value();
  test_file_storage_func(fs);
  std::cout << "=======================  OpenFileReadWrite ==============================\n";
  auto uh_rw = fs->OpenFileWriteOnly("test_file");
  if(!uh_rw.HasValue()) {
    std::cout << "OpenFileReadWrite return error " << uh_rw.Error().Message() << "\n";
    return;
  }
  auto text_write = uh_rw.Value()->WriteText("OpenFileReadWrite write test");
  if(!text_write.HasValue()) {
    std::cout << "WriteText return error " << text_write.Error().Message() << "\n";
  }
  uh_rw.Value()->SyncToFile();
  uh_rw.Value()->SetPosition(0);
  auto text_write_r = uh_rw.Value()->ReadText();
  if(!text_write_r.HasValue()) {
    std::cout << "ReadText return error " << text_write_r.Error().Message() << "\n";
    return;
  }
  std::cout << "text_write_r content : " << text_write_r.Value() << "\n";
  std::cout << "=======================  OpenFileReadOnly ==============================\n";
  auto uh_read = fs->OpenFileReadOnly("test_file");
  if(!uh_read.HasValue()) {
    std::cout << "OpenFileReadOnly return error " << uh_read.Error().Message() << "\n";
    return;
  }
  // auto uh = std::move(uh_read.Value());
  auto text_read = uh_read.Value()->ReadText();
  if(!text_read.HasValue()) {
    std::cout << "ReadText return error " << text_read.Error().Message() << "\n";
    return;
  }
  auto text = text_read.Value();
  std::cout << "read content : " << text << "\n";

  return;
}




int main () {
  std::cout << "======================= fs : test ==============================\n";
  test_file_storage("test");
  std::cout << "======================= fs : /mid/p1 ==============================\n";
  test_file_storage("/mid/p1");
  return 0;
}