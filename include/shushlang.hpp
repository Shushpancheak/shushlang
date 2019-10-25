#pragma once
#include "shush-logs.hpp"
#include "shush-dump.hpp"
#include "shush-file.hpp"

namespace shush {
namespace lang {

inline const std::string VERSION = "0.0.2";

static char dump_error_name_buffer[100] {};
static char dump_error_msg_buffer[100] {};

enum Errc {
  NO_FILE_NAME_GIVEN,
  UNKNOWN_EXTENSION,
  UNKNOWN_COMMAND,
  UNKNOWN_REGISTRY,
  LABEL_DOUBLE_DECLARATION,
  LABEL_UNKNOWN_REFERENCE,
  START_OF_A_WORD_OUT_OF_BOUNDS
};

enum {
  LABEL_STR_SIZE = 50,
  LABELS_MAX_COUNT = 100,
  LABELS_REFS_MAX_COUNT = 500,
  COMMAND_MAX_SIZE = 50
};

struct Label {
  char str[LABEL_STR_SIZE] {};
  size_t byte_id;
};

// Consists of {code_name}_CODE constants.
#include "command_consts.inc"

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

  /**
   * @return byte code of a register.
   * @return -1 on error.
   */
  char GetRegistryByteCode(char* reg_str);

  /**
   * Get line of position of a char in text. Used in finding a line number
   * on which a error occured.
   */
  size_t GetLineOfPosInText();

  /**
   * Finds the end (pos of symbol after the word) of a word with given start
   * position in text[] array.
   */
  size_t GetEndOfWordInText(size_t start);

  /**
   * Copies a word form text to buf.
   * @return position of element after the end of the word.
   * @note makes the whitespace as '\0' after the end of the word.
   */
  size_t CopyWord(char* buf, size_t start);

private:
  char* text;
  /**
   * input file.
   */
  char file_name[50] {};
  size_t file_size;

  // TODO make it custom and real map.
  // contains name of a label and byte id where it points to
  Label label    [LABELS_MAX_COUNT] {};
  size_t labels_count = 0;
  // contains name of a label and the place where it should be substituted.
  Label label_ref [LABELS_REFS_MAX_COUNT] {};
  size_t label_refs_count = 0;
  // if == 0, then last character was of non command nature.
  int64_t command_start = -1;
  // Copied command
  char command[COMMAND_MAX_SIZE] {};
  // Command and arguments buffer in executable form.
  char command_buffer[COMMAND_MAX_SIZE] {};

  size_t error_pos_ = 0;
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