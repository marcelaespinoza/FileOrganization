import os
from fastapi import FastAPI, HTTPException
from ctypes import CDLL, c_int, c_char, c_char_p, c_long, Structure, byref, POINTER
import logging
from pydantic import BaseModel

# Configurar el logger
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

# Cargar la biblioteca compartida
lib = CDLL('./avl.so')

# Estructura Record en C
class Record(Structure):
    _fields_ = [
        ('cod', c_int),
        ('nombre', c_char * 12),
        ('ciclo', c_int),
        ('left', c_long),
        ('right', c_long),
        ('height', c_int)
    ]

# Modelo de Pydantic para el Record de entrada
class RecordInputModel(BaseModel):
    cod: int
    nombre: str
    ciclo: int

# Modelo de Pydantic para el Record que se retorna sin left, right, height
class RecordModel(BaseModel):
    cod: int
    nombre: str
    ciclo: int

# Definir los tipos de las funciones
lib.call_inorder.argtypes = [c_char_p, POINTER(c_int)]
lib.call_inorder.restype = POINTER(Record)

lib.insert_record.argtypes = [c_char_p, POINTER(Record)]
lib.insert_record.restype = None

lib.search_record.argtypes = [c_char_p, c_int]
lib.search_record.restype = POINTER(Record)

lib.free_records.argtypes = [POINTER(Record)]
lib.free_records.restype = None

lib.free_record.argtypes = [POINTER(Record)]
lib.free_record.restype = None

# Crear la instancia de FastAPI
app = FastAPI()

@app.get("/")
def read_root():
    return {"message": "Connection successful"}

@app.get("/call_inorder", response_model=list[RecordModel])
def call_inorder_route(filename: str):
    logger.info(f"Received filename: {filename}")

    if not os.path.isfile(filename):
        logger.error(f"File not found: {filename}")
        raise HTTPException(status_code=404, detail="File not found")

    size = c_int()
    filename_encoded = filename.encode('utf-8')
    record_ptr = lib.call_inorder(filename_encoded, byref(size))

    # Convertir los registros de C a Python
    records = []
    for i in range(size.value):
        record = record_ptr[i]
        # Crear el RecordModel sin left, right, height
        records.append(RecordModel(
            cod=record.cod,
            nombre=record.nombre.decode('utf-8').strip(),
            ciclo=record.ciclo
        ))

    lib.free_records(record_ptr)
    logger.info(f"Successfully retrieved {size.value} records from file: {filename}")
    return records

@app.post("/insert_record")
def insert_record_route(record: RecordInputModel, filename: str):
    logger.info(f"Inserting record: {record.cod}, {record.nombre}, {record.ciclo} into {filename}")

    if not os.path.isfile(filename):
        logger.error(f"File not found: {filename}")
        raise HTTPException(status_code=404, detail="File not found")

    filename_encoded = filename.encode('utf-8')
    
    # Crear una instancia de Record para C
    record_c = Record(
        cod=record.cod,
        nombre=record.nombre.encode('utf-8'),
        ciclo=record.ciclo,
        left=-1,  # Valores predeterminados, pueden no ser necesarios
        right=-1,  # Valores predeterminados, pueden no ser necesarios
        height=0,  # Valor predeterminado
    )

    lib.insert_record(filename_encoded, byref(record_c))
    logger.info(f"Successfully inserted record into file: {filename}")

    return {"message": "Record inserted successfully"}

@app.get("/search_record")
def search_record_route(key: int, filename: str):
    logger.info(f"Searching for record with key: {key} in {filename}")

    if not os.path.isfile(filename):
        logger.error(f"File not found: {filename}")
        raise HTTPException(status_code=404, detail="File not found")

    filename_encoded = filename.encode('utf-8')
    
    # Llamar a la función de búsqueda en C
    record_ptr = lib.search_record(filename_encoded, key)

    if record_ptr is None:
        logger.error(f"Record with key {key} not found.")
        raise HTTPException(status_code=404, detail="Record not found")

    # Convertir el registro de C a Python
    record = record_ptr[0]  
    found_record = RecordModel(
        cod=record.cod,
        nombre=record.nombre.decode('utf-8').strip(),
        ciclo=record.ciclo
    )

    if (found_record.cod == -1):
        logger.error("Record not found")
        raise HTTPException(status_code=404, detail="Record not found")


    lib.free_record(record_ptr)  # Liberar el registro encontrado
    return found_record

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="127.0.0.1", port=8000)

