#ifndef ENCODERDECODER_HUFFMANTREE_H
#define ENCODERDECODER_HUFFMANTREE_H

#include <node.h>

#include <vector>
#include <string>

class HuffmanTree {
public:
  HuffmanTree(const std::vector<size_t>& counts) {
    generateTree(counts);
  }
  ~HuffmanTree() {
    clearTreeDFS(head_);
  }

  std::vector<std::string> translationTable();
  void decode(std::ifstream& fin_source, std::ofstream& fout_dest);
private:
  void generateTree(const std::vector<size_t>& counts);
  void translationTableDFS(std::vector<std::string>& translation_table,
                           Node* node,
                           std::string& state);
  void clearTreeDFS(Node* node) {
    if (nullptr == node) {
      return;
    }
    clearTreeDFS(node->left);
    clearTreeDFS(node->right);
    delete node;
  }
  
  Node* head_;
};

#endif
