#include "file_storage.h"
#include <iostream>



void test_file_storage() {
  ara::core::InstanceSpecifier ins("test");
  auto ret = ara::per::OpenFileStorage(ins);
  if(!ret.HasValue()) { 
    std::cout << "OpenFileStorage return error " << ret.Error().Message() << "\n";
    return;
  }
  auto fs = ret.Value();
  auto uh_ret = fs->OpenFileReadOnly("test_file");
  if(!uh_ret.HasValue()) {
    std::cout << "OpenFileReadOnly return error " << uh_ret.Error().Message() << "\n";
    return;
  }
  // auto uh = std::move(uh_ret.Value());
  auto text_ret = uh_ret.Value()->ReadText();
  if(!text_ret.HasValue()) {
    std::cout << "ReadText return error " << text_ret.Error().Message() << "\n";
    return;
  }
  auto text = text_ret.Value();
  std::cout << "read content : " << text << "\n";

  return;
}


int main () {
  std::cout << "=======================test per==============================\n";
  test_file_storage();
  return 0;
}