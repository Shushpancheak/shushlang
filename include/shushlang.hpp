#pragma once
#include "shush-logs.hpp"
#include "shush-dump.hpp"

namespace shush {
namespace lang {

enum Errc {
  NO_ERROR            = -2,
  ASSERTION_FAILED    = -1,
  NO_FILE_NAME_GIVEN  = 0,
  COULD_NOT_OPEN_FILE = 1,
  FILE_SIZE_TOO_BIG   = 2
};

/**
 * A class specifically made for compiling .shushasm files to shush byte code
 */
class ShushasmCompiler {
public:

private:
  void Ok();
  const char* GetDumpMessage(int errc);

};

/**
 * A class made specifically for compiling .shush files to shush byte code
 */
class ShushCompiler {
public:

private:
  void Ok();
  const char* GetDumpMessage(int errc);

};

}
}