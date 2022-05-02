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
    bool survived;
    char name[256];
    char sex[10];
    int age;
    string temp1;
    string temp2;

    string consulta;
   cin.ignore();
    cout<<"Realize su consulta: "<<endl;
    getline(cin,consulta);
    vector<char>cs;
    vector<string>p;
    string temp="";
    SequentialFile seqfile("titanic_data",75);
    ExtendibleHash extfile("titanic_data_eh.dat", "eh_index.dat", 6);
    cout<<"\n";


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

   
    if(p[0]=="select" & p[1]=="*" & p[3]=="titanic"){
        if((p[6]=="<" | p[6]==">")&(p[10]=="<" | p[10]==">")){
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
                    vector<Record> recs = seqfile.SearchByRange(bkey, ekey);
                  if(recs.size()==0){
                      cout<<"No se encontraron registros en el rango"<<endl;
                    return;
                  }
                  else{
                    for (int i=0; i<recs.size(); i++){
                                      cout<<"*****************"<<endl;

                        cout <<"key: "<< recs[i].key <<endl;
                        if (recs[i].survived){
                            cout <<"survived: true"<<endl;
                        }
                        else cout <<"survived: false"<<endl;

                        cout <<"name: " <<recs[i].name << endl;
                        cout <<"sex: " <<recs[i].sex << endl;
                        cout <<"age: " <<recs[i].age << endl;
                        cout<<"*****************"<<endl;
                    }
                  }
                    
                                        return;

                }
                else{
                    
                    vector<RecordE> recs = extfile.SearchByRange(bkey, ekey);
                  if(recs.size()==0){
                      cout<<"No se encontraron registros en el rango"<<endl;
                    return;
                  }
                  else{
                    for (int i=0; i<recs.size(); i++){
                                      cout<<"*****************"<<endl;

                        cout <<"key: "<< recs[i].key <<endl;
                        if (recs[i].survived){
                            cout <<"survived: true"<<endl;
                        }
                        else cout <<"survived: false"<<endl;
                        cout <<"nombre: " <<recs[i].name << endl;
                        cout <<"sex: " <<recs[i].sex << endl;
                        cout <<"age: " <<recs[i].age << endl;

                        cout<<"*****************"<<endl;

                    }
                                        return;
                  }
                    

                    
                }
            }
        }
        else{
            
            key=stoi(p[7]);
            if (data_type == 0){
                                cout<<"*****************"<<endl;

                try{
                    cout<<seqfile.Search(key);
                }catch (char const* a){cout<<"Elemento no encontrado"<<endl;}
                cout<<"*****************"<<endl;
                return;
                

            }
            else{
                cout<<"*****************"<<endl;
                try{
                    cout <<"key to search: " <<key << endl;
                    cout<<extfile.search(key);
                }catch (char const* a){cout<<"Elemento no encontrado"<<endl;}

                cout<<"*****************"<<endl;
                return;
                
            }
        }
    }
    else if(p[0]=="insert" & p[2]=="titanic"){
        key=stoi(p[16]);
        survived = (p[18] == "false");
        strcpy(name, p[20].c_str());
        strcpy(sex, p[22].c_str());
        age=stoi(p[24]);
        if (data_type == 0){
                        cout<<"*****************"<<endl;

            Record rec(key,survived, name,sex,age);
            seqfile.insert(rec);
            cout << "done"<<endl;
            cout<<"*****************"<<endl;
            return;
        }
        else {
                        cout<<"*****************"<<endl;

            auto x= RecordE{key, survived,name,sex,age};
            extfile.insert(x);
            cout << "done"<<endl;
            cout<<"*****************"<<endl;
            return;
        }


        return;
    }
    else if(p[0]=="delete" & p[2]=="titanic"){
        if (data_type == 0){
            key = stoi(p[6]);

            cout<<"*****************"<<endl;
            seqfile.remove_record(key);
            cout << "done";
            cout<<"*****************"<<endl;
            return;
        }
        else{
            key = stoi(p[6]);
            cout<<"*****************"<<endl;
            extfile.remove(key);
            cout << "done";
            cout<<"*****************"<<endl;
            return;
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
    cout<<"***--------------------***\n";

    //cin.ignore();

    if(opcion==1){
        Function(0); //sF
    }
    else{
        Function(1); //EH
    }
}
void eh_insert() {
    ExtendibleHash eh("titanic_data_eh.dat", "eh_index.dat", 6);
    int passenger_id;
    bool survived;
    int p_class;
    char name[256];
    char sex[10];
    int age = -1;
    int sibsp;
    int parch;
    char ticket[20];
    double fare;
    char cabin[15];
    char embarked[2];
    int i;

    string buffer;
    ifstream file("titanic_data.csv");

    string line;

    getline(file, line);

    while (file.peek() != EOF) {
        getline(file, line);
        auto temp = line.begin();
        buffer = "";
        while (*(temp) != ',') {
            buffer += *(temp);
            advance(temp, 1);
        }
        passenger_id = stoi(buffer);
        //cout << passenger_id << " "<<endl;

        advance(temp, 1);
        buffer = "";
        buffer += *(temp);
        survived = stoi(buffer);
        /* cout << survived << " "; */

        advance(temp, 2);
        buffer = "";
        buffer += *(temp);
        p_class = stoi(buffer);
        // cout << p_class << " ";

        advance(temp, 3);
        i = 0;
        while (*(temp) != '"') {
            name[i] = *(temp);
            i++;
            advance(temp, 1);
        }
        name[i] = '\0';
        // cout << name << " ";
        // printchar(name, 256);

        i = 0;


        while (*(temp) != ',') {
            advance(temp, 1);
        }
        advance(temp, 1);
        while (*(temp) != ',') {
            sex[i] = *(temp);
            advance(temp, 1);
            i++;
        }
        sex[i] = '\0';
        // printchar(sex, 10);

        buffer = "";
        while (*(temp) != ',') {
            advance(temp, 1);
        }
        advance(temp, 1);
        if (*(temp) != ',') {
            while (*(temp) != ',') {
                buffer += *(temp);
                advance(temp, 1);
            }
            age = stoi(buffer);
        } else {
            age = 22;
        }
        // cout << age << " ";
        RecordE rec(passenger_id, survived, name, sex, age);
       // rec.show_meta();
        eh.insert(rec);
    }

    eh.showAll();
}

void sf_insert                                   () {
    SequentialFile seq("titanic_data", 75);
    int passenger_id;
    bool survived;
    int p_class;
    char name[256];
    char sex[10];
    int age = -1;
    int sibsp;
    int parch;
    char ticket[20];
    double fare;
    char cabin[15];
    char embarked[2];
    int i;

    string buffer;
    ifstream file("titanic_data.csv");

    string line;

    getline(file, line);

    while (file.peek() != EOF) {
        getline(file, line);
        auto temp = line.begin();
        buffer = "";
        while (*(temp) != ',') {
            buffer += *(temp);
            advance(temp, 1);
        }
        passenger_id = stoi(buffer);
        // cout << passenger_id << " ";

        advance(temp, 1);
        buffer = "";
        buffer += *(temp);
        survived = stoi(buffer);
        // cout << survived << " ";

        advance(temp, 2);
        buffer = "";
        buffer += *(temp);
        p_class = stoi(buffer);
        // cout << p_class << " ";

        advance(temp, 3);
        i = 0;
        while (*(temp) != '"') {
            name[i] = *(temp);
            i++;
            advance(temp, 1);
        }
        name[i] = '\0';
        // cout << name << " ";
        // printchar(name, 256);

        i = 0;


        while (*(temp) != ',') {
            advance(temp, 1);
        }
        advance(temp, 1);
        while (*(temp) != ',') {
            sex[i] = *(temp);
            advance(temp, 1);
            i++;
        }
        sex[i] = '\0';
        // printchar(sex, 10);

        buffer = "";
        while (*(temp) != ',') {
            advance(temp, 1);
        }
        advance(temp, 1);
        if (*(temp) != ',') {
            while (*(temp) != ',') {
                buffer += *(temp);
                advance(temp, 1);
            }
            age = stoi(buffer);
        } else {
            age = 22;
        }
        // cout << age << " ";
        Record rec(passenger_id, survived, name, sex, age);
        //rec.showdata();
        seq.insert(rec);
    }

    seq.showall();
}
int main(){
    interfaz();
    cout<<"Hasta pronto!"<<endl;
    return 0;
}







