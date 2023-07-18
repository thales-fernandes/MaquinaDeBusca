#include <iostream>
#include <set>
#include <map>
#include <fstream>
#include <dirent.h>
#include <string>
#include <algorithm>

using namespace std; 

//Testa se o iterador no diretório encontra arquivos com nome "." ou ".."
//Estes arquivos não devem ser registrados
//Este teste é chamado pela função "guardar"
bool teste_de_anomalia(string name){
  if(name=="."||name==".."){
    return true;
  }else{
    return false;
  }
}

//Função para normalizar string
void normalizar(string& s){
    char c;
    string s2;
    //Remover caracteres que não sejam a-z A-Z
    for(int i = 0; i < s.length(); i++){   
      c = s[i];
      if((c >= 65 && c <= 90) || (c >= 97 && c <= 122)){
        s2 = s2 + c;
      }
    }
  //Converter para minúsculas
  for(int i = 0; i < s2.length(); i++){
    c = s2[i];
    if(c >= 65 && c <= 90){
      c += 32;
      s2[i] = c;
    }
  }
  s = s2;
}

//Função para guardar o nome dos documentos
set<string> guardar(){
  //String que será devolvida pela função
  set<string> nomes;
  //Estrutura primitiva para iterar pelo diretório
  DIR *dir;
  struct dirent *ent;
  if((dir = opendir ("./documentos/")) != NULL) {
    int count=0;
    while ((ent = readdir (dir)) != NULL) {
      //Estrutura para evitar registrar arquivos "." e ".."
      //Origem destes arquivos é desconhecida por nós
      if(teste_de_anomalia(ent->d_name)){
      }else{
        //Inserção do nome dos arquivos desejados
        nomes.insert(ent->d_name);
      }
    }
    closedir(dir);
  }
  return nomes;
}

//Função que mapea as palavras dos documentos
map<string,set<string>> mapear(map<string,set<string>> indice, set<string> nomes){
  ifstream fs;
  set<string> palavras;
  string l,n;
  for(auto it=nomes.begin();it!=nomes.end();it++){
    n="./documentos/"+*it;
    fs.open(n);
    palavras.clear();
    while(fs>>l){
      normalizar(l);
      palavras.insert(l);
    }
    fs.close();
    fs.clear();
    indice.insert({*it,palavras});
  }
  return indice;
}

//Função que recupera os dados
set<string> recuperar(set<string> query, map<string,set<string>> indice){
  set<string> dados;
  int o;
  //Inserindo os dados em "dados"
  //Como "dados" é um set, a ordem das strings é lexicográfica
  for(auto it=indice.begin();it!=indice.end();it++){
    if(it->second.size()<query.size()){
    }else{
      o=1;
      for(auto itr=query.begin();(itr)!=query.end();itr++){
        if(it->second.count(*itr)==0){
          o=0;
        }
      }
      if(o==1){
        dados.insert(it->first);
      }
    }
  }
  return dados;
}

//Função que imprime os dados
void imprimir(set<string> dados){
  for(auto it=dados.begin();it!=dados.end();it++){
    cout<<*it<<endl;
  }
}

//Testa se "normalizar" deleta caracteres especiais
bool teste_de_exclusividade(){
  string x="%&érato*@$&87";
  normalizar(x);
  if(x=="rato"){
    return true;
  }else{
    return false;
  }
}

//Testa se o "normalizar" normaliza letras maiúsculas como minúsculas
bool teste_de_minusculizacao(){
  string x="Rato";
  normalizar(x);
  if(x=="rato"){
    return true;
  }else{
    return false;
  }
}

//Testa se a função "mapear" retorna um mapa vazio caso o set inserido seja vazio
bool teste_de_mapeamento_vazio(){
  map<string,set<string>> index;
  set<string> names;
  index=mapear(index,names);
  if(index.size()>0){
    return false;
  }else{
    return true;
  }
}

//Testa se o set de dados retornado pela função "recuperar" é vazio caso os termos pesquisados só possuam caracteres especiais
bool teste_de_recuperacao(){
  set<string> pesquisa;
  pesquisa.insert("78326");
  pesquisa.insert("000&%");
  map<string,set<string>> index;
  set<string> names=guardar();
  index=mapear(index,names);
  set<string> data=recuperar(pesquisa,index);
  if(data.size()>0){
    return false;
  }else{
    return true;
  }
}

//Struct usado para retorno de testes
struct resultados{
  bool t1;
  bool t2;
  bool t3;
  bool t4;
  bool t0;
};

//Função que chama todos os testes
resultados testes(){
  resultados r;
  r.t1=teste_de_exclusividade();
  r.t2=teste_de_minusculizacao();
  r.t3=teste_de_mapeamento_vazio();
  r.t4=teste_de_recuperacao();
  if(r.t1 && r.t2 && r.t3 && r.t4){
    r.t0=true;
  }else{
    r.t0=false;
  }
  return r;
}

//Função principal
//Chama as demais funções caso todos os testes tenham sucesso
//O teste de anomalia é realizado pela função guardar
void buscar(set<string> query){
  resultados r=testes();
  if(r.t0){
    //Set que conterá o nome dos arquivos coletados
    set<string> nomes=guardar();
    //Map que conterá o índice de busca
    map<string,set<string>> indice;
    //Armazenando as palavras no map
    indice=mapear(indice,nomes);
    //Recuperando dados
    set<string> dados=recuperar(query,indice);
    //Imprimindo dados
    imprimir(dados);
  }else{
    cout<<"Ocorreu um erro nos testes"<<endl;
    if(!(r.t1)){
      cout<<"teste_de_exclusividade falhou"<<endl;
    }
    if(!(r.t2)){
      cout<<"teste_de_minusculizacao falhou"<<endl;
    }
    if(!(r.t3)){
      cout<<"teste_de_mapeamento_vazio falhou"<<endl;
    }
    if(!(r.t4)){
      cout<<"teste_de_recuperacao falhou"<<endl;
    }
  }
}
