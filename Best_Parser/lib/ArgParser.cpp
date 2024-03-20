#include "ArgParser.h"

using namespace ArgumentParser;

//Change name of Parser
ArgParser::ArgParser(const std::string& new_name) {
    name = new_name;
}

//Output Description
bool ArgParser::HelpCreate() {
    help_text = help_text + name + '\n';
    help_text = help_text + help_arguments[0].description + '\n' + '\n';

    for (int g = 0; g < str_arguments.size(); g++) {
        if (str_arguments[g].short_argument != '\0') {
            help_text = help_text + "-" + str_arguments[g].short_argument + ", --" + str_arguments[g].long_argument + "=<string>, " + str_arguments[g].description;
        } else {
            help_text = help_text + "--" + str_arguments[g].long_argument + "=<string>, " + str_arguments[g].description;
        }
        if (str_arguments[g].min_args_count > 0) {
            help_text = help_text + " [Minimum number of arguments:" + std::to_string(str_arguments[g].min_args_count) + "]";
        }
        if (str_arguments[g].is_default != 0) {
            help_text = help_text + " [Default value:" + str_arguments[g].values[0] + "]" + '\n';
        } else {
            help_text += '\n';
        }
    }

    for (int g = 0; g < bool_arguments.size(); g++) {
        if (bool_arguments[g].short_argument != '\0') {
            help_text = help_text + "-" + bool_arguments[g].short_argument + ", --" + bool_arguments[g].long_argument + " " + bool_arguments[g].description;
        } else {
            help_text = help_text + "--" + bool_arguments[g].long_argument + " " + bool_arguments[g].description;
        }  

        if (bool_arguments[g].is_default != 0) {
            help_text = help_text + " [Default value:" + std::to_string(bool_arguments[g].values[0]) + "]" + '\n';
        } else {
            help_text += '\n';
        }  
    }

    for (int g = 0; g < int_arguments.size(); g++) {
        if (int_arguments[g].short_argument != '\0') {
            help_text = help_text + "-" + int_arguments[g].short_argument + ", --" + int_arguments[g].long_argument + "=<int>, " + int_arguments[g].description;
        } else {
            help_text = help_text + "--" + int_arguments[g].long_argument + "=<int>, " + int_arguments[g].description;
        }
        if (int_arguments[g].min_args_count > 0) {
            help_text = help_text + " [Minimum number of arguments:" + std::to_string(int_arguments[g].min_args_count) + "]";
        }
        if (int_arguments[g].is_default != 0) {
            help_text = help_text + " [Default value:" + std::to_string(int_arguments[g].values[0]) + "]" + '\n';
        } else {
            help_text += '\n';
        }
    }

    if (help_arguments[0].short_argument != '\0') {
        help_text = help_text + '\n' + "-" + help_arguments[0].short_argument + ", --" + help_arguments[0].long_argument + " Display this help and exit" + '\n';
    } else {
        help_text = help_text + '\n' + "--" + help_arguments[0].long_argument + " Display this help and exit" + '\n';
    }

    return 1;
}

bool ArgParser::CheckValidArg() {
    for (const auto& arg : str_arguments) {
        if (arg.min_args_count > 0) {
            std::cerr << "Incorrect number of arguments entered";
            return 0;
        }
    }
    for (const auto& arg : int_arguments) {
        if (arg.min_args_count > 0) {
            std::cerr << "Incorrect number of arguments entered";
            return 0;
        }
    }

    for (const auto& arg : str_arguments) {
        if (arg.values.size() == 0) {
            std::cerr << "Argument without value";
            return 0;
        }
    }
    for (const auto& arg : int_arguments) {
        if (arg.values.size() == 0) {
            std::cerr << "Argument without value";
            return 0;
        }
    }

    return 1;
}

