if (false) {}
 else if (!strcmp(command, "push")) {
         command_buffer[0] = 0;
         char arg[COMMAND_MAX_SIZE]; size_t new_i = 0;
         new_i = CopyWord(arg, i);
         double val = strtod(arg, nullptr); memcpy(command_buffer + 1 , &val, sizeof(double));
         i = new_i + 1;
         --i;

         compiled_file.Write(command_buffer, 1  + sizeof(double));
} else if (!strcmp(command, "pop")) {
         command_buffer[0] = 1;
         char arg[COMMAND_MAX_SIZE]; size_t new_i = 0;
         new_i = CopyWord(arg, i);
         char reg_n = GetRegistryByteCode(arg);
         UEASSERT(reg_n != -1, error_pos_ = i, UNKNOWN_REGISTRY);
         memcpy(command_buffer + 1 , &reg_n, sizeof(char));
         i = new_i + 1;
         --i;

         compiled_file.Write(command_buffer, 1  + sizeof(char));
} else if (!strcmp(command, "add")) {
         command_buffer[0] = 16;
         
         compiled_file.Write(command_buffer, 1 );
} else if (!strcmp(command, "sub")) {
         command_buffer[0] = 17;
         
         compiled_file.Write(command_buffer, 1 );
} else if (!strcmp(command, "div")) {
         command_buffer[0] = 18;
         
         compiled_file.Write(command_buffer, 1 );
} else if (!strcmp(command, "mod")) {
         command_buffer[0] = 19;
         
         compiled_file.Write(command_buffer, 1 );
} else if (!strcmp(command, "sqrt")) {
         command_buffer[0] = 20;
         
         compiled_file.Write(command_buffer, 1 );
} else if (!strcmp(command, "goto")) {
         command_buffer[0] = 32;
         char arg[COMMAND_MAX_SIZE]; size_t new_i = 0;
         new_i = CopyWord(arg, i);
         strcpy(label_ref[label_refs_count].str, arg);
         label_ref[label_refs_count++].byte_id = compiled_file.GetCurrentFilePos() + 1 ;
         size_t zero = 0; memcpy(command_buffer + 1 , &zero, sizeof(size_t));
         i = new_i + 1;
         --i;

         compiled_file.Write(command_buffer, 1  + sizeof(size_t));
} else if (!strcmp(command, "in")) {
         command_buffer[0] = 48;
         
         compiled_file.Write(command_buffer, 1 );
} else if (!strcmp(command, "out")) {
         command_buffer[0] = 49;
         
         compiled_file.Write(command_buffer, 1 );
} else if (!strcmp(command, "end")) {
         command_buffer[0] = 64;
         
         compiled_file.Write(command_buffer, 1 );
} else {
    UEASSERT(false, error_pos_ = i, UNKNOWN_COMMAND);
}