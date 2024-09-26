import requests

# URL de la API
base_url = "http://127.0.0.1:8000"

# Llamar al endpoint de verificación de conexión
response = requests.get(f"{base_url}/")
if response.status_code == 200:
    print("Conexión exitosa:", response.json())

# Llamar al endpoint /call_inorder
filename = "data.dat"  # Asegúrate de que este archivo exista y sea accesible
response = requests.get(f"{base_url}/call_inorder?filename={filename}")

if response.status_code == 200:
    records = response.json()  # Recibir los registros como JSON
    print("Registros recibidos:")
    for record in records:
        print(record)
else:
    print("Error al obtener registros:", response.status_code, response.text)