void ArgParser::StringArgProcess(const std::string& text_push, StringArgument& str_argument, bool check) {
    if (check == true) {
        str_argument.values.push_back(text_push.substr(text_push.find("=") + 1));
        if (str_argument.origin != nullptr) {
            *str_argument.origin = str_argument.values[0];
        }
        str_argument.min_args_count--;
    } else {
        str_argument.values.push_back(text_push);
        if (str_argument.origin != nullptr) {
            *str_argument.origin = str_argument.values[0];
        }
        str_argument.min_args_count--;
    }
}

void ArgParser::IntArgProcess(const std::string& text_push, IntArgument& int_argument, bool check) {
    if (check == true) {
        int_argument.values.push_back(stoi(text_push.substr(text_push.find("=") + 1)));
        if (int_argument.origin != nullptr) {
            *int_argument.origin = int_argument.values[0];
        }
        if (int_argument.origins != nullptr) {
            *int_argument.origins = int_argument.values;
        }
        int_argument.min_args_count--;
    } else {
        int_argument.values.push_back(stoi(text_push));
        if (int_argument.origin != nullptr) {
            *int_argument.origin = int_argument.values[0];
        }
        if (int_argument.origins != nullptr) {
            *int_argument.origins = int_argument.values;
        }
        int_argument.min_args_count--;
    }
}

void ArgParser::BoolArgProcess(const std::string& text_push, FlagArgument& bool_argument) {
    bool_argument.values.push_back(true);
    if (bool_argument.origin != nullptr) {
        *bool_argument.origin = bool_argument.values[0];
    }
}

//Parse Arguments for tests
bool ArgParser::Parse(std::vector<std::string> a) {
    const int16_t kCntExtraMin = 2;

    for (int i = 1; i < a.size(); i++) {
        bool is_true_arg = 0;
        std::string short_arg;

        for (int j = 0; j < help_arguments.size(); j++) { // Help Description
            short_arg.assign(1, help_arguments[j].short_argument);
            if (a[i].length() > 1 && (help_arguments[j].long_argument == a[i].substr(kCntExtraMin) || short_arg == a[i].substr(1,1))) {
                HelpCreate();
                is_true_arg = 1;
                return 1;
            }
        }
        
        if (strtol(a[i].substr(a[i].find("=") + 1).c_str(), NULL, 10) == 0) {
            for (int j = 0; j < str_arguments.size(); j++) { // String Args
                if (a[i].length() > 1 && str_arguments[j].is_positional == 0) { // Check Positional
                    short_arg.assign(1, str_arguments[j].short_argument);
                    if (str_arguments[j].long_argument == a[i].substr(kCntExtraMin, a[i].find("=") - kCntExtraMin)) {
                        StringArgProcess(a[i], str_arguments[j], true);
                        is_true_arg = 1;
                        break;
                    } else if (a[i].length() > 1 && short_arg == a[i].substr(1,1)) {
                        StringArgProcess(a[i], str_arguments[j], true);
                        is_true_arg = 1;
                        break;
                    }
                } else {
                    StringArgProcess(a[i], str_arguments[j], false);
                    is_true_arg = 1;
                    break;
                }
            }
        } else {
            for (int j = 0; j < int_arguments.size(); j++) { // Int Args
                if (int_arguments[j].is_positional == 0) { // Check Positional
                    short_arg.assign(1, int_arguments[j].short_argument);
                    if (a[i].length() > 1 && int_arguments[j].long_argument == a[i].substr(kCntExtraMin, a[i].find("=") - kCntExtraMin)) {
                        IntArgProcess(a[i], int_arguments[j], true);
                        is_true_arg = 1;
                        break;
                    } else if (a[i].length() > 1 && short_arg == a[i].substr(1,1)) {
                        IntArgProcess(a[i], int_arguments[j], true);
                        is_true_arg = 1;
                        break;
                    }
                } else {
                    IntArgProcess(a[i], int_arguments[j], false);
                    is_true_arg = 1;
                    break;
                }
            }
        }

        for (int j = 0; j < bool_arguments.size(); j++) { // Bool Args
            short_arg.assign(1, bool_arguments[j].short_argument);
            if (a[i].length() > 1 && bool_arguments[j].long_argument == a[i].substr(kCntExtraMin)) {
                BoolArgProcess(a[i], bool_arguments[j]);
                is_true_arg = 1;
                break;
            }
            if (a[i].length() > 1 && a[i].substr(1).length() >= 1) { //Multi args in one arg
                for (int g = 1; g <= a[i].substr(1).length(); g++) {
                    if (short_arg == a[i].substr(g, 1)) {
                        BoolArgProcess(a[i], bool_arguments[j]);
                        is_true_arg = 1;
                        break;
                    } 
                }
            }   
        }
        
        if (is_true_arg == 0) {
            std::cerr << "Invalid argument";
            return 0;
        }
    }
    
    return CheckValidArg();
}

