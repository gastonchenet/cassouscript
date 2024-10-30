#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <filesystem>
#include <chrono>

#include "lib.hpp"
#include "util/color.hpp"
#include "structures/Context.hpp"
#include "structures/SymbolTable.hpp"
#include "structures/Lexer.hpp"
#include "structures/Parser.hpp"
#include "structures/Interpreter.hpp"
#include "structures/Position.hpp"
#include "structures/interpreter/BuiltInFunction.hpp"

#define FILE_NAME "<stdin>"
#define VERSION "1.0.0"

namespace fs = std::filesystem;

struct ReturnValue
{
  std::shared_ptr<Value> value;
  std::shared_ptr<Error> error;

  ReturnValue(std::shared_ptr<Value> _value, std::shared_ptr<Error> _error)
  : value(_value), error(_error) {}
};

std::string global_file_path(std::string filename)
{
  if (filename.rfind("./", 0) == 0)
  {
    filename = filename.substr(2);
  }

  std::string path = fs::current_path().string();
  path += "/" + filename;
  return path; 
}

std::string dir_name() {
  std::string path = fs::current_path().string();
  std::string name = path.substr(path.find_last_of("/\\") + 1);
  return name;
}

std::string get_memory_usage()
{
  std::string line;
  std::string name;
  std::string value;
  std::ifstream file("/proc/self/status");

  while (std::getline(file, line))
  {
    if (line.find("VmRSS") != std::string::npos)
    {
      name = line.substr(0, line.find(":"));
      value = line.substr(line.find(":") + 1);
      value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
      value.erase(std::remove(value.begin(), value.end(), '\t'), value.end());
      break;
    }
  }

  file.close();
  return value;
}

ReturnValue run_code(const std::string& code, Context* context, bool verbose, const std::string& filename = FILE_NAME)
{
  Lexer lexer = Lexer(code, filename);
  std::shared_ptr<IllegalCharError> illegal_char_error = lexer.make_tokens();

  if (verbose)
  {
    std::cout << "Lexing" + util::color::colorize(":", util::color::black) + " " + util::color::colorize("OK", util::color::green) << std::endl;
  }

  if (illegal_char_error != nullptr)
  {
    return ReturnValue(nullptr, illegal_char_error);
  }

  if (lexer.tokens.front().match(TokenType::TT_EOF))
  {
    exit(0);
  }

  Parser parser = Parser(lexer.tokens);
  ParseResult result = parser.parse();

  // parser.print_node(&result);

  if (verbose)
  {
    std::cout << "Parsing" + util::color::colorize(":", util::color::black) + " " + util::color::colorize("OK", util::color::green) << std::endl;
  }

  if (result.error != nullptr)
  {
    return ReturnValue(nullptr, result.error);
  }

  Interpreter interpreter = Interpreter();
  RTResult return_value = interpreter.visit(result.node, context);

  if (verbose)
  {
    std::cout << "Interpreting" + util::color::colorize(":", util::color::black) + " " + util::color::colorize("OK", util::color::green) << std::endl;
  }

  return ReturnValue(return_value.value, return_value.error);
}

