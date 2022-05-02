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

## Parser 

Las consultas que pueden leer actualmente nuestro programa son:  Select, Insert, Delete
Para todas las consultas cada palabra clave debe estar separada por espacios, incluyendo espacio entre cuando se coloca coma o parentsis
### Consulta select
La consulta select debe tener el formato 'select * from archivo where key + condicion', la condicion puede ser '< , > o =' +  'dato'.
En el caso de una consulta por rangos debera tener que agregarse 'and key + condicion'. 

### Consulta delete
La consulta delete debe tener el formato 'delete from archivo where key = dato '.  

### Consulta insert 
La consulta insert debe tener el formato 'insert into archivo ( key, survived , name , sex , age ) values ( dato_key , dato_survived , dato_name , dato_sex , dato_age )' 


# Insercion de Data
Para la insercion de datos dentro de nuestros arcivos se construyo y uso las funciones 'eh_insert()' y 'sf_insert()' para las estructuras correspondientes 'extendisble hash' y 'sequential file'

### SF
```cpp
void sf_insert();
```
### EH
```cpp
void eh_insert();
```

## Link de Replit
Se trabajo tambien en un replit, donde puede probar nuestro codigo: 
https://replit.com/join/sfzdtnwiaf-ereiclo

