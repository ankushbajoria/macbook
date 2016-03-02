#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

int main() {
  string s1 = "ankush";
  cout << "s1:" << s1 << endl;
  std::string s2(std::move(s1));
  cout << "s1:" << s1 << endl;


  std::vector<std::string> v;
  v.push_back(s2);
  v.push_back("bajoria");

  for (auto & s : v) {
    std::cout << s << std::endl;
  }

  return 0;
}
