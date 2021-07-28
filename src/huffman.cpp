#include <huffman.h>
#include <huffmantree.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iterator>
#include <utility>

std::vector<size_t> Huffman::symbolsCounts(std::ifstream& fin_source) {
  const size_t kByteValsCnt = 256;
  std::vector<size_t> counts(kByteValsCnt);
  for (char byte; fin_source.get(byte);) {
    ++counts[*reinterpret_cast<uint8_t*>(&byte)];
  }
  return counts;
}

void Huffman::createDict(const std::string& source_name,
                                  const std::string& dict_id,
                                  const std::string& description) {
  const size_t kByteValsCnt = 256;
  for (const auto& dir_entry : std::filesystem::recursive_directory_iterator("dictionaries")) {
    std::ostringstream sout;
    sout << dir_entry.path().filename();
    std::string curr_id = sout.str();
    curr_id.erase(curr_id.begin());
    curr_id.pop_back();
    if (dict_id == curr_id) {
      std::cerr << dict_id + " already exists, rewriting prohibited" << std::endl;
      return;
    }
  }

  std::ifstream fin_source;
  fin_source.open(source_name, std::ios::binary);
  if (!fin_source) {
    std::cerr << "Can't open source file: " << source_name << std::endl;
    return;
  }
  std::vector<size_t> counts = symbolsCounts(fin_source);
  fin_source.close();

  std::ofstream fout_dict;
  fout_dict.open("dictionaries/" + dict_id);
  if (!fout_dict) {
    std::cerr << "Can't open the dict: " + std::string("dictionaries/") + dict_id << std::endl;
    return;
  }
  for (size_t i = 0; i < counts.size(); ++i) {
    fout_dict << counts[i] << " ";
  }
  fout_dict << std::endl;
  fout_dict << description;
  fout_dict.close();
  std::cout << "The dict created successfully" << std::endl;
}

void Huffman::encode(std::ifstream& fin_source,
                            std::ofstream& fout_dest,
                            const std::vector<size_t>& counts) {
  const size_t kByteValsCnt = 256;
  HuffmanTree ht(counts);
  std::vector<std::string> transl_table = ht.translationTable();

  uint8_t out_byte = 0;
  //out_byte |= (1u << 7);
  uint8_t curr_bit = 0;
  for (std::istreambuf_iterator<char> iter(fin_source), e; iter != e; ++iter) {
    char ch = *iter;
    uint8_t in_byte = *reinterpret_cast<uint8_t*>(&ch);
    std::string code_str = transl_table[in_byte];
    for (size_t i = 0; i < code_str.size(); ++i) {
      if (7 == curr_bit) {
        fout_dest << *reinterpret_cast<char*>(&out_byte);
        out_byte = 0;
        //out_byte |= (1u << 7);
        curr_bit = 0;
      }
      char bit_char = code_str[i];
      if ('1' == bit_char) {
        uint8_t bit = 1;
        bit <<= (6 - curr_bit);
        out_byte |= bit;
      }
      ++curr_bit;
    }
  }
  //out_byte &= ~(1u << 7);
  out_byte |= (1u << 7);
  fout_dest << *reinterpret_cast<char*>(&out_byte);
  fout_dest << *reinterpret_cast<char*>(&curr_bit);
}

void Huffman::compressWithDict(const std::string& source_name,
                                const std::string& dict_id,
                                const std::string& destination_name) {
  const size_t kByteValsCnt = 256;
  std::ifstream fin_dict;
  fin_dict.open("dictionaries/" + dict_id);
  if (!fin_dict) {
      std::cerr << "Can't open the dict: " + std::string("dictionaries/") + dict_id << std::endl;
      return;
  }
  std::vector<size_t> counts(kByteValsCnt);
  for (size_t i = 0; i < counts.size(); ++i) {
    fin_dict >> counts[i];
  }
  fin_dict.close();

  std::ifstream fin_source;
  fin_source.open(source_name, std::ios::binary);
  if (!fin_source) {
    std::cerr << "Can't open source file: " << source_name << std::endl;
    return;
  }
  std::ofstream fout_dest;
  fout_dest.open(destination_name, std::ios::binary);
  if (!fout_dest) {
    std::cerr << "Can't open destination file: " << destination_name << std::endl;
    return;
  }
  fout_dest << "1 " << dict_id << std::endl;
  encode(fin_source, fout_dest, counts);
  fin_source.close();
  fout_dest.close();
  std::cout << "Source file encoded successfully" << std::endl;
}

void Huffman::compress(const std::string& source_name,
                                  const std::string& destination_name) {
  std::ifstream fin_source;
  fin_source.open(source_name);
  if (!fin_source) {
    std::cerr << "Can't open source file: " << source_name << std::endl;
    return;
  }
  std::vector<size_t> counts = symbolsCounts(fin_source);
  fin_source.close();

  fin_source.open(source_name, std::ios::binary);
  if (!fin_source) {
    std::cerr << "Can't open source file: " << source_name << std::endl;
    return;
  }
  std::ofstream fout_dest;
  fout_dest.open(destination_name, std::ios::binary);
  if (!fout_dest) {
    std::cerr << "Can't open destination file: " << destination_name << std::endl;
    return;
  }
  fout_dest << "0 ";
  for (size_t i = 0; i < counts.size(); ++i) {
    fout_dest << counts[i] << " ";
  }
  fout_dest << std::endl;
  encode(fin_source, fout_dest, counts);
  fin_source.close();
  fout_dest.close();
  std::cout << "Source file encoded successfully" << std::endl;
}

void Huffman::unpacking(const std::string& source_name,
                            const std::string& destination_name) {
  const size_t kByteValsCnt = 256;
  std::ifstream fin_source;
  fin_source.open(source_name, std::ios::binary);
  if (!fin_source) {
    std::cerr << "Can't open source file: " << source_name << std::endl;
    return;
  }
  std::string line;
  std::getline(fin_source, line);

  std::istringstream sin;
  sin.str(line);
  bool is_dict;
  sin >> is_dict;
  std::vector<size_t> counts(kByteValsCnt);
  if (is_dict) {
    std::string dict_id;
    sin >> dict_id;
    std::ifstream fin_dict;
    fin_dict.open("dictionaries/" + dict_id);
    if (!fin_dict) {
      std::cerr << "Can't open the dict: " + std::string("dictionaries/") + dict_id << std::endl;
      return;
    }
    for (size_t i = 0; i < counts.size(); ++i) {
      fin_dict >> counts[i];
    }
    fin_dict.close();
  }
  else {
    for (size_t i = 0; i < counts.size(); ++i) {
      sin >> counts[i];
    }
  }
  HuffmanTree ht(counts);

  std::ofstream fout_dest;
  fout_dest.open(destination_name, std::ios::binary);
  if (!fout_dest) {
    std::cerr << "Can't open destination file: " << destination_name << std::endl;
    return;
  }
  ht.decode(fin_source, fout_dest);
  std::cout << "File decoded successfully" << std::endl;
}

void Huffman::listOfDict() {
  for (const auto& dir_entry : std::filesystem::recursive_directory_iterator("dictionaries")) {
    std::ifstream fin;
    fin.open(dir_entry, std::ios::binary);
    if (!fin) {
      std::cerr << "Can't open dict (skipped): " << dir_entry << std::endl;
      continue;
    }
    std::ostringstream sout;
    sout << dir_entry.path().filename();
    std::string dict_id = sout.str();
    std::cout << dict_id << std::endl;
    std::string line;
    std::getline(fin, line);
    std::string descr = std::string((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    std::cout << descr << std::endl;
    fin.close();
  }
}