//Parse Arguments for human
bool ArgParser::Parse(int argc, char** argv) {
    std::vector <std::string> a;

    for (int i = 0; i < argc; i++) {
        a.push_back(static_cast<std::string> (argv[i]));
    }

    return Parse(a);
}

//String Arguments
StringArgument& ArgParser::AddStringArgument(const std::string& new_param) {
    StringArgument pars_arr;
    pars_arr.long_argument = new_param;
    str_arguments.push_back(pars_arr);

    return str_arguments.back();
}

StringArgument& ArgParser::AddStringArgument(const char short_param, const std::string& new_param) {
    StringArgument pars_arr;
    pars_arr.short_argument = short_param;
    pars_arr.long_argument = new_param;
    str_arguments.push_back(pars_arr);

    return str_arguments.back();
}

StringArgument& ArgParser::AddStringArgument(const char short_param, const std::string& new_param, const std::string& description) {
    StringArgument pars_arr;
    pars_arr.short_argument = short_param;
    pars_arr.long_argument = new_param;
    pars_arr.description = description;
    str_arguments.push_back(pars_arr);

    return str_arguments.back();
}

std::string ArgParser::GetStringValue(const std::string& param) {
    for (int i = 0; i < str_arguments.size(); i++) {
        if (str_arguments[i].long_argument == param || std::to_string(str_arguments[i].short_argument) == param) {
            return str_arguments[i].values[0];
        }
    }

    std::cerr << "Argument " << param << " is absent" << '\n';
    exit (EXIT_FAILURE);
}

//Metods String
StringArgument& StringArgument::Default(const std::string& value) {
    values.push_back(value);
    is_default = true;
    
    return *this;
}

StringArgument& StringArgument::StoreValue(std::string& value) {
    origin = &value;

    return *this;
}

StringArgument& StringArgument::MultiValue(int args_count) {
    min_args_count = args_count;

    return *this;
}

StringArgument& StringArgument::Positional() {
    is_positional = true;

    return *this;
}

//Int Arguments
IntArgument& ArgParser::AddIntArgument(const std::string& new_param) {
    IntArgument pars_arr;
    pars_arr.long_argument = new_param;
    int_arguments.push_back(pars_arr);

    return int_arguments.back();
}

IntArgument& ArgParser::AddIntArgument(const char short_param, const std::string& new_param) {
    IntArgument pars_arr;
    pars_arr.short_argument = short_param;
    pars_arr.long_argument = new_param;
    int_arguments.push_back(pars_arr);

    return int_arguments.back();
}

IntArgument& ArgParser::AddIntArgument(const std::string& new_param1, const std::string& new_param2) {
    IntArgument pars_arr;
    pars_arr.long_argument = new_param1;
    pars_arr.description = new_param2;
    int_arguments.push_back(pars_arr);

    return int_arguments.back();
}

