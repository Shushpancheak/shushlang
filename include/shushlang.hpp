#pragma once
#include "shush-logs.hpp"
#include "shush-dump.hpp"
#include "shush-file.hpp"

namespace shush {
namespace lang {

inline const std::string VERSION = "0.0.1";

static char dump_error_name_buffer[70];

static inline const char PUSH = 0x00;
static inline const char POP  = 0x01;
static inline const char ADD  = 0x10;

enum Errc {
  NO_FILE_NAME_GIVEN,
  UNKNOWN_EXTENSION
};

enum {
  LABEL_STR_SIZE = 50,
  LABELS_MAX_COUNT = 100,
  LABELS_REFS_MAX_COUNT = 500,
  COMMAND_MAX_SIZE = 10
};

struct Label {
  char str[LABEL_STR_SIZE] {};
  size_t byte_id;
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
  /**
   * input at first and output later.
   */
  char file_name[50];
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
};

static char* GetExtension(char* file_name);
static const char* GetErrorName(int errc);
static void ChangeExtension(char* file_name, const char* new_ext);

}
}