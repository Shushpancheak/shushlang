#pragma once
#include "shush-logs.hpp"
#include "shush-dump.hpp"
#include "shush-file.hpp"

namespace shush {
namespace lang {

inline const std::string VERSION = "0.0.1";

static char dump_error_name_buffer[70];

enum Errc {
  NO_FILE_NAME_GIVEN,
  UNKNOWN_EXTENSION
};

/**
 * A class specifically made for compiling .shushasm files to shush byte code
 */
class ShushasmCompiler {
public:
  ShushasmCompiler(const char* file_name);
  ~ShushasmCompiler();

  void Compile();
  
  void Ok();
  const char* GetDumpMessage(int errc);
  const char* GetErrorName(int errc);

private:
  char* text;
};

/**
 * A class made specifically for compiling .shush files to shush byte code
 */
class ShushCompiler {
public:
  ShushCompiler(const char* file_name);
  ~ShushCompiler();

  void Compile();
  
  void Ok();
  const char* GetDumpMessage(int errc);
  const char* GetErrorName(int errc);

private:
  char* text;
};

/**
 * Call from main()
 */
struct CommonCompiler {
  void Ok();
  const char* GetDumpMessage(int errc);
  const char* GetErrorName(int errc);
  void StartCompiling(int argc, char** argv);

private:
  const char* GetExtension(const char* file_name);
};

const char* GetErrorName(int errc);

}
}