#pragma once

#include <algorithm>
#include <cinttypes>
#include <iostream>
#include <vector>
#include <string>

class AllArgs {
    public:
        std::string description;
        char short_argument = '\0';
        std::string long_argument;
        int min_args_count = 0;
        bool is_positional = false;
        bool is_default = false;
};

class StringArgument : public AllArgs {
    public:
        int type_of_class = 0;
        std::vector<std::string> values;
        std::string* origin = nullptr;

        StringArgument() = default;

        StringArgument(const std::string& new_param) {
            long_argument = new_param;
        }

        StringArgument(const char short_param, const std::string& new_param) {
            short_argument = short_param;
            long_argument = new_param;
        }

        StringArgument(const char short_param, const std::string& new_param, const std::string& new_description) {
            short_argument = short_param;
            long_argument = new_param;
            description = new_description;
        }

        StringArgument& Default(const std::string& value);
        StringArgument& StoreValue(std::string& value);
        StringArgument& MultiValue(int args_count);
        StringArgument& Positional();
};

class IntArgument : public AllArgs {
    public:
        int type_of_class = 1;
        std::vector<int> values;
        int* origin = nullptr;
        std::vector<int>* origins = nullptr;

        IntArgument() = default;

        IntArgument(const std::string& new_param) {
            long_argument = new_param;
        }

        IntArgument(const char short_param, const std::string& new_param) {
            short_argument = short_param;
            long_argument = new_param;
        }

        IntArgument(const std::string& new_param1, const std::string& new_param2) {
            long_argument = new_param1;
            description = new_param2;
        }

        IntArgument(const char short_param, const std::string& new_param, const std::string& new_description) {
            short_argument = short_param;
            long_argument = new_param;
            description = new_description;
        }

        IntArgument& Default(int value);
        IntArgument& StoreValue(int& value);
        IntArgument& StoreValues(std::vector<int>& value);
        IntArgument& MultiValue();
        IntArgument& MultiValue(int args_count);
        IntArgument& Positional();
};

class FlagArgument : public AllArgs {
    public:
        int type_of_class = 2;
        std::vector<bool> values;
        bool* origin = nullptr;
        bool is_default = false;

        FlagArgument() = default;

        FlagArgument(const char short_param, const std::string& new_param) {
            short_argument = short_param;
            long_argument = new_param;
        }

        FlagArgument(const std::string& new_param, const std::string& new_description) {
            long_argument = new_param;
            description = new_description;
        }

        FlagArgument(const char short_param, const std::string& new_param, const std::string& new_description) {
            short_argument = short_param;
            long_argument = new_param;
            description = new_description;
        }

        FlagArgument& Default(bool value);
        FlagArgument& StoreValue(bool& value);
        FlagArgument& Positional();
};

class HelpMessage : public AllArgs {
    public:
        int type_of_class = 3;

        HelpMessage(const char short_param, const std::string& new_param, const std::string& help_description) {
            short_argument = short_param;
            long_argument = new_param;
            description = help_description;
        }
};

namespace ArgumentParser {

class ArgParser {
    public:
        std::vector <std::string> file_names;

        ArgParser(const std::string& new_name);

        void StringArgProcess(const std::string& text_push, StringArgument& str_argument, bool check);
        void IntArgProcess(const std::string& text_push, IntArgument& int_argument, bool check);
        void BoolArgProcess(const std::string& text_push, FlagArgument& bool_argument);
        bool Parse(std::vector<std::string> a);
        bool Parse(int argc, char** argv);
        bool CheckValidArg();

        bool Help();
        bool HelpCreate();
        std::string HelpDescription();
        AllArgs& AddHelp(const char short_param, const std::string& new_param, const std::string& help_description);
        
        StringArgument& AddStringArgument(const std::string& new_param);
        StringArgument& AddStringArgument(const char short_param, const std::string& new_param);
        StringArgument& AddStringArgument(const char short_param, const std::string& new_param, const std::string& description);
        std::string GetStringValue(const std::string& param);
        std::vector<std::string> GetAllStringValues(const std::string& param);

        IntArgument& AddIntArgument(const std::string& new_param);
        IntArgument& AddIntArgument(const char short_param, const std::string& new_param);
        IntArgument& AddIntArgument(const std::string& new_param1, const std::string& new_param2);
        IntArgument& AddIntArgument(const char short_param, const std::string& new_param, const std::string& new_param2);
        int GetIntValue(const std::string& param);
        int GetIntValue(const std::string& param, int num);

        FlagArgument& AddFlag(char short_param, const std::string& new_param);
        FlagArgument& AddFlag(const std::string& new_param1, const std::string& new_param2);
        FlagArgument& AddFlag(char short_param, const std::string& new_param1, const std::string& new_param2);
        bool GetFlag(const std::string& param);

    private:
        std::vector<AllArgs*> arguments;

        std::string name = "Prog";
        std::string help_text;
};

} // namespace ArgumentParser