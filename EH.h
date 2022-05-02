#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <functional>
#include <string>
#define M 10
#define ELIMINADO -3

using namespace std;

void printcharE(char* s, int size) {
    int i = 0;
    string buffer;
    while (s[i] != '\0') {
        buffer += s[i++];
    }
    cout << buffer << " ";
}

struct RecordE {
    int key;
    bool survived;
    char name[256];
    char sex[10];
    int age;

    RecordE() {}

    RecordE(int pid, bool s, char* n, char* se, int a) {
        key = pid;
        survived = s;
        for (int i = 0; i < 256; i++) {
            name[i] = n[i];
        }
        for (int i = 0; i < 10; i++) {
            sex[i] = se[i];
        }
        age = a;
    };

    void show_data() {
        cout << "ID: " << key << endl;
        cout << "Nombre: ";
        printcharE(name,256);
        cout << endl;
        cout << "Sexo: ";
        printcharE(sex, 10);
        cout << endl;
        cout << "Edad: " << age << endl;
        if (survived) cout << "Sobrevivio: true" << endl;
        else cout << "Sobrevivio: false";
    }

    void show_meta() {
        // cout << "Next freelist/deleted: " << next_freelist << endl;
        // cout << "Is next in aux? " << next.aux << endl;
        // cout << "Next pointer: " << next.next << endl;
        // cout<<"----------------------"<<endl;
        /* cout<<"key: "<<key<<endl; */
        /* cout<<"cantidad: "<<cantidad<<endl; */
        /* cout<<"anio: "<<anio<<endl; */
        /* cout<<"----------------------"<<endl; */
        show_data();
    }
};



inline ostream & operator << (ostream & stream, RecordE  p){
    //stream.write((char*) &p, (sizeof(p) + 1));
    p.show_data();
    return stream;
}
inline istream & operator >> (istream & stream, RecordE  p){
    stream.read((char*) &p, sizeof(p));
    return stream;
}
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

    //cout<<file2.peek();
    if(file2.peek() != EOF){
    //if(0){

      char buffer[256];
      char buffer2[9];
      long pag;
      root = new digitalTreeNode{-1};
  
      while(file2.peek() != EOF){
        file2.getline(buffer,256,'|');
        string temp(buffer);
        file2.read((char*)&pag,sizeof(long));
        //cout<<"aaa: "<<" "<<temp << "->" << pag<<endl<<endl;
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
    //cout<<"debería estar bien: "<<endl;
    for(auto it = ayuda.begin();it!= ayuda.end();++it){
      //cout<<it->first<<" "<<it->second<<endl; 
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
    file.close();

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
      //cout << record.key << " va en pagina con pos: " << temp->page << endl;
      p.r[p.size++] = record;
      file.seekp(temp->page);
      file.write((char*)&p,sizeof(p));
      file.close();
      //cout<<"La siguiente pagina es: "<<p.next<<endl;
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
	//cout << record.key << " va en pagina con pos: " << temp->page << endl;
      }

    }


  }
	bool remove(int key) {
		cout << "Removing : " << key << endl;
		int bucket = myHash(key) % (1 << global_depth);
    int shift = global_depth-1;
    auto temp = root;

    while(temp->children != nullptr) temp = temp->children[(bucket >> shift--) & 1];

		bool found = false;
		long prev_pos, overwrite_pos;
		RecordPage prev_page, page, overwrite_page;
		fstream file;
		file.open(_file, ios::in | ios::out | ios::binary);
		overwrite_pos = temp->page;
		file.seekg(temp->page);
		file.read((char*) & page, sizeof(page));
		while (!found) {
			for (int i = 0; i < page.size; i++) {
				if (page.r[i].key == key) {
					found = true;
					remove_from_page(page, key, i);
				}
			}
			if (page.next == -1 && !found) return false;
			if (page.next == -1 || found) break;
			prev_pos = overwrite_pos;
			prev_page = page;
			overwrite_pos = page.next;
			file.seekg(page.next, ios::beg);
			file.read((char*) & page, sizeof(page));
		}

		//cout << "Sobreescribiendo pagina en : " << overwrite_pos << endl;
		if (page.size == 0) {
			//cout << "Se quedo vacio" << endl;
			if (prev_page.next != -2) {
				//cout << "Cambiando puntero de pagina anterior a " << page.next << endl;
				prev_page.next = page.next;
				page.next = ELIMINADO;
				file.seekg(prev_pos, ios::beg);
				file.write((char*) & prev_page, sizeof(prev_page));
			} else {
				//cout << "Pagina inicial" << endl;
			}
		}
		file.seekg(overwrite_pos, ios::beg);
		file.write((char*) & page, sizeof(page));
		file.close();
		// cout << "Se encontro " << key << endl;
		return true;
	}

	void remove_from_page(RecordPage & page, int key, int pos) {
		page.size--;
		for (int i = pos; i < page.size; i++) {
			page.r[i] = page.r[i+1];
		}
	}

 bool existinrecord(RecordE record, int key){
		if (record.key == key) return true;
		return false;
	}

	RecordE search(int key){
		int bucket = myHash(key) % (2 << (global_depth - 1));
		int shift = global_depth-1;
		auto temp = root;
		while(temp->children != nullptr) temp = temp->children[(bucket >> shift--) & 1];
		
		fstream file;
		
		file.open(_file,ios::in|ios::out|ios::binary);

    
    long temp_page = temp->page;
		while(true){
      RecordPage p;  
      file.seekg(temp_page);
  		file.read((char*) &p,sizeof(p));
      for(int i=0; i<p.size; i++){
        if (existinrecord(p.r[i], key)) return p.r[i];
      }
      if (p.next != -1){
        temp_page = p.next;
      }
      else break;
    }
		throw "No se encontro el registro";
	}	

  RecordE readRecord(int i)
    {
        ifstream inFile;
        RecordE record;
        inFile.open(_file, ios::binary);
        inFile.seekg(i * sizeof(record), ios::beg);
        inFile.read((char *) &record, sizeof(record));
        inFile.close();
        return record;
    }

    vector<RecordE> SearchByRange(int bkey, int ekey){
      cout << "buscando entre " << bkey << " y " << ekey << endl;
      vector<RecordE>r;
      fstream file;
      RecordPage p;
      file.open(_file,ios::in|ios::out|ios::binary);

      while(file.peek() != EOF){
        file.read((char*)&p, sizeof(RecordPage));
  
        for(int i=0;p.next!=ELIMINADO & i<p.size;++i) {
            if(p.r[i].key > bkey & p.r[i].key < ekey) {
                r.push_back(p.r[i]);
            }
        }

      }
      file.close();

      return r;
    }
};
