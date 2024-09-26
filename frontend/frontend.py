import sys
import time
import requests
from PyQt5.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QPushButton, QLabel, QTableWidget, QTableWidgetItem, QHBoxLayout, QComboBox, QLineEdit)
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import Qt

# Variables globales
page_size = 100
current_page = 0
data = []

# Función para obtener los datos desde el backend con el endpoint get_all
def fetch_data(window):
    window.loading_label.setVisible(True)
    window.update()

    start_time = time.time()

    try:
        index = window.index_combo.currentText()

        response = requests.get(f"http://localhost:8080/get_all?index={index}&filename=data.dat")
        response.raise_for_status()
        global data
        data = response.json()

        end_time = time.time()
        elapsed_time = end_time - start_time
        window.time_label.setText(f"Datos cargados en {elapsed_time:.2f} segundos")

        update_table(window)

    except requests.exceptions.RequestException as e:
        window.time_label.setText(f"Error al obtener los datos: {e}")

    window.loading_label.setVisible(False)

# Función para obtener datos con el endpoint get_between
def fetch_data_between(window):
    window.loading_label.setVisible(True)
    window.update()

    try:
        index = window.index_combo.currentText()
        start = window.start_input.text()
        end = window.end_input.text()

        response = requests.get(f"http://localhost:8080/get_between?index={index}&filename=data.dat&start={start}&end={end}")
        response.raise_for_status()
        global data
        data = response.json()

        window.time_label.setText(f"Datos entre {start} y {end} cargados.")
        update_table(window)

    except requests.exceptions.RequestException as e:
        window.time_label.setText(f"Error al obtener los datos: {e}")

    window.loading_label.setVisible(False)

# Función para obtener un registro con el endpoint get_record
def fetch_record(window):
    window.loading_label.setVisible(True)
    window.update()

    try:
        index = window.index_combo.currentText()
        key = window.key_input.text()

        response = requests.get(f"http://localhost:8080/get_record?index={index}&key={key}&filename=data.dat")
        response.raise_for_status()
        global data
        data = [response.json()]  # El resultado será un solo registro

        window.time_label.setText(f"Registro con clave {key} cargado.")
        update_table(window)

    except requests.exceptions.RequestException as e:
        window.time_label.setText(f"Error al obtener el registro: {e}")

    window.loading_label.setVisible(False)

# Función para insertar un nuevo registro
def insert_record(window):
    window.loading_label.setVisible(True)
    window.update()

    try:
        # Obtener los valores de los campos de entrada
        cod = window.cod_input.text()
        nombre = window.nombre_input.text()
        ciclo = window.ciclo_input.text()

        # Preparar el payload en formato JSON
        payload = {
            "cod": int(cod),
            "nombre": nombre,
            "ciclo": int(ciclo)
        }

        # Obtener el índice seleccionado
        index = window.index_combo.currentText()

        # Enviar el payload al backend con el parámetro index y filename
        response = requests.post(f"http://localhost:8080/insert?index={index}&filename=data.dat", json=payload)
        response.raise_for_status()

        window.time_label.setText(f"Registro insertado correctamente.")

    except requests.exceptions.RequestException as e:
        window.time_label.setText(f"Error al insertar el registro: {e}")

    window.loading_label.setVisible(False)

# Función para eliminar un registro
def remove_record(window):
    window.loading_label.setVisible(True)
    window.update()

    try:
        # Obtener los valores de la clave a eliminar, índice y nombre de archivo
        key = window.remove_key_input.text()
        index = window.index_combo.currentText()  # Obtener el índice seleccionado
        filename = "data.dat"

        # Enviar la solicitud de eliminación al backend
        response = requests.delete(f"http://localhost:8080/remove?index={index}&filename={filename}&key={key}")
        response.raise_for_status()

        window.time_label.setText(f"Registro con clave {key} eliminado correctamente.")

    except requests.exceptions.RequestException as e:
        window.time_label.setText(f"Error al eliminar el registro: {e}")

    window.loading_label.setVisible(False)

# Función para actualizar la tabla
def update_table(window):
    global current_page
    window.table.setRowCount(0)

    start_index = current_page * page_size
    end_index = start_index + page_size
    for row_index, item in enumerate(data[start_index:end_index]):
        window.table.insertRow(row_index)
        window.table.setItem(row_index, 0, QTableWidgetItem(str(item['cod'])))
        window.table.setItem(row_index, 1, QTableWidgetItem(item['nombre']))
        window.table.setItem(row_index, 2, QTableWidgetItem(str(item['ciclo'])))

