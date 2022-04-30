#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct pointer {
    bool aux;
    long next;

    pointer() {
        aux = 1;
        next = -1;
    }
};

struct Metadata {
    pointer head_data;
    long head_free_list;

    Metadata() {
        head_data = pointer();
        head_free_list = -1;
    }
};

void createMetadata(string filename) {
    ofstream file(filename);

    Metadata default_meta;
    file.write((char*) & default_meta, sizeof(default_meta));
    file.close();
}

struct Record {
    int key;
    int cantidad;
    char anio[5];
    pointer next;
    long next_freelist;

    Record() {
        next = pointer();
        next_freelist = -2;
    }

    Record(int k, int c, string a) {
        key = k;
        cantidad = c;
        for(int i = 0; i < a.size()+1;++i)
            anio[i] = a[i];
        next = pointer();
        next_freelist = -2;
    }
    void show_meta() {
        /* cout << "Next freelist/deleted: " << next_freelist << endl; */
        //cout << "Is next in aux? " << next.aux << endl;
        //cout << "Next pointer: " << next.next << endl;

        cout<<"key: "<<key<<endl;
        cout<<"cantidad: "<<cantidad<<endl;
        cout<<"anio: "<<anio<<endl;
    }
};
inline ostream & operator << (ostream & stream, Record  p){
    //stream.write((char*) &p, (sizeof(p) + 1));
    p.show_meta();
    return stream;
}
inline istream & operator >> (istream & stream, Record  p){
    stream.read((char*) &p, sizeof(p));
    return stream;
}

class SequentialFile {
    string _file;
    string _aux_file;
    string _metadata;

public:
    SequentialFile(){};
    SequentialFile(string filename): _file(filename + ".dat"), _aux_file(filename + "_aux.dat"), _metadata(filename + "_meta.dat") {
        ofstream meta(_metadata, ios::binary | ios::app);
        fstream main(_file,ios::binary|ios::app|ios::out);
        fstream aux(_aux_file,ios::binary|ios::app|ios::out);
        if (meta.tellp() == meta.eof()) {
            createMetadata(_metadata);
        }
        meta.close();
        aux.close();
        main.close();
    }

    ~SequentialFile(){
        //redo();
    }

