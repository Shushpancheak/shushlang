#!/usr/bin/python

file = open("commands.inc", "w")
constants_file = open("command_consts.inc", "w")

def new_command(command_name, command_code, types_of_args):
    cmd_buffer_adding = ""
    cmd_cur_arg_pos   = "" # consists of sizeofs

    for arg_type in types_of_args:
        if (arg_type == 'double'):
            cmd_buffer_adding += "memcpy(command_buffer + 1 {0}, &strtod(text + i + 1 {0}, &end_ptr, 10), sizeof(double));\n".format(cmd_cur_arg_pos)
            cmd_cur_arg_pos += " + sizeof(double)"
        elif (arg_type == 'register'):
            cmd_buffer_adding += "char reg[3] {};\n"
            cmd_buffer_adding += "         memcpy(reg, text + i + 1 {}, 3);\n".format(cmd_cur_arg_pos)
            cmd_buffer_adding += "         char reg_n = GetRegistryByteCode(reg);\n         UMEASSERT(reg_n != -1, error_pos = i, UNKNOWN_REGISTRY);\n"
            cmd_buffer_adding += "         memcpy(command_buffer + 1 {0}, &reg_n, sizeof(char));\n".format(cmd_cur_arg_pos)
            cmd_cur_arg_pos += " + 3"
        elif (arg_type == 'size_t'):
            cmd_buffer_adding += "memcpy(command_buffer + 1 {0}, &strtoul(text + i + 1 {0}, &end_ptr, 10), sizeof(size_t));\n".format(cmd_cur_arg_pos)
            cmd_cur_arg_pos += " + sizeof(size_t)"
        elif (arg_type == 'label'):
            cmd_buffer_adding += "memcpy(label_ref[label_refs_count].str, text + i + 1 {0}, GetEndOfWordInText(text + i + 1 {0}) - i - 1);\n".format(cmd_cur_arg_pos)
            cmd_buffer_adding += "         label_ref[label_refs_count++].byte_id = compiled_file.GetCurrentFilePos() + 1 {};\n".format(cmd_cur_arg_pos)
            # Skipping sizeof(size_t) bytes
            cmd_buffer_adding += "         memcpy(command_buffer + 1 {0}, &0UL, sizeof(size_t));\n".format(cmd_cur_arg_pos)
            cmd_cur_arg_pos += " + sizeof(size_t)"
        else:
            raise Exception("Unknown arg type: {}".format(arg_type))

    file.write(
       """ else if (!strcmp(command, \"{0}\")) {{
         command_buffer[0] = {1};
         {2}
         compiled_file.Write(command_buffer, 1 {3});
         i += 0{3};
}}""".format(command_name, command_code, cmd_buffer_adding, cmd_cur_arg_pos)
    )

    constants_file.write("""
    inline const char {}_CODE = {};""".format(command_name.upper(), command_code))

file.write("if (false) {}\n")

new_command("push", 0x00, ["double"])
new_command("pop",  0x01, ["register"])
new_command("add",  0x10, [])
new_command("sub",  0x11, [])
new_command("div",  0x12, [])
new_command("mod",  0x13, [])
new_command("sqrt", 0x14, [])
new_command("goto", 0x20, ["label"])
new_command("in",   0x30, [])
new_command("out",  0x31, [])
new_command("end",  0x40, [])

file.write(""" else {
    UEASSERT(true, error_pos = i, UNKNOWN_COMMAND);
}""")

file.close()
constants_file.close()