#include <huffman.h>

#include <iostream>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Use at least one of these args: -v, -p, -u, -l. To see details: check -h option.";
    return 0;
  }
  if (std::string("-v") == argv[1]) {
    if (5 != argc) {
      std::cerr << "Args for -v: source_name, dict_id, dict_description. To see details: check -h option.";
      return 0;
    }
    std::string source_name = argv[2];
    std::string dict_id = argv[3];
    std::string description = argv[4];
    Huffman::createDict(source_name, dict_id, description);
  }
  else if (std::string("-p") == argv[1]) {
    if (5 != argc && 4 != argc) {
      std::cerr << "Args for -p: source_name, destination_name, (dict_id - optional). To see details: check -h option.";
      return 0;
    }
    std::string source_name = argv[2];
    std::string dest_name = argv[3];
    if (5 == argc) {
      std::string dict_id = argv[4];
      Huffman::compressWithDict(source_name, dict_id, dest_name);
    }
    else if (4 == argc) {
      Huffman::compress(source_name, dest_name);
    }
  }
  else if (std::string("-u") == argv[1]) {
    if (4 != argc) {
      std::cerr << "Args for -u: source_name, destination_name. To see details: check -h option.";
      return 0;
    }
    std::string source_name = argv[2];
    std::string dest_name = argv[3];
    Huffman::unpacking(source_name, dest_name);
  }
  else if (std::string("-l") == argv[1]) {
    Huffman::listOfDict();
  }
  else if (std::string("-h") == argv[1]) {
    std::cout << "Note: to use this util the directory 'dictionaries/' should be created at the directory with exec files" << std::endl;
    std::cout << "Avaliable commands: " << std::endl;
    std::cout << "-v   args: source_file, dict_id, description (plain text as arg)\n     Description: calculates the dict with source and stores internally to program" << std::endl;
    std::cout << "     Example: .\\huffman.exe -v tests/text.txt text_dict.txt 'Voyna i mir'" << std::endl;
    std::cout << "-p   args: source_file, destination_file, [ dict_id - optional, to use this dict if arg provided ]" << std::endl;
    std::cout << "     Description: encodes source_file with dict_id if this arg provided, otherwise calculates and stores the dict for source_file to destination_file" << std::endl;
    std::cout << "     Example: .\\huffman.exe -p tests/text.txt tests/text_compr.txt text_dict.txt" << std::endl;
    std::cout << "-u   args: source_file, destination_file\n     Description: decodes source_file and stores the result to destination_file" << std::endl;
    std::cout << "     Example: .\\huffman.exe -u tests/text_compr.txt tests/text_res.txt" << std::endl;
    std::cout << "-l   args: none\n     Description: prints list of dicts saved internally to program" << std::endl;
    std::cout << "-h   args: none\n     Description: prints this page" << std::endl;
  }
  else {
    std::cerr << "Unknown arg: try -h option to list avaliable commands" << std::endl;
  }
  return 0;
}
