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
  file_size = file.GetFileSize();
  text = new char[file_size];
  end_ptr = text;

  file.Read(text, file_size);
}


shush::lang::ShushasmCompiler::~ShushasmCompiler() {
  delete[] text;
}


void shush::lang::ShushasmCompiler::Compile() {
  ChangeExtension(file_name, "shushexe");
  file::File compiled_file(file_name, "rwb");

  // First pass, not referencing labels.
  for (size_t i = 0; i < file_size; ++i) {
    // command_start == 0 -- when last char was whitespace.
    if (command_start == 0 && !isspace(text[i])) {
      command_start = i;
    } else if (command_start != 0 && isspace(text[i])) {
      UEASSERT(i - command_start > COMMAND_MAX_SIZE, error_pos_ = i, 
               UNKNOWN_COMMAND);
      memcpy(command, text + command_start, i - command_start);

      // Checking if it is a label
      if (command[i - 1] == ':') {
        command[i - 1] = '\0';
        strcpy(label[labels_count].str, command);
        label[labels_count++].byte_id = compiled_file.GetCurrentFilePos();
        continue;
      }

      #include "commands.inc"
    } else if (isspace(text[i])) {
      command_start = 0;
    }
  }

  // Second pass, filling in addresses of labels
  // For these purposes, let's replace all null references
  // to actual label references
  for (size_t i = 0; i < label_refs_count; ++i) {
    bool found = false;
    for (size_t j = 0; j < labels_count; ++j) {
      if (!strcmp(label[j].str, label_ref[i].str)) {
        UEASSERT(!found, error_pos_ = label[j].byte_id, 
                 LABEL_DOUBLE_DECLARATION);
        found = true;
        compiled_file.SetFilePos(label_ref[i].byte_id);
        compiled_file.Write(reinterpret_cast<char*>(&label[j].byte_id), sizeof(size_t));
      }
    }

    UEASSERT(found, error_pos_ = label_ref[i].byte_id, LABEL_UNKNOWN_REFERENCE);
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


char shush::lang::ShushasmCompiler::GetRegistryByteCode(char* reg_str) {
  if (!strcmp(reg_str, "rax")) {
    return 0;
  } else if (!strcmp(reg_str, "rbx")) {
    return 1;
  } else if (!strcmp(reg_str, "rcx")) {
    return 3;
  } else if (!strcmp(reg_str, "rdx")) {
    return 4;
  } else {
    return -1;
  }
}


size_t shush::lang::ShushasmCompiler::GetLineOfPosInText() {
  MASSERT(text, -1);

  size_t lines_count = 0;
  for (size_t i = 0; i <= error_pos_; ++i) {
    if (text[i] == '\n') {
      ++lines_count;
    }
  }

  return lines_count;
}


size_t shush::lang::ShushasmCompiler::GetEndOfWordInText(size_t start) {
  for (size_t i = start; ; ++i) {
    if (isspace(text[i])) {
      return i;
    }
  }
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

  UMASSERT(extension, UNKNOWN_EXTENSION);

  if (!strcmp(extension, "shush")) {
    ShushCompiler sh_c(argv[1]);
    sh_c.Compile();
  } else if (!strcmp(extension, "shushasm")) {
    ShushasmCompiler shasm_c(argv[1]);
    shasm_c.Compile();
  } else {
    UMASSERT(false, UNKNOWN_EXTENSION);
  }
}


char* shush::lang::GetExtension(char* file_name) {
  for (int i = static_cast<int>(strlen(file_name)) - 1; i >= 0; --i) {
    if (file_name[i] == '.') {
      return file_name + i + 1;
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
    case UNKNOWN_COMMAND : {
      strcpy(dump_error_name_buffer, "Unknown command was encountered");
      break;
    }
    case UNKNOWN_REGISTRY : {
      strcpy(dump_error_name_buffer, "Unknown registry was encountered");
      break;
    }
    default : {
      strcpy(dump_error_name_buffer, "Unknown error");
      break;
    }
  }

  return dump_error_name_buffer;
}


void shush::lang::ChangeExtension(char* file_name, const char* new_ext) {
  char* extension_pos = GetExtension(file_name);
  strcpy(extension_pos, new_ext);
}