std::string read_file(const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Could not open the file" << std::endl;
    exit(1);
  }
  
  std::string line;
  std::string result = "";

  while (std::getline(file, line))
  {
    result += line + "\n";
  }
  
  file.close();

  return result;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }

  std::string option = argv[1];

  if (option == "--version" || option == "-v")
  {
    std::cout << "Version " << VERSION << std::endl;
    return 0;
  }
  else if (option == "--help" || option == "-h")
  {
    std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 0;
  }
  else if (option == "init")
  {
    bool auto_create = false;

    if (argc > 2)
    {
      if (std::string(argv[2]) == "--auto" || std::string(argv[2]) == "-a")
      {
        auto_create = true;
      }
    }

    std::ofstream file(global_file_path("main.cspt"));

    if (!file.is_open())
    {
      std::cerr << "Could not create the file" << std::endl;
      return 1;
    }

    std::ofstream project_file(global_file_path("project.csd"));

    if (!project_file.is_open())
    {
      std::cerr << "Could not create the file" << std::endl;
      return 1;
    }

    file << "log(\"Hello, World!\")" << std::endl;
    file.close();

    std::string name = dir_name();
    std::string version = "1.0.0";
    std::string author = "";
    std::string entry_file = "main.cspt";

    if (!auto_create)
    {
      std::cout << "Enter the project name (" + name + "): ";
      getline(std::cin, name, '\n');
      if (name.empty()) name = dir_name();

      std::cout << "Enter the project version (" + version + "): ";
      getline(std::cin, version, '\n');
      if (version.empty()) version = "1.0.0";

      std::cout << "Enter the project author: ";
      getline(std::cin, author, '\n');

      std::cout << "Enter the entry file (" + entry_file + "): ";
      getline(std::cin, entry_file, '\n');
      if (entry_file.empty()) entry_file = "main.cspt";
    }

    project_file << "\"name\" => \"" << name << "\"" << std::endl;
    project_file << "\"version\" => \"" << version << "\"" << std::endl;
    project_file << "\"author\" => \"" << author << "\"" << std::endl;
    project_file << "\"entry_file\" => \"" << entry_file << "\"" << std::endl;

    project_file.close();

    std::cout << "Initialized a new project" << std::endl;
    return 0;
  }
  else
  { 
    std::vector<std::string> exec_options = {};

    for (int i = 2; i < argc; i++)
    {
      exec_options.push_back(argv[i]);
    }

    bool verbose = std::find(exec_options.begin(), exec_options.end(), "--verbose") != exec_options.end() || std::find(exec_options.begin(), exec_options.end(), "-V") != exec_options.end();

    Context global_context = Context("<global>");
    SymbolTable symbol_table = SymbolTable();
    symbol_table.set("log", std::make_shared<BuiltInFunction>("log", &global_context));
    symbol_table.set("ask", std::make_shared<BuiltInFunction>("ask", &global_context));
    global_context.set_symbol_table(&symbol_table);

    if (option == "repl")
    {
      std::cout << "Type 'exit' to exit the repl." << std::endl;

      while (true)
      {
        std::string code;
        std::cout << util::color::colorize(">>> ", util::color::black);

        getline(std::cin, code, '\n');

        if (code == "exit")
        {
          std::cout << "Exiting..." << std::endl;
          break;
        }

        if (code.empty())
        {
          continue;
        }

        ReturnValue result = run_code(code, &global_context, verbose);
        if (result.error != nullptr)
        {
          std::cerr << result.error->to_string() << std::endl;
          continue;
        }

        std::cout << result.value->to_string() << std::endl;
        symbol_table.set("_", result.value);
      }
      
      return 0;
    }
    else
    {
      std::string filename = global_file_path(option);
      std::string code = read_file(filename);

      if (code.empty())
      {
        exit(0);
      }

      auto begin = std::chrono::high_resolution_clock::now();
      ReturnValue result = run_code(code, &global_context, verbose, filename);
      auto end = std::chrono::high_resolution_clock::now();

      if (result.error != nullptr)
      {
        std::cerr << result.error->to_string() << std::endl;
        return 1;
      }

      double delay = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
      double milliseconds = delay / 1000000;

      if (
        std::find(exec_options.begin(), exec_options.end(), "--time") != exec_options.end() ||
        std::find(exec_options.begin(), exec_options.end(), "-t") != exec_options.end()
      )
      {
        std::cout << util::color::colorize("Execution time: " + std::to_string(milliseconds) + "ms", util::color::black) << std::endl;
      }

      if (
        std::find(exec_options.begin(), exec_options.end(), "--memory") != exec_options.end() ||
        std::find(exec_options.begin(), exec_options.end(), "-m") != exec_options.end()
      )
      {
        std::cout << util::color::colorize("Memory usage: " + get_memory_usage(), util::color::black) << std::endl;
      }

      return 0;
    }
  }
}