def next_page(window):
    global current_page
    if current_page < len(data) // page_size:
        current_page += 1
        update_table(window)

def previous_page(window):
    global current_page
    if current_page > 0:
        current_page -= 1
        update_table(window)

class DataApp(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Datos desde C++ Backend")
        self.setGeometry(100, 100, 600, 600)

        layout = QVBoxLayout()

        # ComboBox para seleccionar el índice
        self.index_combo = QComboBox()
        self.index_combo.addItems(["avl", "btree", "hash"])
        layout.addWidget(self.index_combo)

        # Botón para actualizar los datos con get_all
        self.update_button = QPushButton("Obtener Todos los Datos")
        self.update_button.clicked.connect(lambda: fetch_data(self))
        layout.addWidget(self.update_button)

        # Campos para get_between
        self.start_input = QLineEdit()
        self.start_input.setPlaceholderText("Inicio (start)")
        self.end_input = QLineEdit()
        self.end_input.setPlaceholderText("Fin (end)")
        layout.addWidget(self.start_input)
        layout.addWidget(self.end_input)

        self.get_between_button = QPushButton("Obtener Datos entre Start y End")
        self.get_between_button.clicked.connect(lambda: fetch_data_between(self))
        layout.addWidget(self.get_between_button)

        # Campo para get_record
        self.key_input = QLineEdit()
        self.key_input.setPlaceholderText("Clave (key)")
        layout.addWidget(self.key_input)

        self.get_record_button = QPushButton("Obtener Registro por Clave")
        self.get_record_button.clicked.connect(lambda: fetch_record(self))
        layout.addWidget(self.get_record_button)

        # Sección para insertar un nuevo registro
        self.cod_input = QLineEdit()
        self.cod_input.setPlaceholderText("Código")
        layout.addWidget(self.cod_input)

        self.nombre_input = QLineEdit()
        self.nombre_input.setPlaceholderText("Nombre")
        layout.addWidget(self.nombre_input)

        self.ciclo_input = QLineEdit()
        self.ciclo_input.setPlaceholderText("Ciclo")
        layout.addWidget(self.ciclo_input)

        self.insert_button = QPushButton("Insertar Registro")
        self.insert_button.clicked.connect(lambda: insert_record(self))
        layout.addWidget(self.insert_button)

        # Sección para eliminar un registro
        self.remove_key_input = QLineEdit()
        self.remove_key_input.setPlaceholderText("Clave a eliminar")
        layout.addWidget(self.remove_key_input)

        self.remove_button = QPushButton("Eliminar Registro")
        self.remove_button.clicked.connect(lambda: remove_record(self))
        layout.addWidget(self.remove_button)

        # Etiqueta para mostrar el tiempo
        self.time_label = QLabel("")
        layout.addWidget(self.time_label)

        # Tabla de datos
        self.table = QTableWidget()
        self.table.setColumnCount(3)
        self.table.setHorizontalHeaderLabels(["Código", "Nombre", "Ciclo"])
        self.table.setEditTriggers(QTableWidget.NoEditTriggers)
        self.table.setColumnWidth(0, 100)
        self.table.setColumnWidth(1, 250)
        self.table.setColumnWidth(2, 100)
        layout.addWidget(self.table)

        # Ícono de "cargando"
        self.loading_label = QLabel(self)
        pixmap = QPixmap("loading.gif").scaled(50, 50, Qt.KeepAspectRatio)
        self.loading_label.setPixmap(pixmap)
        self.loading_label.setVisible(False)
        layout.addWidget(self.loading_label)

        # Botones de navegación
        button_layout = QHBoxLayout()
        self.previous_button = QPushButton("Anterior")
        self.previous_button.clicked.connect(lambda: previous_page(self))
        self.next_button = QPushButton("Siguiente")
        self.next_button.clicked.connect(lambda: next_page(self))
        button_layout.addWidget(self.previous_button)
        button_layout.addWidget(self.next_button)
        layout.addLayout(button_layout)

        self.setLayout(layout)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = DataApp()
    window.show()
    sys.exit(app.exec_())
