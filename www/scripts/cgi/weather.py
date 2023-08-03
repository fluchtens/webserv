import requests
import sys
import os
from urllib.parse import parse_qs

def get_weather(city_query_string):
    api_key = '56c4c7794c184cef894173938231807'

    base_url = 'http://api.weatherapi.com/v1/current.json'
    params = {
        'key': api_key,
        'q': city_query_string
    }

    response = requests.get(base_url, params=params)
    data = response.json()

    if 'error' in data:
        sys.stdout.write("Erreur: " + data['error']['message'])
    else:
        location = data['location']['name']
        country = data['location']['country']
        temp_c = data['current']['temp_c']
        condition = data['current']['condition']['text']

        html_content = "<!DOCTYPE html>\r\n"
        html_content += "<html lang=\"en\">\r\n"
        html_content += "<head>\r\n"
        html_content += "\t<meta charset=\"UTF-8\">\r\n"
        html_content += "\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\r\n"
        html_content += "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n"
        html_content += f"\t<title>Weather in {location}, {country}</title>\r\n"
        html_content += "</head>\r\n"
        html_content += "<body>\r\n"
        html_content += f"\t<h1>Weather in {location}, {country}</h1>\r\n"
        html_content += f"\t<p>Temperature: {temp_c}°C</p>\r\n"
        html_content += f"\t<p>Condition: {condition}</p>\r\n"
        html_content += "</body>\r\n"
        html_content += "</html>"

        sys.stdout.write(html_content)

if __name__ == '__main__':
    query_string = os.environ.get('QUERY_STRING', '')
    query_params = parse_qs(query_string)
    city_name = query_params.get('city', [''])[0]
    city_name = city_name.replace(',', ', ')
    get_weather(city_name)
