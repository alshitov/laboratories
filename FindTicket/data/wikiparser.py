import requests
from bs4 import BeautifulSoup


def get_html(url):
    return requests.get(url).content.decode('utf-8')


def parse_cities(soup) -> list:
    cities = []
    content_holder = soup.find_all('table')[0]
    rows = content_holder.find_all('tr')

    for index in range(1, len(rows)):
        airport_info = rows[index]
        city_cell = airport_info.find_all('td')[5]

        try:
            cities.append(city_cell.find('a').text)
        except AttributeError:
            pass
    return sorted(list(set(cities)))


def get_cities_with_airports():
    url = 'https://ru.wikipedia.org/wiki/%D0%A1%D0%BF%D0%B8%D1%81%D0%BE%D0%BA_' \
          '%D0%B0%D1%8D%D1%80%D0%BE%D0%BF%D0%BE%D1%80%D1%82%D0%BE%D0%B2_%D0%A0%D0%BE%D1%81%D1%81%D0%B8%D0%B8'
    html = get_html(url)
    soup = BeautifulSoup(html, 'html.parser')
    cities_with_airports = parse_cities(soup)
    return cities_with_airports
