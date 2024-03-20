#include "parser.h"

using namespace ArgumentParser;

//Change name of Parser
ArgParser::ArgParser(const std::string& new_name) {
    name = new_name;
    AddFlag('c', "create");
    AddStringArgument('f', "file");
    AddStringArgument("filenames").Positional();
    AddFlag('l', "list");
    AddFlag('x', "extract");
    AddFlag('a', "append");
    AddFlag('d', "delete");
    AddFlag('A', "concatenate");
}

//Output Description
bool ArgParser::HelpCreate() {
    help_text = help_text + name + '\n';

    for (int g = 0; g < arguments.size(); g++) {
        HelpMessage& arg = *reinterpret_cast <HelpMessage*>(arguments[g]);
        if (arg.type_of_class == 3) {
            help_text = help_text + arg.description + '\n' + '\n';
        }
    }

    for (int g = 0; g < arguments.size(); g++) {
        StringArgument& arg = *reinterpret_cast <StringArgument*>(arguments[g]);
        if (arg.type_of_class == 0) {
            if (arg.short_argument != '\0') {
                help_text = help_text + "-" + arg.short_argument + ", --" + arg.long_argument + "=<string>, " + arg.description;
            } else {
                help_text = help_text + "--" + arg.long_argument + "=<string>, " + arg.description;
            }
            if (arg.min_args_count > 0) {
                help_text = help_text + " [Minimum number of arguments:" + std::to_string(arg.min_args_count) + "]";
            }
            if (arg.is_default != 0) {
                help_text = help_text + " [Default value: " + arg.values[0] + "]" + '\n';
            } else {
                help_text += '\n';
            }
        }
    }

    for (int g = 0; g < arguments.size(); g++) {
        FlagArgument& arg = *reinterpret_cast <FlagArgument*>(arguments[g]);
        if (arg.type_of_class == 2) {
            if (arg.short_argument != '\0') {
                help_text = help_text + "-" + arg.short_argument + ", --" + arg.long_argument + " " + arg.description;
            } else {
                help_text = help_text + "--" + arg.long_argument + " " + arg.description;
            }  
            if (arg.is_default != 0) {
                help_text = help_text + " [Default value:" + std::to_string(arg.values[0]) + "]" + '\n';
            } else {
                help_text += '\n';
            }  
        }
    }

    for (int g = 0; g < arguments.size(); g++) {
        IntArgument& arg = *reinterpret_cast <IntArgument*>(arguments[g]);
        if (arg.type_of_class == 1) {
            if (arg.short_argument != '\0') {
                help_text = help_text + "-" + arg.short_argument + ", --" + arg.long_argument + "=<int>, " + arg.description;
            } else {
                help_text = help_text + "--" + arg.long_argument + "=<int>, " + arg.description;
            }
            if (arg.min_args_count > 0) {
                help_text = help_text + " [Minimum number of arguments:" + std::to_string(arg.min_args_count) + "]";
            }
            if (arg.is_default != 0) {
                help_text = help_text + " [Default value:" + std::to_string(arg.values[0]) + "]" + '\n';
            } else {
                help_text += '\n';
            }
        }
    }

    for (int g = 0; g < arguments.size(); g++) {
        HelpMessage& arg = *reinterpret_cast <HelpMessage*>(arguments[g]);
        if (arg.short_argument != '\0') {
            help_text = help_text + '\n' + "-" + arg.short_argument + ", --" + arg.long_argument + " Display this help and exit" + '\n';
        } else {
            help_text = help_text + '\n' + "--" + arg.long_argument + " Display this help and exit" + '\n';
        }
    }

    return 1;
}

