import sys
import time
import requests
from PyQt5.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QPushButton, QLabel, QTableWidget, QTableWidgetItem, QHBoxLayout, QComboBox, QLineEdit, QTextEdit)
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import Qt
import re

# Variables globales
page_size = 100
current_page = 0
data = []
columns = []  #lista dinámica que guarda las columnas de la tabla

# Parser SQL con soporte para `USING INDEX` y columnas variables
def sql_parser(window, query):
    query = query.strip().upper()

    # Verificar si se está usando un índice específico
    index_match = re.search(r"USING INDEX (HASH|BTREE|AVL)", query)
    if index_match:
        index = index_match.group(1).lower()
    else:
        index = window.index_combo.currentText()  # Si no se especifica índice, se usa el seleccionado en la interfaz

    # SELECT * FROM table
    if re.match(r"SELECT \* FROM \w+", query):
        table_name = query.split("FROM")[1].split("USING")[0].strip().lower() + ".dat"
        window.time_label.setText(f"Fetching data from {table_name} using index {index}")
        fetch_data(window, table_name, index)
    
    # SELECT * FROM table WHERE key BETWEEN x AND y
    elif re.match(r"SELECT \* FROM \w+ WHERE KEY BETWEEN \d+ AND \d+", query):
        table_name, start, end = re.findall(r"FROM (\w+) WHERE KEY BETWEEN (\d+) AND (\d+)", query)[0]
        window.time_label.setText(f"Fetching data from {table_name} between {start} and {end} using index {index}")
        fetch_data_between(window, table_name.lower() + ".dat", start, end, index)
    
    # INSERT INTO table (col1, col2, col3) VALUES (1, 'value', 2)
    elif re.match(r"INSERT INTO \w+ \(\w+(, \w+)*\) VALUES \((\d+|'[^']*')(, (\d+|'[^']*'))*\)", query):
        insert_match = re.findall(r"INTO (\w+).*VALUES \((.*)\)", query)[0]
        table_name = insert_match[0]
        values = insert_match[1].split(", ")
        cols = re.findall(r"INTO \w+ \((.*)\)", query)[0].split(", ")

        window.time_label.setText(f"Inserting record into {table_name} using index {index}")
        insert_record(window, table_name.lower() + ".dat", cols, values, index)
    
    # DELETE FROM table WHERE key = x
    elif re.match(r"DELETE FROM \w+ WHERE KEY = \d+", query):
        table_name, key = re.findall(r"FROM (\w+) WHERE KEY = (\d+)", query)[0]
        window.time_label.setText(f"Deleting record with key {key} from {table_name} using index {index}")
        remove_record(window, table_name.lower() + ".dat", key, index)
    
    # CREATE TABLE table_name (col1, col2, col3)
    elif re.match(r"CREATE TABLE \w+ \(.+\)", query):
        table_name = query.split("TABLE")[1].split("(")[0].strip().lower() + ".dat"
        window.time_label.setText(f"Creating table {table_name}")
        create_table(window, table_name)
    
    else:
        window.time_label.setText("Comando SQL no reconocido")

# Función para obtener los datos desde el backend con el endpoint get_all
def fetch_data(window, filename="data.dat", index="hash"):
    window.loading_label.setVisible(True)
    window.update()

    start_time = time.time()

    try:
        response = requests.get(f"http://localhost:8080/get_all?index={index}&filename={filename}")
        response.raise_for_status()
        global data, columns
        result = response.json()

        data = result["records"]
        columns = result["columns"]  # Las columnas vienen desde el backend

        end_time = time.time()
        elapsed_time = end_time - start_time
        window.time_label.setText(f"Datos cargados en {elapsed_time:.2f} segundos usando el índice {index}")

        update_table(window)

    except requests.exceptions.RequestException as e:
        window.time_label.setText(f"Error al obtener los datos: {e}")

    window.loading_label.setVisible(False)

# Función para obtener datos con el endpoint get_between
def fetch_data_between(window, filename, start, end, index="hash"):
    window.loading_label.setVisible(True)
    window.update()

    try:
        response = requests.get(f"http://localhost:8080/get_between?index={index}&filename={filename}&start={start}&end={end}")
        response.raise_for_status()
        global data, columns
        result = response.json()

        data = result["records"]
        columns = result["columns"]  # Las columnas vienen desde el backend

        window.time_label.setText(f"Datos entre {start} y {end} cargados usando el índice {index}")
        update_table(window)

    except requests.exceptions.RequestException as e:
        window.time_label.setText(f"Error al obtener los datos: {e}")

    window.loading_label.setVisible(False)

