# FileOrganization
Project 1 of data base 2 course

## INSTALAR LIBRERIAS
pip install -r requirements.txt

## COMPILAR CODIGOS
g++ -shared -o avl.so -fPIC AVLFile.cpp


## INICIAR SERVIDOR
uvicorn main:app --reload