    void insert(Record record) {
        /* cout << "Inserting " << record.key << endl; */
        fstream file(_file, ios::in | ios::out | ios::binary);
        fstream aux(_aux_file, ios::in | ios::out | ios::binary);
        fstream meta(_metadata, ios::in | ios::out | ios::binary);

        Metadata metadata;
        Record current, prev, overwrite,top;
        bool reached_end = 0;
        pair<bool, long> to_overwrite, prev_to_overwrite;

        meta.seekg(0, ios::beg);
        meta.read((char*) & metadata, sizeof(metadata));

        if (metadata.head_data.next == -1) {
            /* cout << "No data" << endl; */
            metadata.head_data.aux = 1;
            metadata.head_data.next = 0;
            aux.write((char*) & record, sizeof(record));

            meta.seekg(0, ios::beg);
            meta.write((char*) & metadata, sizeof(metadata));
            aux.close();
            meta.close();
        } else {
            if (metadata.head_data.aux) {
                aux.seekg(metadata.head_data.next, ios::beg);
                aux.read((char*) & current, sizeof(current));
                to_overwrite = {1, ((long) aux.tellg() - sizeof(current))};
            } else {
                file.seekg(metadata.head_data.next, ios::beg);
                file.read((char*) & current, sizeof(current));
                to_overwrite = {0, ((long) file.tellg() - sizeof(current))};
            }

            /* cout << "Head: " << current.key << endl; */
            /* cout << "Read from: " << to_overwrite.second << endl << endl; */
            if (record.key < current.key) {
                /* cout << "Less than first element" << endl; */
                record.next.aux = metadata.head_data.aux;
                record.next.next = metadata.head_data.next;
                metadata.head_data.aux = 1;
                if (metadata.head_free_list != -1) {

                    metadata.head_data.next = metadata.head_free_list;
                    aux.seekg(metadata.head_free_list,ios::beg);
                    aux.read((char*)&top,sizeof(Record));

                    metadata.head_free_list = top.next_freelist;

                } else {
                    /* cout << "No free list to push front" << endl; */
                    aux.seekg(0, ios::end);
                    metadata.head_data.next = aux.tellg();
                }

                /* cout << "New first element; " << metadata.head_data.next << endl; */
                meta.seekg(0, ios::beg);
                meta.write((char*) & metadata, sizeof(metadata));
                meta.close();
            } else {
                while (current.key < record.key) {
                    if (current.next.next == -1 && current.key < record.key) {
                        reached_end = 1;
                        break;
                    }
                    prev_to_overwrite = to_overwrite;
                    prev = current;
                    /* cout << "Reading from: " << current.next.next << " "; */
                    if (current.next.aux) {
                        aux.seekg(current.next.next, ios::beg);
                        aux.read((char*) & current, sizeof(current));
                        to_overwrite = {1, ((long) aux.tellg() - sizeof(current))};
                    } else {
                        file.seekg(current.next.next, ios::beg);
                        file.read((char*) & current, sizeof(current));
                        to_overwrite = {0, ((long) file.tellg()) - sizeof(current)};
                    }
                    /* cout << "key: " << current.key << endl; */
                }

                if (!reached_end) {
                    /* cout << "Reached end" << endl; */
                    to_overwrite = prev_to_overwrite;
                    current = prev;
                }

                /* cout << "Not push front" << endl; */
                /* cout << record.key << " goes after " << current.key << endl; */
                record.next.aux = current.next.aux;
                record.next.next = current.next.next;
                current.next.aux = 1;
                if (metadata.head_free_list != -1) {
                    current.next.next = metadata.head_free_list;
                } else {
                    aux.seekg(0, ios::end);
                    current.next.next = aux.tellg();
                }
                /* cout << "Overwriting pos: " << to_overwrite.second << endl; */
                if (to_overwrite.first) {
                    aux.seekg(to_overwrite.second, ios::beg);
                    aux.write((char*) & current, sizeof(current));
                } else {
                    file.seekg(to_overwrite.second, ios::beg);
                    file.write((char*) & current, sizeof(current));
                }
            }

            if (metadata.head_free_list != -1) {
                aux.seekg(metadata.head_free_list, ios::beg);
                aux.read((char*) & overwrite, sizeof(overwrite));
                aux.seekg(metadata.head_free_list, ios::beg);
                aux.write((char*) & record, sizeof(record));

                metadata.head_free_list = overwrite.next.next;
                meta.seekg(0, ios::beg);
                meta.write((char*) & metadata, sizeof(metadata));
            } else {
                /* cout<<"Escribiendo nuevo registro"<<endl; */
                aux.seekg(0, ios::end);
                aux.write((char*) & record, sizeof(record));
            }
        }
    }

