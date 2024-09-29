import sys
import requests
from PyQt5.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QPushButton, QLabel, QTextEdit)
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import Qt

# Funciones puras para manejar los comandos
def create_table(command):
    if "game" in command and "avl" in command:
        return "La tabla 'game' con índice 'avl' fue creada (simulación)."
    return "Error: Solo se puede crear la tabla 'game' usando el índice 'avl'."

def insert_from_file(command):
    if "game" in command:
        csv_path = command.split("'")[1]
        return read_csv_from_file(csv_path)
    return "Error: Solo se puede insertar desde archivos a la tabla 'game'."

def read_csv_from_file(csv_path):
    try:
        payload = {"csv_path": csv_path}
        response = requests.post(f"http://localhost:8080/avl/read_csv?table_name=game", json=payload)
        response.raise_for_status()
        return f"Datos del archivo {csv_path} insertados correctamente."
    except requests.exceptions.RequestException as e:
        return f"Error al leer el CSV: {e}"

def search_by_key(command):
    if "game" in command and "clave = " in command:
        key = extract_key_from_command(command)
        return fetch_record(key)
    return "Error: El comando no está bien formado o no contiene una clave válida."

def fetch_record(key):
    try:
        response = requests.get(f"http://localhost:8080/avl/get_record?table_name=game&key={key}")
        response.raise_for_status()
        return f"Registro con clave {key}: {response.json()}"
    except requests.exceptions.RequestException as e:
        return f"Error al obtener el registro: {e}"

def range_search(command):
    if "game" in command and "between" in command:
        start, end = extract_range_from_command(command)
        return fetch_data_between(start, end)
    return "Error: El comando no está bien formado o no contiene un rango válido."

def fetch_data_between(start, end):
    try:
        response = requests.get(f"http://localhost:8080/avl/get_between?table_name=game&start={start}&end={end}")
        response.raise_for_status()
        return f"Registros entre {start} y {end}: {response.json()}"
    except requests.exceptions.RequestException as e:
        return f"Error al obtener los datos: {e}"

def insert_values(command):
    if "game" in command:
        values = extract_values_from_command(command)
        return insert_record(values)
    return "Error: Solo se puede insertar en la tabla 'game'."

def insert_record(values):
    try:
        payload = values
        response = requests.post(f"http://localhost:8080/avl/post_record?table_name=game", json=payload)
        response.raise_for_status()
        return "Registro insertado correctamente."
    except requests.exceptions.RequestException as e:
        return f"Error al insertar el registro: {e}"

def delete_by_key(command):
    if "game" in command and "clave = " in command:
        key = extract_key_from_command(command)
        return remove_record(key)
    return "Error: El comando no está bien formado o no contiene una clave válida."

def remove_record(key):
    try:
        response = requests.delete(f"http://localhost:8080/avl/delete_record?table_name=game&key={key}")
        response.raise_for_status()
        return f"Registro con clave {key} eliminado correctamente."
    except requests.exceptions.RequestException as e:
        return f"Error al eliminar el registro: {e}"

# Funciones puras auxiliares para extraer datos de los comandos
def extract_key_from_command(command):
    try:
        return command.split("clave = ")[1].strip().strip(";")
    except IndexError:
        return None

def extract_range_from_command(command):
    try:
        return command.split("between")[1].strip().strip(";").split(" and ")
    except IndexError:
        return None, None

def extract_values_from_command(command):
    values = command.split("values")[1].strip(" ();").split(",")
    return {
        "key": int(values[0]),
        "Rank": int(values[0]),
        "GameTitle": values[1].strip(),
        "Platform": values[2].strip(),
        "Year": int(values[3]),
        "Genre": values[4].strip(),
        "Publisher": values[5].strip(),
        "NorthAmerica": float(values[6]),
        "Europe": float(values[7]),
        "Japan": float(values[8]),
        "RestOfWorld": float(values[9]),
        "Global": float(values[10]),
        "Review": float(values[11])
    }

# Función pura para procesar los comandos
def process_command(command):
    command = command.lower().strip()

    if command.startswith("create table"):
        return create_table(command)
    elif command.startswith("insert into table") and "from file" in command:
        return insert_from_file(command)
    elif command.startswith("select * from") and "clave = " in command:
        return search_by_key(command)
    elif command.startswith("select * from") and "between" in command:
        return range_search(command)
    elif command.startswith("insert into"):
        return insert_values(command)
    elif command.startswith("delete from") and "clave = " in command:
        return delete_by_key(command)
    else:
        return "Comando no reconocido."

# Frontend para ingresar y mostrar comandos
class CommandApp(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Parser SQL-like para AVL Backend")
        self.setGeometry(100, 100, 600, 400)

        layout = QVBoxLayout()

        # Entrada para el comando SQL-like
        self.command_input = QTextEdit()
        layout.addWidget(self.command_input)

        # Botón para ejecutar el comando
        self.execute_button = QPushButton("Ejecutar Comando")
        self.execute_button.clicked.connect(self.execute_command)
        layout.addWidget(self.execute_button)

        # Etiqueta para mostrar el resultado
        self.result_label = QLabel("")
        layout.addWidget(self.result_label)

        # Ícono de "cargando"
        self.loading_label = QLabel(self)
        pixmap = QPixmap("loading.gif").scaled(50, 50, Qt.KeepAspectRatio)
        self.loading_label.setPixmap(pixmap)
        self.loading_label.setVisible(False)
        layout.addWidget(self.loading_label)

        self.setLayout(layout)

    # Función para ejecutar el comando y mostrar el resultado
    def execute_command(self):
        self.loading_label.setVisible(True)
        self.update()

        command = self.command_input.toPlainText()
        result = process_command(command)

        self.result_label.setText(result)
        self.loading_label.setVisible(False)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = CommandApp()
    window.show()
    sys.exit(app.exec_())
