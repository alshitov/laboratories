"""
Module: datagen.py;
Generates JSON-formatted data for FindTicket application:
avia_tickets, railway_tickets, bus_tickets, train_tickets.
"""

import os
__location__ = os.path.realpath(os.path.join(os.getcwd(), os.path.dirname(__file__)))
from sys import argv
from random import randint
import json
import xlrd


# Average Transport Speed (km/h) And Delays
TRANSPORTS = {
    'plain': {
        'speed': 900.0,
        'delay': 0.5,
        'delay_measure': 'h',
        'base_cost': 2000,
        'rub_per_km': 2
    },
    'train': {
        'speed': 80.0,
        'delay': 30.0,
        'delay_measure': '%',
        'base_cost': 1500,
        'rub_per_km': 1.5
    },
    'bus': {
        'speed': 90.0,
        'delay': 20.0,
        'delay_measure': '%',
        'base_cost': 1500,
        'rub_per_km': 1
    }
}


def _load_sources() -> dict:
    """ Function loads sources for tickets generators """
    with open(os.path.join(__location__, 'sources.json')) as fout:
        return json.load(fout)


def _load_cities_distances() -> [list]:
    """ The distances between cities are saved in .xls format """
    # Will hold 'city_name': {'other_city_1_name': distance_1_km, ...} mapping
    distances = {}

    workbook = xlrd.open_workbook(
        os.path.join(__location__, 'distances.xls'))
    sheet = workbook.sheet_by_index(sheetx=0)

    for i in range(1, sheet.nrows):
        city_name = sheet.cell(i, 0).value
        city_info = {}

        for j in range(1, sheet.ncols):
            next_city = sheet.cell(0, j).value
            city_info[next_city] = sheet.cell(i, j).value

        distances[city_name] = city_info
    return distances


distances = _load_cities_distances()
places = sorted(list(distances.keys()))


def _calc_approximate_travel_time(transport_type, depart, dest) -> float:
    """
    Calculate approximate travel time according
    to transport type and the distance between
    department point and destination point
    """
    # Get the distance (in km)
    distance = distances[depart][dest]  # float

    # Get transport characteristics
    characteristics = TRANSPORTS[transport_type]
    speed = characteristics['speed']
    delay = characteristics['delay']
    delay_measure = characteristics['delay_measure']

    # Calculate full-way time
    landing_time = distance / speed
    delay_time = delay if delay_measure == 'h' else landing_time * (delay / 100)

    return round((landing_time + delay_time), 2)


def _summarize_time(departure_datetime: str, travel_time: float) -> str:
    """ Calculate arriving time by adding
    landing_time to departure datetime.
    The better idea would be to use library like time of datetime
    instead of perform calculations in a hardcoded way. """
    # Parse departure date & time from datetime string: 'DD/MM/YYYY - HH:MM'
    dep_day, dep_month, dep_year = [int(n) for n in departure_datetime.split(' - ')[0].split('/')]
    dep_h, dep_m = [int(n) for n in departure_datetime.split(' - ')[1].split(':')]
    dep_t = dep_h * 60 + dep_m

    # Translate travel_time (in hours) to minutes
    travel_m = travel_time * 60

    sum_m = dep_t + travel_m

    # Clause when travel ends today
    if sum_m < 1440:
        arr_h = sum_m // 60
        arr_m = sum_m % 60
        return '%d/%d/%d - %d:%d' % (dep_day, dep_month, dep_year, arr_h, arr_m)

    # Travel ends not today;
    # To simplify the task, let's say every month is 30 days long
    else:
        travel_d = travel_m // 1440     # Days while travel
        travel_m = (travel_m % 1440)    # Leftover minutes while travel
        arr_m = dep_t + travel_m        # Arriving minutes
        add_d = arr_m // 1440           # Arriving minutes > 1440
        arr_t = arr_m - (1440 * add_d)  # Calc arriving time
        all_d = travel_d + add_d        # Arriving day

        # If added days does not entail month change
        if dep_day + all_d <= 30:
            return '%d/%d/%d - %d:%d' % (
                dep_day + all_d,
                dep_month,
                dep_year,
                arr_t // 60,
                arr_t % 60
            )

        # To simplify the task, let's say there is no travel longer than 1 month
        else:
            dep_month += 1

            # If added month does not entail year change
            if dep_month <= 12:
                return '%d/%d/%d - %d:%d' % (
                    (dep_day + all_d) - 30,
                    dep_month + 1,
                    dep_year,
                    arr_t // 60,
                    arr_t % 60
                )

            else:
                return '%d/%d/%d - %d:%d' % (
                    (dep_day + all_d) - 30,
                    (dep_month + 1) % 12,
                    dep_year + 1,
                    arr_t // 60,
                    arr_t % 60
                )


