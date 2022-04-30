#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <functional>
#include <string>
#define M 3
#define ELIMINADO -3

using namespace std;



struct RecordE{
  int key;
  int cantidad;
  char anio[5] = "2022" ;


  void show_data(){
    cout<<key<<" ";
    //cout<<cantidad<<" ";
    //for(int i = 0; i < 4;++i)
      //cout<<anio[i];
    cout<<endl;
  }


};

struct RecordPage{
  RecordE r[M];
  int size;
  long next;

  void show_data() {
    for (int i = 0; i < size; i++)
      r[i].show_data();

    cout<<"El size es "<<size<<endl;
    cout<<"El next es "<<next<<endl;
    cout << endl;
  }
};

struct digitalTreeNode{
  digitalTreeNode** children;
  long page;	

  digitalTreeNode(long p){page = p;children = nullptr;}

  void createChildren(long p1,long p2){
    children = new digitalTreeNode*[2];
    children[0] = new digitalTreeNode{p1};
    children[1] = new digitalTreeNode{p2};
    page = -1;

  }

};




class ExtendibleHash {
  digitalTreeNode *root;
  string _file;
  string _index;
  hash<int> myHash;
  int global_depth;

  public:
  ExtendibleHash() {}
  ExtendibleHash(string _f,string _i,int gd):_index{_i},_file{_f},global_depth{gd} {
    fstream file2;
    file2.open(_index,ios::in|ios::out|ios::binary);

  
    if(file2.peek() != EOF){

      char buffer[256];
      char buffer2[9];
      long pag;
      root = new digitalTreeNode{-1};
  
      while(file2.peek() != EOF){
        file2.getline(buffer,256,'|');
        string temp(buffer);
        file2.read((char*)&pag,sizeof(long));
        cout<<"aaa: "<<" "<<temp << "->" << pag<<endl<<endl;
        file2.get();
        auto temp2 = root;

        
        for(int i = 0; i < temp.size();++i){
          if(temp2->children == nullptr){
            temp2->createChildren(0,0);
          }
          if(i == temp.size()-1)
              temp2->children[temp[i]-48]->page = pag;
          else
            temp2 = temp2->children[temp[i]-48];

        }

        
         
      }
      

      file2.close();



    }else{
      file2.close();
      fstream file;
      RecordPage r;
      long p1 = 0;
      long p2;
      r.size = 0;
        r.next = 0;
      file.open(_file,ios::out|ios::binary);
      file.write((char*)&r,sizeof(r));
      p2 = file.tellp();
      file.write((char*)&r,sizeof(r));
      root = new digitalTreeNode{-1};
      root->createChildren(p1,p2);
      file.close();
    }
  }

  void inorder(digitalTreeNode* actual,map<string,long>&pages, string bin){
    if(actual->children == nullptr){
      pages.insert({bin,actual->page});
      return;
    }
    inorder(actual->children[0],pages,bin + "0");
    inorder(actual->children[1],pages,bin + "1");

  }

  ~ExtendibleHash(){

    map<string,long> ayuda;
    fstream index;
    index.open(_index,ios::out|ios::binary);
    inorder(root,ayuda,"");

    for(auto it = ayuda.begin();it!= ayuda.end();++it){
      cout<<it->first<<" "<<it->second<<endl; 
      index.write((it->first).c_str(),(it->first).size());
      index.put('|');
      index.write((char*)& (it->second),sizeof(long));
      index.put('\n');

    }
    index.close();

    

       
  }

  void showAll(){
    fstream file;
    RecordPage p;
    file.open(_file,ios::in|ios::out|ios::binary);

    while(file.peek() != EOF){
      cout<<"Pagina en posicion: "<<file.tellg()<<endl;
      file.read((char*)&p, sizeof(RecordPage));
      p.show_data();

    }

  }

  void insert(RecordE record){

    int bucket = myHash(record.key) % (1 << global_depth);
    int shift = global_depth-1;
    auto temp = root;

    while(temp->children != nullptr) temp = temp->children[(bucket >> shift--) & 1];

    fstream file;
    RecordPage p;
    file.open(_file,ios::in|ios::out|ios::binary);
    file.seekg(temp->page);
    file.read((char*) &p,sizeof(p));

    if(p.size < M){
 
      p.r[p.size++] = record;
      file.seekp(temp->page);
      file.write((char*)&p,sizeof(p));
      file.close();
    
    } else {
      if (shift >= 0) {
	long temp2 = temp->page;
	long size2 = p.size;
	file.seekp(0,ios::end);
	temp->createChildren(temp2,file.tellp());
	p.size = 0;
	p.next = -1;
	file.write((char*)&p,sizeof(p));
	file.seekp(temp2);
	file.write((char*)&p,sizeof(p));
	file.close();

	insert(record);

	for (int i = 0; i < size2; ++i){
	  insert(p.r[i]);
	}
      } else {
	RecordPage new_page;
	file.seekp(0,ios::end);
	new_page.r[0] = record;
	new_page.size = 1;
	new_page.next = file.tellp();
	file.write((char*)&p,sizeof(p));
	file.seekp(temp->page);
	file.write((char*)&new_page,sizeof(new_page));
	file.close();

      }

    }


  }
	


  
};
