
# CHOCOPY SCANNER 

Implementación de un analizador léxico (scanner) para el lenguaje propuesto (ChocoPy).

## Objetivos
1. El código fuente se leerá desde un archivo o un elemento de área de texto.
2. Implementar la lectura del input.
    - [x] Línea por línea
    - [x] La identación se calcula sumando los caracteres vacíos al comienzo de la línea.
    - [x] getchar(): devuelve el siguiente carácter del input y mueve el puntero del carácter al siguiente.
    - [x] peekchar(): devuelve el siguiente carácter sin mover el puntero. 

3. Implementar el scanner
    - [x] gettoken(): llama al scanner. Su salida es un token.
        Token:
        ```
        tokentype: tipo de token 
        tokenval : valor de token
        ```
    - Determinar si lo que estamos leyendo:
        - [x] NEWLINE, INDENT  Y DEDENT
        - [x] Es una palabra reservada.
        - [x] Es un id.
        - [x] Es un literal (número o string)
        - [x] Es un símbolo o delimitador.

4. Cuando se detecte un error:
    - [x] Informar con detalles útiles, incluido dónde se encontró, qué salió mal exactamente y cómo el programador podría solucionarlo.
    - [x] Probar con diferentes posibles errores  

5. Incluir una funcionalidad de salida detallada que rastree las etapas del scanner.


## Ejemplo de uso

El siguiente código en ChocoPy se guarda en un archivo `test.py`.

```python
# A broken program
def is_even(x:int) -> bool:
    if x % 2 == 1:
        return 0      # FIXME
    else:
        return True

print(is_even("3"))   # FIXME
```
Se compila y ejecuta el código con: 

```bash
make
make test #o con ./main.exe test.py
```

Tambien se puede compilar usando CMake con:

```bash
cmake -S . -B build -G "Unix Makefiles"
cd build
make
./main.exe ../test.py
./main.exe ../test1.py
./main.exe ../test2.py
```


Resultado:
```
INFO SCAN - Start scanning...
DEBUG SCAN - KEY [def] found at (2:1)
DEBUG SCAN - ID [is_even] found at (2:5)
DEBUG SCAN - DEL [(] found at (2:12)
DEBUG SCAN - ID [x] found at (2:13)
DEBUG SCAN - DEL [:] found at (2:14)
DEBUG SCAN - KEY [int] found at (2:15)
DEBUG SCAN - DEL [)] found at (2:18)
DEBUG SCAN - DEL [->] found at (2:20)
DEBUG SCAN - KEY [bool] found at (2:23)
DEBUG SCAN - DEL [:] found at (2:27)
DEBUG SCAN - NEWLINE [] found at (2:28)
DEBUG SCAN - INDENT [] found at (3:1)
DEBUG SCAN - KEY [if] found at (3:5)
DEBUG SCAN - ID [x] found at (3:8)
DEBUG SCAN - OP [%] found at (3:11)
DEBUG SCAN - LITNUM [2] found at (3:12)
DEBUG SCAN - OP [==] found at (3:13)
DEBUG SCAN - DEL [=] found at (3:15)
DEBUG SCAN - LITNUM [1] found at (3:17)
DEBUG SCAN - DEL [:] found at (3:18)
DEBUG SCAN - NEWLINE [] found at (3:19)
DEBUG SCAN - INDENT [] found at (4:2)
DEBUG SCAN - KEY [return] found at (4:9)
DEBUG SCAN - LITNUM [0] found at (4:16)
DEBUG SCAN - DEDENT [] found at (5:1)
DEBUG SCAN - KEY [else] found at (5:5)
DEBUG SCAN - DEL [:] found at (5:9)
DEBUG SCAN - NEWLINE [] found at (5:10)
DEBUG SCAN - INDENT [] found at (6:2)
DEBUG SCAN - KEY [return] found at (6:9)
DEBUG SCAN - KEY [True] found at (6:16)
DEBUG SCAN - NEWLINE [] found at (6:20)
DEBUG SCAN - DEDENT [] found at (7:0)
DEBUG SCAN - DEDENT [] found at (7:0)
DEBUG SCAN - NEWLINE [] found at (7:1)
DEBUG SCAN - ID [print] found at (8:1)
DEBUG SCAN - DEL [(] found at (8:6)
DEBUG SCAN - ID [is_even] found at (8:7)
DEBUG SCAN - DEL [(] found at (8:14)
DEBUG SCAN - LITSTR ["3"] found at (8:15)
DEBUG SCAN - DEL [)] found at (8:18)
DEBUG SCAN - DEL [)] found at (8:19)
INFO SCAN - Completed with 0 errors
``` 
Finalmente:
```bash
make clean
```