    bool remove_record(int key){
            fstream main;
            fstream aux;
            Metadata metadata;
            pointer it;
            main.open(_metadata,ios::in|ios::out|ios::binary);
            main.read((char*)&metadata,sizeof(Metadata));
            it.next = metadata.head_data.next;
            it.aux = metadata.head_data.aux;
            main.close();
            main.open(_file,ios::in|ios::out|ios::binary);
            aux.open(_aux_file,ios::in|ios::out|ios::binary);

            Record del;
            Record temp;

            if(it.next == -1){
                main.close();
                aux.close();
                return false;

            }

            if(it.aux){
                aux.seekg(it.next);
                aux.read((char*)&temp,sizeof(Record));
            }else{
                main.seekg(it.next);
                main.read((char*)&temp,sizeof(Record));
            }

            if(temp.key == key){
                if(it.aux){
                    temp.next_freelist = metadata.head_free_list;
                    aux.seekp(it.next);
                    aux.write((char*)&temp,sizeof(Record));
                    metadata.head_free_list = it.next;
                    /* cout<<"siguiente a eliminar: "<<temp.next_freelist<<endl; */

                }else{
                    temp.next_freelist = -1;
                    main.seekp(it.next);
                    main.write((char*)&temp,sizeof(Record));

                }
                fstream md;
                md.open(_metadata,ios::out|ios::binary);
                metadata.head_data.next = temp.next.next;
                metadata.head_data.aux = temp.next.aux;
                md.write((char*)&metadata,sizeof(Metadata));
                md.close();
                return true;

            }

            while(true){
                if(it.aux){
                    aux.seekg(it.next);
                    aux.read((char*)&temp,sizeof(Record));
                }else{
                    main.seekg(it.next);
                    main.read((char*)&temp,sizeof(Record));
                }

                if(temp.next.next == -1){
                    main.close();
                    aux.close();
                    return false;
                }

                if(temp.next.aux){
                    aux.seekg(temp.next.next);
                    aux.read((char*)&del,sizeof(Record));
                }else{
                    main.seekg(temp.next.next);
                    main.read((char*)&del,sizeof(Record));
                }

                if(del.key == key){
                    /* cout<<del.key<<endl<<endl<<endl; */
                    if(temp.next.aux){
                        del.next_freelist = metadata.head_free_list;
                        aux.seekp(temp.next.next);
                        aux.write((char*)&del,sizeof(Record));
                        metadata.head_free_list = temp.next.next;
                        /* cout<<"siguiente a eliminar: "<<del.next_freelist<<endl; */

                        fstream md;
                        md.open(_metadata,ios::out|ios::binary);
                        md.write((char*)&metadata,sizeof(Metadata));
                        md.close();

                    }else{
                        del.next_freelist = -1;
                        main.seekp(temp.next.next);
                        main.write((char*)&del,sizeof(Record));

                    }

                    temp.next.next = del.next.next;
                    temp.next.aux = del.next.aux;



                    if(it.aux){
                        aux.seekg(it.next);
                        aux.write((char*)&temp,sizeof(Record));
                    }else{
                        main.seekg(it.next);
                        main.write((char*)&temp,sizeof(Record));
                    }

                    return true;
                }

                it.next = temp.next.next;
                it.aux = temp.next.aux;

            }



        }

    void redo() {
        fstream main;
        fstream aux;
        fstream new_file;
        Metadata metadata;
        pointer it;
        Record temp;
        pointer temp_next;
        long new_pos = sizeof(Record);

        main.open(_metadata,ios::in|ios::out|ios::binary);
        main.read((char*)&metadata,sizeof(Metadata));
        it.next = metadata.head_data.next;
        it.aux = metadata.head_data.aux;
        metadata.head_data.aux = 0;
        metadata.head_data.next = 0;
        metadata.head_free_list = -1;
        main.seekg(0, ios::beg);
        main.write((char*) & metadata, sizeof(Metadata));
        main.close();


        main.open(_file,ios::in|ios::out|ios::binary);
        aux.open(_aux_file,ios::in|ios::out|ios::binary);
        new_file.open("temp.dat", ios::in | ios::out | ios::binary | ios::trunc);
        //new_file.seek(0, ios::beg);

        while(true){
            if(it.aux){
                aux.seekg(it.next);
                aux.read((char*)&temp,sizeof(Record));
            }else{
                main.seekg(it.next);
                main.read((char*)&temp,sizeof(Record));
            }
            temp_next = temp.next;
            temp.next.aux = 0;
            if (temp.next.next != -1) temp.next.next = new_pos;
            else temp.next.next = -1;
            new_file.write((char*)&temp,sizeof(Record));

            if(temp_next.next == -1) break;

            it.next = temp_next.next;
            it.aux = temp_next.aux;
            new_pos += sizeof(Record);
        }

        main.close();
        aux.close();
        new_file.close();

        int status = remove(_file.c_str());
        main.open(_aux_file,ios::out|ios::binary);
        main.close();
        int status2 = rename("temp.dat",_file.c_str());
    }

