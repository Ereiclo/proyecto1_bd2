#include <iostream>
#include <vector>
#include <string>
#include "SF.h"
#include "EH.h"
#include <sstream>
#include <cstring>


void Function(int data_type){
    int key=0;
    int bkey=0;
    int ekey=0;
    int k=0;
    int cantidad=0;
    char  anio[5];
    string temp1;
    string temp2;

    string consulta;
    //cin.ignore();
    cout<<"Realize su consulta: ";
    getline(cin,consulta);
    vector<char>cs;
    vector<string>p;
    string temp="";
    SequentialFile seqfile;
    ExtendibleHash extfile;
    cout<<"\n";
    if (data_type == 0)
      seqfile = SequentialFile("t");
    else
      extfile = ExtendibleHash("data.dat","index.dat", 3);
      
    
     for(int i=0;i<consulta.size();i++){
        cs.push_back(consulta[i]);
    }

    int t=0;

    while(t!=cs.size()){
        if(cs[t]!=' '){

            temp+=cs[t];

            t++;
        }

        else{
            p.push_back(temp);
            temp="";
            t++;
        }

    }
    p.push_back(temp);

  //insertar_tests2(seqfile);
/*
   for(int i=0;i<p.size();i++){
        cout<<p[i]<<endl;
    }
*/
    if(p[0]=="select" & p[1]=="*"){
        if((p[6]=="<" | p[6]==">")&(p[10]=="<" | p[10]==">")){
            //busqueda rango
            if(p[5]=="key" & p[6]=="<"){
                
                ekey=stoi(p[7]);
            }
            else if(p[5]=="key" & p[6]==">"){
                
                bkey=stoi(p[7]);
            }
            else if(p[6]=="<" & p[7]=="key"){
                
                bkey=stoi(p[5]);
            }
            else if(p[6]==">" & p[7]=="key"){
               
                 ekey=stoi(p[5]);
            }
            if(p[9]=="key" & p[10]=="<"){
                 
                 ekey=stoi(p[11]);
            }
            else if(p[9]=="key" & p[10]==">"){
                
                bkey=stoi(p[11]);
            }
            else if(p[10]=="<" & p[11]=="key"){
               
                bkey=stoi(p[9]);
            }
            else if(p[10]==">" & p[11]=="key"){
               
                ekey=stoi(p[9]);
            }
            else{
                cout<<"No se reconoce la consulta";
                return;
            }
            if (bkey >= ekey){
              cout<<"No se reconoce la consulta";
              return;
            }
            else{
              if (data_type == 0){
                cout<<"seqfile.RangeSearch("<<bkey<<","<<ekey<<")";
                vector<Record> recs = seqfile.SearchByRange(bkey, ekey);
                for (int i=0; i<recs.size(); i++){
                  cout << recs[i].key << " " << recs[i].anio << " " <<recs[i].cantidad << endl;
                }
                cout<<"*****************"<<endl;
                seqfile.showall();
              }
              else{
                cout<<"extfile.RangeSearch("<<bkey<<","<<ekey<<")";
                vector<RecordE> recs = extfile.SearchByRange(bkey, ekey, "t", "t_aux");
                for (int i=0; i<recs.size(); i++){
                  cout << recs[i].key << " " << recs[i].anio << " " <<recs[i].cantidad << endl;
                }
                cout<<"*****************"<<endl;
                seqfile.showall();
                //EHCASE
              }

              return;
            }
        }
        else{
            // busqueda
            /*
            stringstream intValue(p[8]);
            intValue>>key;*/
            key=stoi(p[7]);
            if (data_type == 0){
              cout<<"seqfile.Search("<<key<<")"<<endl;
              cout<<seqfile.Search(key);
             cout<<"*****************"<<endl;
        seqfile.showall();
            }
            else{
              cout<<"seqfile.Search("<<key<<")"<<endl;
              
              //EHCASE
            }

            return;

        }
    }
    else if(p[0]=="insert"){
        key=stoi(p[12]);
        cantidad = stoi(p[14]);
        strcpy(anio, p[16].c_str());
        if (data_type == 0){
          cout << "sqlfile.insert("<<key<<","<<cantidad<<","<<anio<<")"<<endl;
          Record rec(key, cantidad, anio);
          seqfile.insert(rec);
          cout << "done";
          cout<<"*****************"<<endl;
        }
        else {
          //EHCASE
        }

        seqfile.showall();
      
        return;
    }
    else if(p[0]=="delete"){
        if (data_type == 0){
          key = stoi(p[6]);
          cout<<"seqfile.remove("<<key<<")";
          seqfile.remove_record(key);
          cout << "done";
          cout<<"*****************"<<endl;
          seqfile.showall();
        }
        else{
          //EHCASE
        }
        
        return;
    }
    else{
        cout<<"No se reconoce la consulta";
        return;
    }
}

void interfaz(){
    int opcion=0;
    cout<<"\n";
    cout<<"Bienvenido, Seleccione una opcion: \n";
    cout<<"\n";

    cout<<"***--------------------***\n";
    cout<<"   1.Sequential file\n";
    cout<<"   2.Extendible hashing\n";
    cout<<"***--------------------***\n";

    cout<<"-> Opcion: ";cin>>opcion;
    //cout<<"Realize su consulta: ";
    cin.ignore();

    if(opcion==0){
        Function(0); //sF
    }
    else{
        Function(1); //EH
    }
}

int main(){
    //interfaz();
    Function(0);
    return 0;
}
