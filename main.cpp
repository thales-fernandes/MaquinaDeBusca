#include <iostream>
#include "maquinadebusca.hpp"
#include <set>
#include <map>

using namespace std;

int main() {
  set<string> query;
  string k="";
  while(cin >> k){
    normalizar(k);
    query.insert(k);
  }
  buscar(query);
}