    void showall() {
        fstream file(_file, ios::in | ios::out | ios::binary);
        fstream aux(_aux_file, ios::in | ios::out | ios::binary);
        fstream meta(_metadata, ios::in | ios::out | ios::binary);

        Record current;
        Metadata metadata;

        meta.read((char*) & metadata, sizeof(metadata));
        cout << "Metadata head pos: " << metadata.head_data.next << " " << metadata.head_data.aux << " " << metadata.head_free_list<<endl;

        if (metadata.head_data.aux) {
            aux.seekg(metadata.head_data.next, ios::beg);
            aux.read((char*) & current, sizeof(current));
        } else {
            file.seekg(metadata.head_data.next, ios::beg);
            file.read((char*) & current, sizeof(current));
        }

        while (current.next.next != -1) {
            cout << current.key << " ";
            current.show_meta();
            if (current.next.aux) {
                aux.seekg(current.next.next, ios::beg);
                aux.read((char*) & current, sizeof(current));
            } else {
                file.seekg(current.next.next, ios::beg);
                file.read((char*) & current, sizeof(current));
            }
        }
        cout << current.key << " ";
        current.show_meta();
    }

    void eliminar_archivos(){
        int status = remove(_file.c_str());
        int status2 = remove(_aux_file.c_str());
        int status3 = remove(_metadata.c_str());
    }
    Record readRecordM(long i)
    {
        ifstream inFile;
        Record record;
        inFile.open(_file, ios::binary); //
        inFile.seekg(i * sizeof(record), ios::beg);
        inFile.read((char *) &record, sizeof(record));
        inFile.close();
        return record;
    }
    Record readRecordA(long i)
    {
        ifstream inFile;
        Record record;
        inFile.open(_aux_file, ios::binary);
        inFile.seekg(i * sizeof(record), ios::beg);
        inFile.read((char *) &record, sizeof(record));
        inFile.close();
        return record;
    }
    Record readRecord(int i, string _file)
    {
        ifstream inFile;
        Record record;
        inFile.open(_file, ios::binary); //
        inFile.seekg(i * sizeof(record), ios::beg);
        inFile.read((char *) &record, sizeof(record));
        inFile.seekg(0, ios::beg);
        inFile.close();
        return record;
    }

    bool extra_condition(string file, int dato){
        if ((file == _aux_file && dato==-2) || (_file == file && dato!=-1))
            return true;
        return false;
    }

    vector<int> SearchPos(int begin, int end, string file_){
        fstream file;
        file.open(file_,ios::app);
        file.seekg(0,ios::end);
        int nregistros = file.tellp()/sizeof(Record);
        cout<<"r2:"<<nregistros<<endl;
        vector<int> Pos;
        if(!file.is_open()){
            cout << "No se pudo abrir el archivo.\n";
        }
        else{
            int m = 0;
            while(m < nregistros){
                Record a = readRecord(m, file_);
                int nom = a.key;
                if (begin < nom && nom < end && extra_condition(file_, a.next_freelist))
                    Pos.push_back(m);
                m++;
            }
        }
        file.seekg(0, ios::beg);
        file.close();
        return Pos;
    }

    vector<Record> SearchByRange(int begin, int end){
        vector<Record> alumnos;
        vector<int> k = SearchPos(begin ,end,  _file);
        vector<int> a = SearchPos(begin ,end, _aux_file);
        if(!k.empty() || !a.empty()){
            for(int i=0; i<k.size(); i++){
                alumnos.push_back(readRecord(k[i],  _file));
            }
            for (int i=0; i<a.size(); i++){
                alumnos.push_back(readRecord(a[i], _aux_file));
            }
            return alumnos;
        }
        else{
            abort();
        }
    }
    long SearchPos(int key){


        fstream file;
        file.open(_file,ios::app);
        file.seekg(0,ios::end);
        int nregistros=file.tellp()/sizeof(Record);

        if(!file.is_open()){
            cout << "No se pudo abrir el archivo.\n";
            return -1;
        }
            //else if()
        else{
            int l=0;
            int u= nregistros -1;
            while(u>=l){
                int m= (l+u)/2;
                Record a = readRecordM(m);
                if(key<a.key){
                    u=m-1;
                }
                else if(key>a.key){
                    l =m+1;
                }
                else{
                    if(a.next_freelist!=-1){
                        file.close();
                        return m;
                    }
                }
            }
        }
        file.close();
        return -1;
    }

