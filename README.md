# proyecto1_bd2

Este es el repositorio del grupo 1 para el el proyecto 1 del curso Base de datos II en UTEC. Se decidió implementar dos ténicas de organización de archivos: Sequential file y Extendible Hashing. Para comprobar su funcionamiento usamos el dataset titanic_data.csv.

## Intregantes y participación
| Nombre   | Actividades | Nota |
|-----|------------------|------------------|
| Eric Bracamonte |    Remove sequential file, redo sequential file, insert extendible hash, guardar digital tree en memoria secundaria |            |
| Paulo Cuaresma  |      Búsqueda en sequential file, búsqueda por rango en exntendible hash, sql parser con consola, edito el video    |           |
| Francisco Magot |    Insert sequential file, redo sequential file, remove extendible hash, parsear data real|       |         | 
| Edgar Chacon    |    Búsqueda en sequential file, búsqueda en sequential en exntendible hash, sql parser con consola    |       |

## Sequential File

```cpp
public:
SequentialFile(string filename,long _k); //como crear el archivo 
void insert(RecordS record); //como insertar registros O(n + k)
bool remove_record(int key); //como remover un registro con llave key, si no se pudo se retorna false O(n + k)
Record search(int key); //buscar un registro O(log n + k)
search_by_range(int begin,int end); //buscar un rango de registros O(n + k)

private:
redo()//rehace el archivo O(n)

```
  
## Extendible Hashing

```cpp
public:
ExtendibleHash(string filename,string index,int global_depth); //como crear el archivo 
void insert(RecordS record); //como insertar registros O(1)
bool remove_record(int key); //como remover un registro con llave key, si no se pudo se retorna false O(k)
RecordE search(int key); //buscar un registro O(n)
search_by_range(int begin,int end); //buscar un rango de registros O(n)
```
