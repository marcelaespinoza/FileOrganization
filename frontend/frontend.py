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

# Función para obtener todos los datos
def fetch_all_data(window):
    window.loading_label.setVisible(True)
    window.update()

    start_time = time.time()

    try:
        table_name = window.table_combo.currentText()

        response = requests.get(f"http://localhost:8080/avl/get_all?table_name={table_name}")
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

# Función para obtener datos entre dos claves
def fetch_data_between(window):
    window.loading_label.setVisible(True)
    window.update()

    try:
        table_name = window.table_combo.currentText()
        start = window.start_input.text()
        end = window.end_input.text()

        response = requests.get(f"http://localhost:8080/avl/get_between?table_name={table_name}&start={start}&end={end}")
        response.raise_for_status()
        global data
        data = response.json()

        window.time_label.setText(f"Datos entre {start} y {end} cargados.")
        update_table(window)

    except requests.exceptions.RequestException as e:
        window.time_label.setText(f"Error al obtener los datos: {e}")

    window.loading_label.setVisible(False)

# Función para obtener un registro específico
def fetch_record(window):
    window.loading_label.setVisible(True)
    window.update()

    try:
        table_name = window.table_combo.currentText()
        key = window.key_input.text()

        response = requests.get(f"http://localhost:8080/avl/get_record?table_name={table_name}&key={key}")
        response.raise_for_status()
        global data
        data = [response.json()]

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
        table_name = window.table_combo.currentText()
        key = int(window.key_input.text())
        game_title = window.game_title_input.text()
        platform = window.platform_input.text()
        year = int(window.year_input.text())
        genre = window.genre_input.text()
        publisher = window.publisher_input.text()
        north_america = float(window.na_input.text())
        europe = float(window.eu_input.text())
        japan = float(window.jp_input.text())
        rest_of_world = float(window.row_input.text())
        global_sales = float(window.global_input.text())
        review = float(window.review_input.text())

        # Preparar el payload en formato JSON
        payload = {
            "key": key,
            "Rank": key,
            "GameTitle": game_title,
            "Platform": platform,
            "Year": year,
            "Genre": genre,
            "Publisher": publisher,
            "NorthAmerica": north_america,
            "Europe": europe,
            "Japan": japan,
            "RestOfWorld": rest_of_world,
            "Global": global_sales,
            "Review": review
        }

        # Enviar el payload al backend
        response = requests.post(f"http://localhost:8080/avl/post_record?table_name={table_name}", json=payload)
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
        key = window.remove_key_input.text()
        table_name = window.table_combo.currentText()

        response = requests.delete(f"http://localhost:8080/avl/delete_record?table_name={table_name}&key={key}")
        response.raise_for_status()

        window.time_label.setText(f"Registro con clave {key} eliminado correctamente.")

    except requests.exceptions.RequestException as e:
        window.time_label.setText(f"Error al eliminar el registro: {e}")

    window.loading_label.setVisible(False)

# Función para leer un CSV e insertarlo en la tabla
def read_csv(window):
    window.loading_label.setVisible(True)
    window.update()

    try:
        csv_path = window.csv_path_input.text()
        table_name = window.table_combo.currentText()

        payload = {"csv_path": csv_path}

        response = requests.post(f"http://localhost:8080/avl/read_csv?table_name={table_name}", json=payload)
        response.raise_for_status()

        window.time_label.setText(f"Datos del CSV cargados en la tabla {table_name}.")

    except requests.exceptions.RequestException as e:
        window.time_label.setText(f"Error al leer el CSV: {e}")

    window.loading_label.setVisible(False)

