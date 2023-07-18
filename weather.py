import requests
import sys

def get_weather(city):
    api_key = '56c4c7794c184cef894173938231807'  # Remplacez par votre propre clé API météo

    base_url = 'http://api.weatherapi.com/v1/current.json'
    params = {
        'key': api_key,
        'q': city
    }

    response = requests.get(base_url, params=params)
    data = response.json()

    if 'error' in data:
        print("Erreur:", data['error']['message'])
    else:
        location = data['location']['name']
        country = data['location']['country']
        temp_c = data['current']['temp_c']
        condition = data['current']['condition']['text']

        print(f"Météo à {location}, {country}:")
        print(f"Température: {temp_c}°C")
        print(f"Condition: {condition}")


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Veuillez fournir le nom d'une ville en argument.")
    else:
        city_name = ' '.join(sys.argv[1:])
        get_weather(city_name)
