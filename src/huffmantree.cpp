#include <huffmantree.h>
#include <queueprarrmin.h>
#include <node.h>

#include <fstream>
#include <iterator>
#include <stdexcept>

void HuffmanTree::generateTree(const std::vector<size_t>& counts) {
  const size_t kByteValsCnt = 256;
  if (counts.size() != kByteValsCnt) {
    throw std::runtime_error("HuffmanTree::generateTree: counts.size() != 128");
  }

  QueuePrArrMin pq;
  for (size_t i = 0; i < kByteValsCnt; ++i) {
      Node* node = new Node;
      node->data = i;
      node->left = nullptr;
      node->right = nullptr;
      pq.push(std::make_pair(counts[i], node));
  }
  for (size_t i = 0; i < kByteValsCnt - 1; ++i) {
    QueuePrArrMin::KeyValType elem1 = pq.top();
    pq.pop();
    QueuePrArrMin::KeyValType elem2 = pq.top();
    pq.pop();
    size_t freq1 = elem1.first;
    size_t freq2 = elem2.first;
    Node* node1 = elem1.second;
    Node* node2 = elem2.second;

    Node* parent = new Node;
    parent->data = -1;
    parent->left = node1;
    parent->right = node2;
    pq.push(std::make_pair(freq1 + freq2, parent));
  }
  head_ = pq.top().second;
}

void HuffmanTree::translationTableDFS(std::vector<std::string>& translation_table,
                                                          Node* node,
                                                          std::string& state) {
  if (nullptr == node) {
    return;
  }
  if (nullptr == node->left && nullptr == node->right) {
    _ASSERT(-1 != node->data);
    translation_table[node->data] = state;
    return;
  }
  
  state.push_back('0');
  translationTableDFS(translation_table, node->left, state);
  state.pop_back();
  state.push_back('1');
  translationTableDFS(translation_table, node->right, state);
  state.pop_back();
}

std::vector<std::string> HuffmanTree::translationTable() {
  const size_t kByteValsCnt = 256;
  std::vector<std::string> translation_table(kByteValsCnt);
  std::string state = "";
  translationTableDFS(translation_table, head_, state);
  _ASSERT(state.empty());
  return translation_table;
}

void HuffmanTree::decode(std::ifstream& fin_source, std::ofstream& fout_dest) {
  uint8_t in_byte = 0;
  uint8_t curr_bit = 0;
  Node* curr = head_;
  for (std::istreambuf_iterator<char> iter(fin_source), e; iter != e; ++iter) {
    char ch = *iter;
    uint8_t in_byte = *reinterpret_cast<uint8_t*>(&ch);
    uint8_t end_bit = 7;
    if (in_byte >> 7) {
      ++iter;
      char ch1 = *iter;
      end_bit = *reinterpret_cast<uint8_t*>(&ch1);
    }
    for (uint8_t curr_bit = 0; curr_bit < end_bit; ++curr_bit) {
      curr = ((in_byte >> (6 - curr_bit)) & 1u) ? curr->right : curr->left;
      if (-1 != curr->data) {
        char out_byte = *reinterpret_cast<char*>(&(curr->data));
        fout_dest << out_byte;
        curr = head_;
      }
    }
  }
}
