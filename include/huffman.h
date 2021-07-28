#ifndef HUFFMAN_HUFFMAN_H
#define HUFFMAN_HUFFMAN_H

#include <vector>
#include <string>

class Huffman {
  public:
    static void createDict(const std::string& path_to_text, 
                      const std::string& dict_id, 
                      const std::string& description);
    static void compressWithDict(const std::string& path_to_text,
                    const std::string& dict_id,
                    const std::string& path_to_zip);
    static void compress(const std::string& path_to_text,
                      const std::string& path_to_zip);
    static void unpacking(const std::string& path_to_zip,
                const std::string& path_to_text);
    static void listOfDict();
  private:
    static void encode(std::ifstream& fin_source,
                std::ofstream& fout_dest,
                const std::vector<size_t>& counts);
    static std::vector<size_t> symbolsCounts(std::ifstream& fin_source);
};

#endif // #endif HUFFMAN_HUFFMAN_H