#!/usr/bin/python

file = open("commands.inc", "w")
constants_file = open("command_consts.inc", "w")

def new_command(command_name, command_code, types_of_args):
    cmd_buffer_adding = "char arg[COMMAND_MAX_SIZE]; size_t new_i = 0;\n         " if len(types_of_args) > 0 else ""
    cmd_cur_arg_pos   = "" # consists of sizeofs

    for arg_type in types_of_args:
        cmd_buffer_adding += "new_i = CopyWord(arg, i);\n         " # new_i == ind of character after end of arg.

        if (arg_type == 'double'):
            cmd_buffer_adding += "double val = strtod(arg, nullptr); memcpy(command_buffer + 1 {0}, &val, sizeof(double));\n".format(cmd_cur_arg_pos)
            cmd_cur_arg_pos += " + sizeof(double)"
        elif (arg_type == 'register'):
            cmd_buffer_adding += "char reg_n = GetRegistryByteCode(arg);\n         UEASSERT(reg_n != -1, error_pos_ = i, UNKNOWN_REGISTRY);\n"
            cmd_buffer_adding += "         memcpy(command_buffer + 1 {0}, &reg_n, sizeof(char));\n".format(cmd_cur_arg_pos)
            cmd_cur_arg_pos += " + sizeof(char)"
        elif (arg_type == 'size_t'):
            cmd_buffer_adding += "memcpy(command_buffer + 1 {0}, &strtoul(arg, nullptr, 10), sizeof(size_t));\n".format(cmd_cur_arg_pos)
            cmd_cur_arg_pos += " + sizeof(size_t)"
        elif (arg_type == 'label'): # TODO isolate memcpys
            cmd_buffer_adding += "strcpy(label_ref[label_refs_count].str, arg);\n"
            cmd_buffer_adding += "         label_ref[label_refs_count++].byte_id = compiled_file.GetCurrentFilePos() + 1 {};\n".format(cmd_cur_arg_pos)
            # Skipping sizeof(size_t) bytes
            cmd_buffer_adding += "         size_t zero = 0; memcpy(command_buffer + 1 {0}, &zero, sizeof(size_t));\n".format(cmd_cur_arg_pos)
            cmd_cur_arg_pos += " + sizeof(size_t)"
        else:
            raise Exception("Unknown arg type: {}".format(arg_type))

        cmd_buffer_adding += "         i = new_i + 1;\n"

    if len(types_of_args) > 0:
        # So that the i in the end pointed to whitespace before
        # the next command, and `for` could increment it by itself.
        cmd_buffer_adding += "         --i;\n"

    file.write(""" else if (!strcmp(command, \"{0}\")) {{
         command_buffer[0] = {1};
         {2}
         compiled_file.Write(command_buffer, 1 {3});
}}""".format(command_name, command_code, cmd_buffer_adding, cmd_cur_arg_pos)
    )

    constants_file.write("""
    inline const char {}_CODE = {};""".format(command_name.upper(), command_code))

file.write("if (false) {}\n")

new_command("push",   0x00, ["double"])
new_command("pop",    0x01, ["register"])
new_command("add",    0x10, [])
new_command("sub",    0x11, [])
new_command("mul",    0x12, [])
new_command("div",    0x13, [])
new_command("mod",    0x14, [])
new_command("sqrt",   0x15, [])
new_command("goto",   0x20, ["label"])
new_command("in",     0x30, [])
new_command("out",    0x31, [])
new_command("end",    0x40, [])
new_command("gotoeq", 0x50, ["label"])
new_command("gotoge", 0x51, ["label"])
new_command("gotole", 0x52, ["label"])
new_command("gotog",  0x53, ["label"])
new_command("gotol",  0x54, ["label"])
new_command("store",  0x60, ["register"])

file.write(""" else {
    UEASSERT(false, error_pos_ = i, UNKNOWN_COMMAND);
}""")

file.close()
constants_file.close()
