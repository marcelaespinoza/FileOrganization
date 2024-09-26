from fastapi import FastAPI
from ctypes import CDLL, c_int, c_char_p, c_char_p, c_long, c_char, POINTER, Structure, byref

# Cargar la biblioteca compartida
lib = CDLL('./avl.so')

# Estructura Record
class Record(Structure):
    _fields_ = [
        ('cod', c_int),
        ('nombre', c_char * 12),
        ('ciclo', c_int),
        ('left', c_long),
        ('right', c_long),
        ('height', c_int)
    ]

# Definir los tipos de las funciones
lib.call_inorder.argtypes = [c_char_p, POINTER(c_int)]
lib.call_inorder.restype = POINTER(Record)

lib.free_records.argtypes = [POINTER(Record)]
lib.free_records.restype = None

app = FastAPI()

@app.get("/")
def read_root():
    return {"message": "Connection successful"}

@app.get("/call_inorder")
def call_inorder_route(filename: str):
    size = c_int()  
    filename_encoded = filename.encode('utf-8')  
    record_ptr = lib.call_inorder(filename_encoded, byref(size))  # Llamar a la función en cpp

    # Convertir los registros de C a Python
    records = []
    for i in range(size.value):
        record = record_ptr[i]
        records.append({
            "cod": record.cod,
            "nombre": record.nombre.decode('utf-8').strip(),
            "ciclo": record.ciclo,
            "left": record.left,
            "right": record.right,
            "height": record.height
        })

    # Liberar la memoria en el lado C++
    lib.free_records(record_ptr)

    return records

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="127.0.0.1", port=8000)