# Función para actualizar la tabla
def update_table(window):
    global current_page
    window.table.setRowCount(0)

    start_index = current_page * page_size
    end_index = start_index + page_size
    for row_index, item in enumerate(data[start_index:end_index]):
        window.table.insertRow(row_index)
        window.table.setItem(row_index, 0, QTableWidgetItem(str(item['key'])))
        window.table.setItem(row_index, 1, QTableWidgetItem(item['GameTitle']))
        window.table.setItem(row_index, 2, QTableWidgetItem(str(item['Year'])))

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

        self.setWindowTitle("Datos AVL Backend")
        self.setGeometry(100, 100, 800, 600)

        layout = QVBoxLayout()

        # ComboBox para seleccionar la tabla
        self.table_combo = QComboBox()
        self.table_combo.addItems(["game", "employee"])
        layout.addWidget(self.table_combo)

        # Botón para obtener todos los datos
        self.get_all_button = QPushButton("Obtener Todos los Datos")
        self.get_all_button.clicked.connect(lambda: fetch_all_data(self))
        layout.addWidget(self.get_all_button)

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

        # Campo para obtener registro por clave
        self.key_input = QLineEdit()
        self.key_input.setPlaceholderText("Clave (key)")
        layout.addWidget(self.key_input)

        self.get_record_button = QPushButton("Obtener Registro por Clave")
        self.get_record_button.clicked.connect(lambda: fetch_record(self))
        layout.addWidget(self.get_record_button)

        # Campos para insertar un nuevo registro
        self.game_title_input = QLineEdit()
        self.game_title_input.setPlaceholderText("Título del Juego")
        layout.addWidget(self.game_title_input)

        self.platform_input = QLineEdit()
        self.platform_input.setPlaceholderText("Plataforma")
        layout.addWidget(self.platform_input)

        self.year_input = QLineEdit()
        self.year_input.setPlaceholderText("Año")
        layout.addWidget(self.year_input)

        self.genre_input = QLineEdit()
        self.genre_input.setPlaceholderText("Género")
        layout.addWidget(self.genre_input)

        self.publisher_input = QLineEdit()
        self.publisher_input.setPlaceholderText("Publicador")
        layout.addWidget(self.publisher_input)

        self.na_input = QLineEdit()
        self.na_input.setPlaceholderText("Ventas en América del Norte")
        layout.addWidget(self.na_input)

        self.eu_input = QLineEdit()
        self.eu_input.setPlaceholderText("Ventas en Europa")
        layout.addWidget(self.eu_input)

        self.jp_input = QLineEdit()
        self.jp_input.setPlaceholderText("Ventas en Japón")
        layout.addWidget(self.jp_input)

        self.row_input = QLineEdit()
        self.row_input.setPlaceholderText("Ventas en el Resto del Mundo")
        layout.addWidget(self.row_input)

        self.global_input = QLineEdit()
        self.global_input.setPlaceholderText("Ventas Globales")
        layout.addWidget(self.global_input)

        self.review_input = QLineEdit()
        self.review_input.setPlaceholderText("Reseñas")
        layout.addWidget(self.review_input)

        self.insert_button = QPushButton("Insertar Registro")
        self.insert_button.clicked.connect(lambda: insert_record(self))
        layout.addWidget(self.insert_button)

        # Campo para eliminar registro por clave
        self.remove_key_input = QLineEdit()
        self.remove_key_input.setPlaceholderText("Clave para eliminar")
        layout.addWidget(self.remove_key_input)

        self.remove_button = QPushButton("Eliminar Registro")
        self.remove_button.clicked.connect(lambda: remove_record(self))
        layout.addWidget(self.remove_button)

        # Campo para leer CSV e insertar en la tabla
        self.csv_path_input = QLineEdit()
        self.csv_path_input.setPlaceholderText("Ruta del archivo CSV")
        layout.addWidget(self.csv_path_input)

        self.read_csv_button = QPushButton("Leer CSV")
        self.read_csv_button.clicked.connect(lambda: read_csv(self))
        layout.addWidget(self.read_csv_button)

        # Etiqueta de carga
        self.loading_label = QLabel()
        pixmap = QPixmap("loading.gif")
        self.loading_label.setPixmap(pixmap)
        self.loading_label.setVisible(False)
        layout.addWidget(self.loading_label)

        # Etiqueta para el tiempo de carga
        self.time_label = QLabel("Tiempo de carga:")
        layout.addWidget(self.time_label)

        # Tabla para mostrar los datos
        self.table = QTableWidget()
        self.table.setColumnCount(3)
        self.table.setHorizontalHeaderLabels(["Clave", "Título del Juego", "Año"])
        layout.addWidget(self.table)

        # Botones de paginación
        pagination_layout = QHBoxLayout()
        self.prev_button = QPushButton("Anterior")
        self.prev_button.clicked.connect(lambda: previous_page(self))
        pagination_layout.addWidget(self.prev_button)

        self.next_button = QPushButton("Siguiente")
        self.next_button.clicked.connect(lambda: next_page(self))
        pagination_layout.addWidget(self.next_button)
        layout.addLayout(pagination_layout)

        self.setLayout(layout)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = DataApp()
    window.show()
    sys.exit(app.exec_())
