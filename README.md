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
SequentialFile(string filename,long _k); //crear el archivo 
void insert(RecordS record); //insertar registros O(n + k)
bool remove_record(int key); //remover un registro con llave key, si no se pudo se retorna false O(n + k)
Record search(int key); //buscar un registro O(log n + k)
search_by_range(int begin,int end); //buscar un rango de registros O(n + k)

private:
redo()//rehace el archivo O(n)

```
  
## Extendible Hashing

```cpp
public:
ExtendibleHash(string filename,string index,int global_depth); //crear el archivo 
void insert(RecordS record); //insertar registros O(1)
bool remove_record(int key); //remover un registro con llave key, si no se pudo se retorna false O(k)
RecordE search(int key); //buscar un registro O(n)
search_by_range(int begin,int end); //buscar un rango de registros O(n)
```

## Parser 

Las consultas que pueden leer actualmente nuestro programa son:  Select, Insert, Delete
Para todas las consultas cada palabra clave debe estar separada por espacios, incluyendo espacio entre cuando se coloca coma o parentesis
### Consulta select
La consulta select debe tener el formato 'select * from titanic where key + condicion', la condición puede ser '< , > o =' +  'dato'.
En el caso de una consulta por rangos debera tener que agregarse 'and key + condicion'. 

### Consulta delete
La consulta delete debe tener el siguiente formato: 'delete from archivo where key = dato '.  

### Consulta insert 
La consulta insert debe tener el siguiente formato: 'insert into archivo ( key , survived , name , sex , age ) values ( dato_key , dato_survived , dato_name , dato_sex , dato_age )' 
NOTA: En caso de que el nombre tenga espacios (ejem: paulo cuaresma ) convenientemente requerimos que se ingrese de la siguiente manera : paulo_cuaresma.

# Insercion de Data
Para la inserción de datos dentro de nuestros archivos se construyó y uso las funciones 'eh_insert()' y 'sf_insert()' para las estructuras correspondientes 'extendisble hash' y 'sequential file'. La data que se inserta se encuentra en el archivo 'titanic_data.csv' y guarda la información acerca de los pasajeros que estuvieron a bordo del Titanic y además indica si sobrevivieron o no.

### SF
```cpp
void sf_insert();
```
### EH
```cpp
void eh_insert();
```

# Inicializacion de Data:
Para poder inicializar los archivos, primero en el int main() se deberán llamar a las funciones 'sf_insert()' y 'eh_insert()' una sola vez, las cuales a través del dataset crearan los archivos .dat.
Finalmente para realizar consultas segun los metodos implementados se debera llamar a la funcion 'interfaz()' donde elegiremos una opción y procederemos a ingresar nuestras consultas.

## Link de Replit:
Se trabajó también en un replit, donde puede probar nuestro código: 
https://replit.com/join/sfzdtnwiaf-ereiclo

## Link del video:
https://www.youtube.com/watch?v=OHvVuqc6sSE
