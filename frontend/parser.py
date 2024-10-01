import sys
import requests
import time
from PyQt5 import QtWidgets

class SQLParserWindow(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("SQL Parser")
        self.setGeometry(100, 100, 600, 400)

        # Layout
        self.layout = QtWidgets.QVBoxLayout(self)

        # ComboBox para seleccionar tipo de índice
        self.index_combo = QtWidgets.QComboBox(self)
        self.index_combo.addItems(["avl", "extendible", "sequential"])
        self.layout.addWidget(self.index_combo)

        # Input para la consulta SQL
        self.sql_input = QtWidgets.QTextEdit(self)
        self.sql_input.setPlaceholderText("Escriba su sentencia SQL aquí...")
        self.layout.addWidget(self.sql_input)

        # Botón de Ejecutar
        self.execute_button = QtWidgets.QPushButton("Ejecutar")
        self.execute_button.clicked.connect(self.execute_sql)
        self.layout.addWidget(self.execute_button)

        # Etiqueta de carga
        self.loading_label = QtWidgets.QLabel("Cargando...", self)
        self.loading_label.setVisible(False)
        self.layout.addWidget(self.loading_label)

        # Tabla de Resultados
        self.result_table = QtWidgets.QTableWidget(self)
        self.layout.addWidget(self.result_table)

        # Etiqueta de tiempo
        self.time_label = QtWidgets.QLabel("", self)
        self.layout.addWidget(self.time_label)

    def execute_sql(self):
        query = self.sql_input.toPlainText().strip()
        index_type = self.index_combo.currentText()  # Obtener el índice seleccionado
        self.loading_label.setVisible(True)
        self.update()

        start_time = time.time()

        try:
            if query.startswith("SELECT"):
                parts = query.split(" ")

                if len(parts) < 4:
                    self.time_label.setText("Formato de consulta no válido.")
                    return

                table_name = parts[3]  # Nombre de la tabla

                if "WHERE" in parts:
                    where_index = parts.index("WHERE")

                    if len(parts) > where_index + 1:
                        condition = ' '.join(parts[where_index + 1:]).strip()

                        key_condition = condition.split('=')
                        if len(key_condition) == 2:
                            key = key_condition[0].strip()  # "key"
                            value = key_condition[1].strip()  # "1500"

                            print(f"Valor de key: '{value}'")  # Para depuración

                            if key.lower() == "key":
                                url = f"http://localhost:8080/{index_type}/get_record?table_name={table_name}&key={value}"
                                print(f"URL construida: {url}")  # Para depuración
                                response = requests.get(url)

                                # Diagnóstico adicional
                                print(f"Código de estado de la respuesta: {response.status_code}")
                                print(f"Contenido de la respuesta: {response.text}")

                                response.raise_for_status()  # Levanta error para códigos 4xx y 5xx
                                result = response.json()

                                # Comprobar si 'results' existe en la respuesta
                                if "results" in result:
                                    end_time = time.time()
                                    elapsed_time_ms = (end_time - start_time) * 1000  # Convertir a milisegundos
                                    self.time_label.setText(f"Datos cargados en {elapsed_time_ms:.2f} ms")

                                    self.update_table([result["results"]])  # Pasar solo el resultado
                                else:
                                    self.time_label.setText("No se encontraron resultados.")
                                    print("Respuesta sin resultados:", result)

                            else:
                                self.time_label.setText("Condición WHERE no válida. Debe ser 'key=valor'.")
                                return
                        else:
                            self.time_label.setText("Falta condición en la cláusula WHERE.")
                            return
                    else:
                        self.time_label.setText("Falta condición en la cláusula WHERE.")
                        return
                else:
                    # Si no hay WHERE, obtenemos todos los registros
                    response = requests.get(f"http://localhost:8080/{index_type}/get_all?table_name={table_name}")

                    # Diagnóstico adicional
                    print(f"Código de estado de la respuesta: {response.status_code}")
                    print(f"Contenido de la respuesta: {response.text}")

                    response.raise_for_status()  # Levanta error para códigos 4xx y 5xx
                    result = response.json().get("results", [])

                    end_time = time.time()
                    elapsed_time_ms = (end_time - start_time) * 1000  # Convertir a milisegundos
                    self.time_label.setText(f"Datos cargados en {elapsed_time_ms:.2f} ms")

                    self.update_table(result)

            else:
                self.time_label.setText("Solo se admite SELECT para consultas.")
                return

        except requests.exceptions.RequestException as e:
            self.time_label.setText("Error en la conexión con la API.")
            print(f"Error en la conexión: {e}")
            if 'response' in locals():  # Verificar si 'response' existe
                print(f"Respuesta del servidor: {response.text}")  # Imprimir el texto de la respuesta
        except ValueError as e:
            self.time_label.setText("Error al procesar la respuesta de la API.")
            print(f"Error de valor: {e}")
        except Exception as e:
            self.time_label.setText("Error inesperado.")
            print(f"Error inesperado: {e}")  # Imprimir el mensaje de error completo
        finally:
            self.loading_label.setVisible(False)

    def update_table(self, data):
        # Limpiar la tabla antes de actualizar
        self.result_table.setRowCount(0)
        self.result_table.setColumnCount(0)

        if not data:
            self.time_label.setText("No se encontraron datos.")
            return
        
        # Configurar el número de filas y columnas
        self.result_table.setRowCount(len(data))
        self.result_table.setColumnCount(len(data[0]) if data else 0)

        # Set header labels
        headers = data[0].keys()
        self.result_table.setHorizontalHeaderLabels(headers)

        for row_idx, row_data in enumerate(data):
            for col_idx, (key, value) in enumerate(row_data.items()):
                self.result_table.setItem(row_idx, col_idx, QtWidgets.QTableWidgetItem(str(value)))

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = SQLParserWindow()
    window.show()
    sys.exit(app.exec_())
