#include "shushlang.hpp"


int main(int argc, char** argv) {
  try {
  std::cout << "shushlang version: " << shush::lang::VERSION << std::endl;
  shush::lang::CommonCompiler cc;
  cc.StartCompiling(argc, argv);

  } catch (shush::dump::Dump& dump) {
    shush::dump::HandleFinalDump(dump);
  }
  return 0;
}


shush::lang::ShushasmCompiler::ShushasmCompiler(const char* file_name) {
  strcpy(this->file_name, file_name);

  file::File file(file_name, "r");
  const size_t file_size = file.GetFileSize();
  text = new char[file_size];

  file.Read(text, file_size);
}


shush::lang::ShushasmCompiler::~ShushasmCompiler() {
  delete[] text;
}


void shush::lang::ShushasmCompiler::Compile() {
  ChangeExtension(file_name, "shushexe");
  file::File compiled_file(file_name, "wb");

  // TODO make it custom and real map.

  // contains name of a label and byte id where it points to
  Label labels    [LABELS_MAX_COUNT] {};
  size_t labels_count = 0;
  // contains name of a label and the place where it should be substituted.
  Label label_ref [LABELS_REFS_MAX_COUNT] {};
  size_t label_res_count = 0;
  // if == 0, then last character was of non command nature.
  size_t command_start = 0;
  // Command and arguments buffer in executable form.
  char command_buffer[COMMAND_MAX_SIZE];
  // End ptr for strtol.
  char* end_ptr = nullptr;
  // Tmp arguments.
  int64_t val = 0;
  uint8_t reg = 0;

  // First pass
  for (size_t i = 0; text[i] != '\0'; ++i) {
    if (command_start == 0 &&
        (isalpha(text[i]) || text[i] == '<' ||
         text[i] == '>'   || text[i] == '=')) {
      command_start = i;
    } else if (command_start != 0 && text[i] == ' ') {
      char command[COMMAND_MAX_SIZE] {};
      memcpy(command, text + command_start, i - command_start);

      if (!strcmp(command, "push")) {
        end_ptr = text + i + 1;
        val = strtoll(text + i + 1, &end_ptr, 10);
        i = end_ptr - text;

        command_buffer[0] = PUSH;
        memcpy(command_buffer + 1, &val, sizeof(int64_t));

        compiled_file.Write(command_buffer, 1 + sizeof(int64_t));
      } else if (!strcmp(command, "pop")) {
        end_ptr = text + i + 1;
        reg = strtoll(text + i + 1, &end_ptr, 10);
        i = end_ptr - text;

        command_buffer[0] = POP;
        memcpy(command_buffer + 1, &reg, sizeof(uint8_t));

        compiled_file.Write(command_buffer, 1 + sizeof(uint8_t));
      } else if (!strcmp(command, "add")) {
        //end_ptr = text + i + 1;
        //reg = strtoll(text + i + 1, &end_ptr, 10);
        //i = end_ptr - text;
        //
        //command_buffer[0] = POP;
        //memcpy(command_buffer + 1, &reg, sizeof(uint8_t));
        //
        //compiled_file.Write(command_buffer, 1 + sizeof(uint8_t));
      }
    }
  }
}


void shush::lang::ShushasmCompiler::Ok() {
  return;
}


const char* shush::lang::ShushasmCompiler::GetDumpMessage(int errc) {
  return nullptr;
}


const char* shush::lang::ShushasmCompiler::GetErrorName(int errc) {
  return shush::lang::GetErrorName(errc);
}


shush::lang::ShushCompiler::ShushCompiler(const char* file_name) {
  // TODO
  text = new char[1];
}


shush::lang::ShushCompiler::~ShushCompiler() {
  delete[] text;
}


void shush::lang::ShushCompiler::Compile() {
  //TODO
  return;
}


void shush::lang::ShushCompiler::Ok() {
  return;
}


const char* shush::lang::ShushCompiler::GetDumpMessage(int errc) {
  return nullptr;
}


const char* shush::lang::ShushCompiler::GetErrorName(int errc) {
  return shush::lang::GetErrorName(errc);
}


void shush::lang::CommonCompiler::Ok() {
  return;
}


const char* shush::lang::CommonCompiler::GetDumpMessage(int errc) {
  return nullptr;
}


const char* shush::lang::CommonCompiler::GetErrorName(int errc) {
  return shush::lang::GetErrorName(errc);
}


void shush::lang::CommonCompiler::StartCompiling(int argc, char** argv) {
  UMASSERT(argc > 1, NO_FILE_NAME_GIVEN);

  const char* extension = GetExtension(argv[1]);

  UMASSERT(extension && (extension == "shush" || extension == "shushasm"), UNKNOWN_EXTENSION);

  if (!strcmp(extension, "shush")) {
    ShushCompiler sh_c(argv[1]);
    sh_c.Compile();
  } else if (!strcmp(extension, "shushasm")) {
    ShushasmCompiler shasm_c(argv[1]);
    shasm_c.Compile();
  }
}


char* shush::lang::GetExtension(char* file_name) {
  for (int i = static_cast<int>(strlen(file_name)) - 1; i >= 0; --i) {
    if (file_name[i] == '.') {
      return file_name + i;
    }
  }
  return nullptr;
}


const char* shush::lang::GetErrorName(int errc) {
  switch (errc) {
    case NO_FILE_NAME_GIVEN : {
      strcpy(dump_error_name_buffer, "No file name given");
      break;
    }
    case UNKNOWN_EXTENSION : {
      strcpy(dump_error_name_buffer, "The extension of the file provided is unknown to the compiler");
      break;
    }
    default : {
      strcpy(dump_error_name_buffer, "Unknown error");
    }
  }

  return dump_error_name_buffer;
}


void shush::lang::ChangeExtension(char* file_name, const char* new_ext) {
  char* extension_pos = GetExtension(file_name);
  strcpy(extension_pos, new_ext);
}