    long SearchAux(int key){

        fstream file;
        file.open(_aux_file,ios::app);
        file.seekg(0,ios::end);
        int nregistros= file.tellp()/sizeof(Record);
        if(!file.is_open()) cout << "No se pudo abrir el archivo.\n";
        else{
            int m;

            for(int i=0;i<nregistros;i++){
                m=i;
                Record a = readRecordA(m);
                if(a.key==key){
                    if(a.next_freelist==-2){
                        file.close();
                        return m;
                    }
                }
            }

        }
        file.close();
        return -1;
    }



    Record Search(int key){

        vector<Record> r;
        long k= SearchPos(key);
        long a = SearchAux(key);

        if(k!=-1){
            //r.push_back(
            return readRecordM(k);
            //return r;

        }
        else{
            if(a!=-1){
                // r.push_back(
                return readRecordA(a);
                //return r;

            }
            else{
                //cerr<<"No se encontro el registro"<<endl;
                throw "No se encontro el registro";
                //return r;
            }
        }


    }

};

void insertar_tests(SequentialFile&seq){
    Record rec1(10, 10, "2022");
    Record rec2(15, 10, "2022");
    Record rec3(5, 10, "2022");
    Record rec4(22, 10, "2022");
    Record rec5(69, 10, "2022");
    Record rec6(30, 10, "2022");
    Record rec7(20,10,"2022");

    seq.insert(rec1);
    seq.insert(rec2);
    seq.insert(rec3);
    seq.insert(rec4);
    seq.insert(rec5);
    seq.insert(rec6);
    seq.insert(rec7);


}

void insertar_tests2(SequentialFile&seq){
    Record rec1(37, 10, "2022");
    Record rec2(51, 10, "2022");
    Record rec3(73, 10, "2022");
    Record rec4(13, 10, "2022");
    Record rec5(8, 10, "2022");
    Record rec6(2, 10, "2022");
    Record rec7(90,10,"2022");

    seq.insert(rec1);
    seq.insert(rec2);
    seq.insert(rec3);
    seq.insert(rec4);
    seq.insert(rec5);
    seq.insert(rec6);
    seq.insert(rec7);


}

/*int main() {*/
    /*SequentialFile seq("test");*/
    //seq.eliminar_archivos();
    //insertar_tests2(seq);

    //seq.remove_record(90);
    //vector<Record> recs = seq.SearchByRange(1, 9);
    //for (int i=0; i<recs.size(); i++){cout << recs[i].key << " " << recs[i].anio << " " <<recs[i].cantidad << endl;}

    /*seq.showall();*/
    //cout<<seq.Search(90);
    //cout<<"---"<<endl;
    //seq.remove_record(90);
    //cout<<"---"<<endl;
    /*try{
        cout<<seq.Search(90);
    }catch(char const * x){
        cout<<x;
    }*/

    // cout<<endl; 
    //seq.showall();


    /* seq.showall(); */
    /* Record rec6(13, 10, "2022"); */
    /* seq.insert(rec6); */
    //seq.showall();

/*} */
/* Metadata md; */
/* fstream ayuda; */
/* ayuda.open("test_meta.dat",ios::in|ios::binary); */
/* ayuda.read((char*)&md,sizeof(Metadata)); */
/* ayuda.close(); */

/* cout<<sizeof(Record)<<endl; */
/* cout<<md.head_data.next<<endl; */
/* cout<<md.head_data.aux<<endl; */
/* cout<<md.head_free_list<<endl; */
