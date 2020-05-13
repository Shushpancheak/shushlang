#pragma once
#include "shush-logs.hpp"
#include "shush-dump.hpp"
#include "shush-file.hpp"
#include <unordered_map>

namespace shush {
namespace lang {

inline const std::string VERSION = "0.0.3";

inline const size_t DUMP_MESSAGE_SIZE = 100;
static char dump_error_name_buffer[DUMP_MESSAGE_SIZE] {};
static char dump_error_msg_buffer[DUMP_MESSAGE_SIZE] {};

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

struct Labels {
  size_t labels_count = 0;
  size_t label_refs_count = 0;
  // contains name of a label and byte id where it points to
  Label label     [LABELS_MAX_COUNT] {};
  // contains name of a label and the place where it should be substituted.
  Label label_ref [LABELS_REFS_MAX_COUNT] {};

  // return position after label name
  void AddReference(const char* label_name, const size_t byte_id);
  void AddLabel(const char* label_name, const size_t byte_id);
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
  void FirstPass(shush::file::File& compiled_file);
  void FillLabels(shush::file::File& compiled_file);

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
   * Adds reference to a label to labels and fills corresponding place
   * in command_buffer with 0 (command_buffer + 1).
   *
   * @return position after the label name
   */
  size_t AddLabelReference(char* command_buffer, size_t label_name_pos, const size_t byte_id);

private:
  char* text;
  /**
   * input file.
   */
  char file_name[50] {};
  size_t file_size;

  // if == 0, then last character was of non command nature.
  int64_t command_start = -1;
  // Copied command
  char command[COMMAND_MAX_SIZE] {};
  // Command and arguments buffer in executable form.
  char command_buffer[COMMAND_MAX_SIZE] {};

  Labels labels;

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