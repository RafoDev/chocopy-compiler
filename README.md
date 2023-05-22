
# ChocoPy Compiler 🍫🐍

![Chocopy Banner](https://i.postimg.cc/NFWFJRty/chocopybanner.png)

Implementación de un analizador léxico (scanner) y sintáctico (parser) para un lenguaje basado en ChocoPy.
# Objetivos
## Scanner
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

## Parser
1. Integración con scanner: El parser debe hacer uso de la función getToken() del scanner para recibir cada uno de los tokens.
2. Se tiene una función/método por cada símbolo no terminal (NT), que analizará si el símbolo se está derivando de manera correcta.
3. Decidir qué producción es la que se usará para derivar el NT (usar conjunto de primeros) y recorrer cada símbolo de su derivación.

    - [x] Si estamos frente a un token, este debe hacer match con el input
    - [x] Si estamos frente a otro NT, se debe llamar a su función/ método.
    - [x] Tomar en cuenta que para usar la producción vacía, el siguiente
    token del input debe estar en el conjunto de siguientes del NT.

    - [x] peekchar(): devuelve el siguiente carácter sin mover el puntero. 

3. El parser nos devuelve True si el código de entrada pertenece al lenguaje.
    - [x] Caso contrario, el parser nos devuelve la lista de errores


# Uso

Se compila y ejecuta el código con: 

```bash
make
make test # para ejecutar los tests disponibles
make clean
./main.exe "tests/nombre_del_test.txt"
./main.exe # se lanza la ui
```

Alternativamente se puede usar CMake:

```bash
cmake -S . -B build -G "Unix Makefiles"
cd build
make
...
```
## Interfaz
Captura de pantalla de la UI.

![Chocopy Banner](https://i.postimg.cc/6QqD5d07/usage-Console.png)

# CFG
Gramática Libre de Contexto utilizada.
```plaintext
Program → DefList StatementList
DefList → Def DefList
DefList → ε
Def → def ID ( TypedVarList ) Return : Block
TypedVar → ID : Type
Type → int
Type → str
Type → [ Type ]
TypedVarList → ε
TypedVarList → TypedVar TypedVarListTail
TypedVarListTail → , TypedVar TypedVarListTail
TypedVarListTail → ε
Return → ε
Return → -> Type
Block → NEWLINE INDENT Statement StatementList DEDENT
StatementList → Statement StatementList
StatementList → ε
Statement → SimpleStatement NEWLINE
Statement → if Expr : Block ElifList Else
Statement → while Expr : Block
Statement → for ID in Expr : Block
ElifList → Elif ElifList
ElifList → ε
Elif → elif Expr : Block
Else → ε
Else → else : Block
SimpleStatement → Expr SSTail
SimpleStatement → pass
SimpleStatement → return ReturnExpr
SSTail → ε
SSTail → = Expr
ReturnExpr → Expr
ReturnExpr → ε
Expr → orExpr ExprPrime
ExprPrime → if andExpr else andExpr ExprPrime
ExprPrime → ε
orExpr → andExpr orExprPrime
orExprPrime → or andExpr orExprPrime
orExprPrime → ε
andExpr → notExpr andExprPrime
andExprPrime → and notExpr andExprPrime
andExprPrime → ε
notExpr → CompExpr notExprPrime
notExprPrime → not CompExpr notExprPrime
notExprPrime → ε
CompExpr → IntExpr CompExprPrime
CompExprPrime → CompOp IntExpr CompExprPrime
CompExprPrime → ε
IntExpr → Term IntExprPrime
IntExprPrime → + Term IntExprPrime
IntExprPrime → - Term IntExprPrime
IntExprPrime → ε
Term → Factor TermPrime
TermPrime → * Factor TermPrime
TermPrime → // Factor TermPrime
TermPrime → % Factor TermPrime
TermPrime → ε
Factor → - Factor
Factor → Name
Factor → Literal
Factor → List
Factor → ( Expr )
Name → ID NameTail
NameTail → ε
NameTail → ( ExprList )
NameTail → List
Literal → None
Literal → True
Literal → False
Literal → INTEGER
Literal → STRING
List → [ ExprList ]
ExprList → ε
ExprList → Expr ExprListTail
ExprListTail → ε
ExprListTail → , Expr ExprListTail
CompOp → ==
CompOp → !=
CompOp → <
CompOp → >
CompOp → <=
CompOp → >=
CompOp → is
```