bool ArgParser::CheckValidArg() {
    for (const auto& arg : arguments) {
        StringArgument& argument = *reinterpret_cast <StringArgument*>(arg);
        if (argument.type_of_class == 0 && argument.min_args_count > 0) {
            std::cerr << "Incorrect number of arguments entered";
            return 0;
        }
    }
    for (const auto& arg : arguments) {
        IntArgument& argument = *reinterpret_cast <IntArgument*>(arg);
        if (argument.type_of_class == 1 && argument.min_args_count > 0) {
            std::cerr << "Incorrect number of arguments entered";
            return 0;
        }
    }

    for (const auto& arg : arguments) {
        StringArgument& argument = *reinterpret_cast <StringArgument*>(arg);
        if (argument.type_of_class == 0 && argument.values.size() == 0 && argument.is_positional == false) {
            std::cerr << "Argument without value";
            return 0;
        }
    }

    for (const auto& arg : arguments) {
        IntArgument& argument = *reinterpret_cast <IntArgument*>(arg);
        if (argument.type_of_class == 1 && argument.values.size() == 0) {
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
       
        for (int j = 0; j < arguments.size(); j++) { // Help Description
            HelpMessage& arg = *reinterpret_cast <HelpMessage*>(arguments[j]);
            if (arg.type_of_class == 3) {
                short_arg.assign(1, arg.short_argument);
                if (a[i].length() > 1 && (arg.long_argument == a[i].substr(kCntExtraMin) || short_arg == a[i].substr(1,1))) {
                    HelpCreate();
                    is_true_arg = 1;
                    return 1;
                }
            }
        }

        if (strtol(a[i].substr(a[i].find("=") + 1).c_str(), NULL, 10) == 0) {
            for (int j = 0; j < arguments.size(); j++) { // String Args
                StringArgument& arg = *reinterpret_cast <StringArgument*>(arguments[j]);
                if (arg.type_of_class == 0) {
                    if (a[i].length() > 1 && arg.is_positional == 0) { // Check Positional
                        short_arg.assign(1, arg.short_argument);
                        if (arg.long_argument == a[i].substr(kCntExtraMin, a[i].find("=") - kCntExtraMin)) {
                            StringArgProcess(a[i], arg, true);
                            is_true_arg = 1;
                            break;
                        } else if (a[i].length() > 1 && short_arg == a[i].substr(1,1)) {
                            StringArgProcess(a[i], arg, true);
                            is_true_arg = 1;
                            break;
                        }
                    } else {
                        bool chk = false;
                        for (int g = 0; g < arguments.size(); g++) {
                            FlagArgument& arg2 = *reinterpret_cast <FlagArgument*>(arguments[g]);
                            short_arg.assign(1, arg2.short_argument);
                            if (((arg2.type_of_class == 2) && ((arg2.long_argument == a[i].substr(kCntExtraMin)) || (short_arg == a[i].substr(1))))) {
                                chk = true;
                            }
                        }
                        if (chk == false) {
                            StringArgProcess(a[i], arg, false);
                            is_true_arg = 1;
                            break;
                        }
                    }
                }
            }
        } else {
            for (int j = 0; j < arguments.size(); j++) { // Int Args
                IntArgument& arg = *reinterpret_cast <IntArgument*>(arguments[j]);
                if (arg.type_of_class == 1) {
                    if (arg.is_positional == 0) { // Check Positional
                        short_arg.assign(1, arg.short_argument);
                        if (a[i].length() > 1 && arg.long_argument == a[i].substr(kCntExtraMin, a[i].find("=") - kCntExtraMin)) {
                            IntArgProcess(a[i], arg, true);
                            is_true_arg = 1;
                            break;
                        } else if (a[i].length() > 1 && short_arg == a[i].substr(1,1)) {
                            IntArgProcess(a[i], arg, true);
                            is_true_arg = 1;
                            break;
                        }
                    } else {
                        IntArgProcess(a[i], arg, false);
                        is_true_arg = 1;
                        break;
                    }
                }
            }
        }

        for (int j = 0; j < arguments.size(); j++) { // Bool Args
            FlagArgument& arg = *reinterpret_cast <FlagArgument*>(arguments[j]);
            if (arg.type_of_class == 2) {
                short_arg.assign(1, arg.short_argument);
                if (a[i].length() > 1 && arg.long_argument == a[i].substr(kCntExtraMin)) {
                    BoolArgProcess(a[i], arg);
                    is_true_arg = 1;
                    break;
                }
                if (short_arg == a[i].substr(1, 1)) {
                    BoolArgProcess(a[i], arg);
                    is_true_arg = 1;
                    break;
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
    arguments.push_back(new StringArgument{new_param});
    StringArgument& arg = *reinterpret_cast <StringArgument*>(arguments.back());

    return arg;
}

StringArgument& ArgParser::AddStringArgument(const char short_param, const std::string& new_param) {
    arguments.push_back(new StringArgument{short_param, new_param});
    StringArgument& arg = *reinterpret_cast <StringArgument*>(arguments.back());

    return arg;
}

StringArgument& ArgParser::AddStringArgument(const char short_param, const std::string& new_param, const std::string& new_description) {
    arguments.push_back(new StringArgument{short_param, new_param, new_description});
    StringArgument& arg = *reinterpret_cast <StringArgument*>(arguments.back());

    return arg;
}

std::string ArgParser::GetStringValue(const std::string& param) {
    for (int i = 0; i < arguments.size(); i++) {
        StringArgument& arg = *reinterpret_cast <StringArgument*>(arguments[i]);
        if (arg.type_of_class == 0) {
            if (arg.long_argument == param || std::to_string(arg.short_argument) == param) {
                return arg.values[0];
            }
        }
    }

    std::cerr << "Argument " << param << " is absent" << '\n';
    exit (EXIT_FAILURE);
}

std::vector<std::string> ArgParser::GetAllStringValues(const std::string& param) {
    for (int i = 0; i < arguments.size(); i++) {
        StringArgument& arg = *reinterpret_cast <StringArgument*>(arguments[i]);
        if (arg.type_of_class == 0) {
            if (arg.long_argument == param || std::to_string(arg.short_argument) == param) {
                return arg.values;
            }
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
    arguments.push_back(new IntArgument{new_param});
    IntArgument& arg = *reinterpret_cast <IntArgument*>(arguments.back());

    return arg;
}

IntArgument& ArgParser::AddIntArgument(const char short_param, const std::string& new_param) {
    arguments.push_back(new IntArgument{short_param, new_param});
    IntArgument& arg = *reinterpret_cast <IntArgument*>(arguments.back());

    return arg;
}

IntArgument& ArgParser::AddIntArgument(const std::string& new_param1, const std::string& new_description) {
    arguments.push_back(new IntArgument{new_param1, new_description});
    IntArgument& arg = *reinterpret_cast <IntArgument*>(arguments.back());

    return arg;
}

IntArgument& ArgParser::AddIntArgument(const char short_param, const std::string& new_param, const std::string& new_description) {
    arguments.push_back(new IntArgument{short_param, new_param, new_description});
    IntArgument& arg = *reinterpret_cast <IntArgument*>(arguments.back());

    return arg;
}

int ArgParser::GetIntValue(const std::string& param) {
    for (int i = 0; i < arguments.size(); i++) {
        IntArgument& arg = *reinterpret_cast <IntArgument*>(arguments[i]);
        if (arg.type_of_class == 1) {
            if (arg.long_argument == param || std::to_string(arg.short_argument) == param) {
                return arg.values[0];
            }
        }
    }

    std::cerr << "Argument " << param << " is absent" << '\n';
    exit (EXIT_FAILURE);
}

int ArgParser::GetIntValue(const std::string& param, int num) {
    for (int i = 0; i < arguments.size(); i++) {
        IntArgument& arg = *reinterpret_cast <IntArgument*>(arguments[i]);
        if (arg.type_of_class == 1) {
            if (arg.long_argument == param || std::to_string(arg.short_argument) == param) {
                return arg.values[num];
            }
        }
    }

    std::cerr << "Argument " << param << " is absent " << '\n';
    exit (EXIT_FAILURE);
}

//Metods Int
IntArgument& IntArgument::Default(int value) {
    values.push_back(value);
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
    arguments.push_back(new FlagArgument{short_param, new_param});
    FlagArgument& arg = *reinterpret_cast <FlagArgument*>(arguments.back());

    return arg;
}

FlagArgument& ArgParser::AddFlag(const std::string& new_param, const std::string& new_description) {
    arguments.push_back(new FlagArgument{new_param, new_description});
    FlagArgument& arg = *reinterpret_cast <FlagArgument*>(arguments.back());

    return arg;
}

FlagArgument& ArgParser::AddFlag(char short_param, const std::string& new_param, const std::string& new_description){
    arguments.push_back(new FlagArgument{short_param, new_param, new_description});
    FlagArgument& arg = *reinterpret_cast <FlagArgument*>(arguments.back());

    return arg;
}

bool ArgParser::GetFlag(const std::string& param){
    for (int i = 0; i < arguments.size(); i++) {
        FlagArgument& arg = *reinterpret_cast <FlagArgument*>(arguments[i]);
        if (arg.type_of_class == 2) {
            if (arg.long_argument == param || std::to_string(arg.short_argument) == param) {
                if (arg.values.size() != 0) {
                    return arg.values[0];
                } else {
                    return 0;
                }
            }
        }
    }

    std::cerr << "Argument " << param << " is absent" << '\n';
    return 0;
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

FlagArgument& FlagArgument::Positional() {
    is_positional = true;

    return *this;
}

//Helper
AllArgs& ArgParser::AddHelp(const char short_param, const std::string& new_param, const std::string& help_description) {
    arguments.push_back(new FlagArgument{short_param, new_param, help_description});

    return *arguments.back();
}

std::string ArgParser::HelpDescription() {
    if (arguments.size() != 0) {
        HelpCreate();
        return help_text;
    } else {
        std::string warning = "There is no description of the program";
        return warning;
    }
}

bool ArgParser::Help() {
    if (arguments.size() != 0) {
        std::cout << help_text;
        return 1;
    } else {
        std::cout << "There is no description of the program";
        return 0;
    }
}
/////////////////////////////////////////////////////////////