int ArgParser::GetIntValue(const std::string& param) {
    for (int i = 0; i < int_arguments.size(); i++) {
        if (int_arguments[i].long_argument == param || std::to_string(int_arguments[i].short_argument) == param) {
            return int_arguments[i].values[0];
        }
    }

    std::cerr << "Argument " << param << " is absent" << '\n';
    exit (EXIT_FAILURE);
}

int ArgParser::GetIntValue(const std::string& param, int num) {
    for (int i = 0; i < int_arguments.size(); i++) {
        if (int_arguments[i].long_argument == param || std::to_string(int_arguments[i].short_argument) == param) {
            return int_arguments[i].values[num];
        }
    }

    std::cerr << "Argument " << param << " is absent" << '\n';
    exit (EXIT_FAILURE);
}

//Metods Int
IntArgument& IntArgument::Default(const std::string& value) {
    values.push_back(stoi(value));
    is_default = true;
    
    return *this;
}

IntArgument& IntArgument::StoreValue(int& value) {
    origin = &value;

    return *this;
}

IntArgument& IntArgument::StoreValues(std::vector<int>& value) {
    origins = &value;

    return *this;
}

IntArgument& IntArgument::MultiValue() {
    return *this;
}

IntArgument& IntArgument::MultiValue(int args_count) {
    min_args_count = args_count;

    return *this;
}

IntArgument& IntArgument::Positional() {
    is_positional = true;

    return *this;
}

// Flag Arguments
FlagArgument& ArgParser::AddFlag(char short_param, const std::string& new_param){
    FlagArgument pars_arr;
    pars_arr.short_argument = short_param;
    pars_arr.long_argument = new_param;
    bool_arguments.push_back(pars_arr);

    return bool_arguments.back();
}

FlagArgument& ArgParser::AddFlag(const std::string& new_param1, const std::string& new_param2) {
    FlagArgument pars_arr;
    pars_arr.long_argument = new_param1;
    pars_arr.description = new_param2;
    bool_arguments.push_back(pars_arr);

    return bool_arguments.back();
}

FlagArgument& ArgParser::AddFlag(char short_param, const std::string& new_param1, const std::string& new_param2){
    FlagArgument pars_arr;
    pars_arr.short_argument = short_param;
    pars_arr.long_argument = new_param1;
    pars_arr.description = new_param2;
    bool_arguments.push_back(pars_arr);

    return bool_arguments.back();
}

bool ArgParser::GetFlag(const std::string& param){
    for (int i = 0; i < bool_arguments.size(); i++) {
        if (bool_arguments[i].long_argument == param || std::to_string(bool_arguments[i].short_argument) == param) {
            if (bool_arguments[i].values.size() != 0) {
                return bool_arguments[i].values[0];
            } else {
                std::cerr << "Argument " << param << " is empty" << '\n';
                exit (EXIT_FAILURE);
            }
        }
    }

    std::cerr << "Argument " << param << " is absent" << '\n';
    exit (EXIT_FAILURE);
}

//Flag Metods
FlagArgument& FlagArgument::Default(bool value) {
    values.push_back(value);
    is_default = true;
    
    return *this;
}

FlagArgument& FlagArgument::StoreValue(bool& value) {
    origin = &value;

    return *this;
}

//Helper
HelpMessage& ArgParser::AddHelp(const char short_param, const std::string& new_param, const std::string& help_description) {
    HelpMessage pars_arr;
    pars_arr.short_argument = short_param;
    pars_arr.long_argument = new_param;
    pars_arr.description = help_description;
    help_arguments.push_back(pars_arr);

    return help_arguments.back();
}

std::string ArgParser::HelpDescription() {
    if (help_arguments.size() != 0) {
        HelpCreate();
        return help_text;
    } else {
        std::string warning = "There is no description of the program";
        return warning;
    }
}

bool ArgParser::Help() {
    if (help_arguments.size() != 0) {
        std::cout << help_text;
        return 1;
    } else {
        std::cout << "There is no description of the program";
        return 0;
    }
}
/////////////////////////////////////////////////////////////