def _dep_arr_time_randgen(transport_type: str, depart: str, dest: str) -> (str, str):
    """
    Function generates random departure datetime
    and calculates approximate arrival datetime
    from depart to dest according to given transport type
    """
    dep_datetime = '%d/%d/%d - %d:%d' % (
        randint(1, 30),
        randint(1, 12),
        randint(2019, 2020),
        randint(0, 24),
        randint(0, 60)
    )
    travel_time = _calc_approximate_travel_time(transport_type, depart, dest)
    print(travel_time)
    arr_datetime = _summarize_time(dep_datetime, travel_time)

    return dep_datetime, arr_datetime


def _random_from_list(choices: [str], amount):
    """ Function returns _amount_ of random string from given list """
    assert amount <= len(choices)

    if amount == 1:
        return choices[randint(0, len(choices) - 1)]

    else:
        # Divide choices into _amount_ of length-equal pieces
        count = len(choices) // amount
        chunks = [choices[i: (i + count)] for i in range(0, len(choices), count)]
        # Get random element from each chunk
        return [choices[randint(0, len(ch))] for ch in chunks]


def _price_calc(transport_type: str, distance: float) -> float:
    """ Function calculates approximate travel cost """
    base_cost = TRANSPORTS[transport_type]['base_cost']
    rub_per_km = TRANSPORTS[transport_type]['rub_per_km']
    return base_cost + (distance * rub_per_km)


def _aviagen(avia_source: dict, amount: int) -> [dict]:
    """ Function generates _amount_ of random airplane ticket """
    transport_type = 'plain'
    gen_result = []

    for _ in range(amount):
        # Generate fields
        company = _random_from_list(avia_source['companies'], 1)

        departure_place = _random_from_list(places, 1)
        arrival_place = _random_from_list(places, 1)
        while departure_place == arrival_place:
            arrival_place = _random_from_list(places, 1)

        departure_datetime, arrival_datetime = _dep_arr_time_randgen(
            transport_type,
            departure_place,
            arrival_place
        )
        distance = distances[departure_place][arrival_place]
        price = _price_calc(
            transport_type,
            distance
        )
        rate = _random_from_list(avia_source['rates'], 1)
        if rate in ("Первый", "Бизнес"):
            features = _random_from_list(avia_source['features'], randint(1, 3))
        else:
            features = []

        adult_sale = randint(0, 20)
        child_sale = randint(20, 40)
        baby_sale = randint(40, 60)

        gen_result.append({
            'company': company,
            'departure_place': departure_place,
            'departure_datetime': departure_datetime,
            'arrival_place': arrival_place,
            'arrival_datetime': arrival_datetime,
            'distance': distance,
            'price': price,
            'price_currency': 'rub',
            'features': features,
            'adult_sale': adult_sale,
            'child_sale': child_sale,
            'baby_sale': baby_sale,
        })

    return gen_result


def _railwaygen(railway_source: dict, amount: int) -> [dict]:
    """ Function generates _amount_ of random railway tickets.
    Fields:
    company: str,
    departure_place: str,
    departure_datetime: str,
    arrival_place: str,
    arrival_datetime: str,
    price: float,
    price_currency: str,
    # Private
    train_type: str,
    features: list,
    rate: str):
    """
    pass


def _busgen(bus_source: dict, amount: int) -> [dict]:
    """ Function generates _amount_ of random bus tickets.
    Fields:
    company: str,
    departure_place: str,
    departure_datetime: str,
    arrival_place: str,
    arrival_datetime: str,
    price: float,
    price_currency: str,
    # Private
    bus_type: str,
    features: list,
    rate: str
    """
    pass


def _traingen(train_source: dict, amount: int) -> [dict]:
    """ Function generates _amount_ of random train tickets.
    Fields:
    company: str,
    departure_place: str,
    departure_datetime: str,
    arrival_place: str,
    arrival_datetime: str,
    price: float,
    price_currency: str,
    # Private
    train_type: str
    """
    pass


def ticketgen(amount: int):
    """
    Call point.
    Function generates random tickets for all transport types.
    """
    # Load sources from what to generate random tickets
    sources = _load_sources()

    # Generate all types of tickets
    mapping = {
        'avia': _aviagen,
        # 'railway': _railwaygen,
        # 'bus': _busgen,
        # 'train': _traingen,
    }
    tickets = {}
    for group in mapping:
        try:
            _source = sources[group]
        except KeyError:
            raise Exception('Invalid "%s" key of sources.json' % group)

        if _source:
            _tickets = mapping[group](_source, amount)
            tickets[group] = _tickets

    import pprint
    pprint.pprint(tickets)

    # Make dump for application
    # if tickets:
    #     _make_dump(tickets)


def _make_dump(tickets: dict):
    """ Dump each of tickets groups to eponymous .json """
    for group in tickets:
        try:
            FILE = open('./%s.json' % group)
        except FileNotFoundError:
            raise Exception('Invalid tickets group name: %s' % group)

        if FILE:
            json.dump(tickets[group], FILE)
            FILE.close()


def _force_quit():
    exit('Please, specify the amount of tickets to generate (must be int)')


if __name__ == '__main__':
    if len(argv) == 2:
        try:
            amount = int(argv[1])
            ticketgen(amount)
        except ValueError:
            _force_quit()
    else:
        _force_quit()