# Función para insertar un nuevo registro
def insert_record(window, filename, cols, values, index="hash"):
    window.loading_label.setVisible(True)
    window.update()

    try:
        payload = dict(zip(cols, values))

        response = requests.post(f"http://localhost:8080/insert?index={index}&filename={filename}", json=payload)
        response.raise_for_status()

        window.time_label.setText(f"Registro insertado correctamente usando el índice {index}.")

    except requests.exceptions.RequestException as e:
        window.time_label.setText(f"Error al insertar el registro: {e}")

    window.loading_label.setVisible(False)

# Función para eliminar un registro
def remove_record(window, filename, key, index="hash"):
    window.loading_label.setVisible(True)
    window.update()

    try:
        response = requests.delete(f"http://localhost:8080/remove?index={index}&filename={filename}&key={key}")
        response.raise_for_status()

        window.time_label.setText(f"Registro con clave {key} eliminado correctamente usando el índice {index}.")

    except requests.exceptions.RequestException as e:
        window.time_label.setText(f"Error al eliminar el registro: {e}")

    window.loading_label.setVisible(False)

# Función para crear un archivo que simula una tabla
def create_table(window, filename):
    try:
        with open(filename, 'w') as f:
            f.write("")  # Crear un archivo vacío

        window.time_label.setText(f"Tabla {filename} creada correctamente.")

    except IOError as e:
        window.time_label.setText(f"Error al crear la tabla: {e}")

# Función para actualizar la tabla en la interfaz con columnas dinámicas
def update_table(window):
    global current_page, columns

    window.table.setColumnCount(len(columns))  # Ajustar el número de columnas
    window.table.setHorizontalHeaderLabels(columns)  # Ajustar los nombres de las columnas

    window.table.setRowCount(0)

    start_index = current_page * page_size
    end_index = start_index + page_size
    for row_index, item in enumerate(data[start_index:end_index]):
        window.table.insertRow(row_index)
        for col_index, col_name in enumerate(columns):
            window.table.setItem(row_index, col_index, QTableWidgetItem(str(item[col_name])))

class SQLApp(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("SQL Parser con Backend C++")
        self.setGeometry(100, 100, 600, 600)

        layout = QVBoxLayout()

        # ComboBox para seleccionar el índice por defecto
        self.index_combo = QComboBox()
        self.index_combo.addItems(["avl", "btree", "hash"])
        layout.addWidget(self.index_combo)

        # Cuadro de texto para el comando SQL
        self.sql_input = QTextEdit()
        self.sql_input.setPlaceholderText("Escribe tu consulta SQL aquí...")
        layout.addWidget(self.sql_input)

        # Botón para ejecutar el comando SQL
        self.execute_button = QPushButton("Ejecutar SQL")
        self.execute_button.clicked.connect(lambda: sql_parser(self, self.sql_input.toPlainText()))
        layout.addWidget(self.execute_button)

        # Etiqueta para mostrar el tiempo
        self.time_label = QLabel("")
        layout.addWidget(self.time_label)

        # Tabla de datos
        self.table = QTableWidget()
        self.table.setEditTriggers(QTableWidget.NoEditTriggers)
        layout.addWidget(self.table)

        # Ícono de cargando
        self.loading_label = QLabel()
        self.loading_pixmap = QPixmap("loading.gif")  # Ruta a un gif o imagen de cargando
        self.loading_label.setPixmap(self.loading_pixmap)
        self.loading_label.setVisible(False)  # Oculto por defecto
        layout.addWidget(self.loading_label)

        # Paginación
        pagination_layout = QHBoxLayout()

        prev_button = QPushButton("Anterior")
        prev_button.clicked.connect(self.prev_page)
        pagination_layout.addWidget(prev_button)

        next_button = QPushButton("Siguiente")
        next_button.clicked.connect(self.next_page)
        pagination_layout.addWidget(next_button)

        layout.addLayout(pagination_layout)

        self.setLayout(layout)

    def prev_page(self):
        global current_page
        if current_page > 0:
            current_page -= 1
            update_table(self)

    def next_page(self):
        global current_page
        if current_page < len(data) // page_size:
            current_page += 1
            update_table(self)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = SQLApp()
    window.show()
    sys.exit(app.exec_())
