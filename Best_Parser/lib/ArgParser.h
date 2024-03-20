#pragma once

#include <algorithm>
#include <cinttypes>
#include <iostream>
#include <vector>
#include <string>

class StringArgument {
    public:
        std::vector<std::string> values;
        std::string description;
        char short_argument = '\0';
        std::string long_argument;
        std::string* origin = nullptr;
        int min_args_count = 0;
        bool is_positional = false;
        bool is_default = false;

        StringArgument& Default(const std::string& value);
        StringArgument& StoreValue(std::string& value);
        StringArgument& MultiValue(int args_count);
        StringArgument& Positional();
};

class IntArgument {
    public:
        std::vector<int> values;
        std::string description;
        char short_argument = '\0';
        std::string long_argument;
        int* origin = nullptr;
        std::vector<int>* origins = nullptr;
        int min_args_count = 0;
        bool is_positional = false;
        bool is_default = false;

        IntArgument& Default(const std::string& value);
        IntArgument& StoreValue(int& value);
        IntArgument& StoreValues(std::vector<int>& value);
        IntArgument& MultiValue();
        IntArgument& MultiValue(int args_count);
        IntArgument& Positional();
};

class FlagArgument {
    public:
        std::vector<bool> values;
        char short_argument = '\0';
        std::string description;
        std::string long_argument;
        bool* origin = nullptr;
        bool is_default = false;

        FlagArgument& Default(bool value);
        FlagArgument& StoreValue(bool& value);
};

class HelpMessage {
    public:
        char short_argument = '\0';
        std::string long_argument;
        std::string description;
};

namespace ArgumentParser {

class ArgParser {
    public:
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
        HelpMessage& AddHelp(const char short_param, const std::string& new_param, const std::string& help_description);
        
        StringArgument& AddStringArgument(const std::string& new_param);
        StringArgument& AddStringArgument(const char short_param, const std::string& new_param);
        StringArgument& AddStringArgument(const char short_param, const std::string& new_param, const std::string& description);
        std::string GetStringValue(const std::string& param);

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
        std::string name = "Prog";

        std::string help_text;

        std::vector<StringArgument> str_arguments;

        std::vector<IntArgument> int_arguments;

        std::vector<FlagArgument> bool_arguments;

        std::vector<HelpMessage> help_arguments;
};

} // namespace ArgumentParser
