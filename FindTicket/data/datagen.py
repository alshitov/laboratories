"""
Module: datagen.py;
Generates JSON-formatted data for FindTicket application:
avia_tickets, railway_tickets, bus_tickets, train_tickets.
"""

import os
from sys import path, argv
__location__ = os.path.realpath(os.path.join(os.getcwd(), os.path.dirname(__file__)))
path.append(__location__)
from random import randint
from copy import deepcopy
import json
import xlrd
import wikiparser as wp


def _load_sources() -> dict:
    """ Function loads sources for tickets generators """
    with open(os.path.join(__location__, 'sources.json')) as fout:
        return json.load(fout)


sources = _load_sources()


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
places = sorted(list(distances.keys()))     # all
# Cities for airplane tickets
cities_w_airport = wp.get_cities_with_airports()
air_tickets_cities = [city for city in places if city in cities_w_airport]


def _calc_approximate_travel_time(transport_type, depart, dest) -> float:
    """
    Calculate approximate travel time according
    to transport type and the distance between
    department point and destination point
    """
    # Get the distance (in km)
    distance = distances[depart][dest]  # float

    # Get transport characteristics
    characteristics = sources[transport_type]
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
        return '%d/%d/%d - %d:%d' % (dep_month, dep_day, dep_year, arr_h, arr_m)

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
                dep_month,
                dep_day + all_d,
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
                    dep_month + 1,
                    (dep_day + all_d) - 30,
                    dep_year,
                    arr_t // 60,
                    arr_t % 60
                )

            else:
                return '%d/%d/%d - %d:%d' % (
                    (dep_month + 1) % 12,
                    (dep_day + all_d) - 30,
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
    base_cost = sources[transport_type]['base_cost']
    rub_per_km = sources[transport_type]['rub_per_km']
    return base_cost + (distance * rub_per_km)


def _ticketgen(transport: str, source: dict, amount: int) -> [dict]:
    """ Function generates _amount_ of random each ticket types.
    Ticket fields:
        -> company
        -> departure_place
        -> arrival_place
        -> departure_datetime
        -> arrival_datetime
        -> distance
        -> price
        -> price_currency
        -> features
        -> rate
        -> type (speed)
        -> sales (adult, student, elder, child, baby)
    """
    gen_result = {}

    if transport == 'avia':
        places = air_tickets_cities
    else:
        global places

    for idx in range(amount):
        # Generate fields
        departure_place = _random_from_list(places, 1)
        arrival_place = _random_from_list(places, 1)
        while departure_place == arrival_place:
            arrival_place = _random_from_list(places, 1)

        if transport == 'bus':
            company = departure_place + 'Авто'
        else:
            company = _random_from_list(source['companies'], 1)

        departure_datetime, arrival_datetime = _dep_arr_time_randgen(
            transport,
            departure_place,
            arrival_place
        )
        distance = distances[departure_place][arrival_place]
        price = _price_calc(
            transport,
            distance
        )
        rate = _random_from_list(source['rates'], 1)

        if rate in source['rates_with_features']:
            _features = source['features']
            features = _random_from_list(_features, randint(1, len(_features) - 1))
        else:
            features = []

        _type = _random_from_list(source['types'], 1)

        ticket = {
            'company': company,
            'departure_place': departure_place,
            'departure_datetime': departure_datetime,
            'arrival_place': arrival_place,
            'arrival_datetime': arrival_datetime,
            'distance': distance,
            'price': price,
            'price_currency': 'rub',
            'features': features,
            'rate': rate,
            'type': _type,
        }

        # Generating additional fields (sales)
        _sales = source['sales']
        sales = {}

        if _sales:
            for field in _sales:
                _field_low, _field_high = _sales[field]
                sales[field] = randint(_field_low, _field_high)

        ticket['sales'] = sales
        # Original ticket
        # gen_result[idx] = ticket

        # Generate tickets with nearby dates (+30 tickets in same month)
        time = departure_datetime.split(' - ')[1]
        month, day, year = departure_datetime.split(' - ')[0].split('/')
        for i in range(1, 31):
            copy = deepcopy(ticket)
            copy['departure_datetime'] = '{0}/{1}/{2} - {3}'.format(
                i,
                month,
                year,
                time
            )
            gen_result[str(idx) + str(i)] = copy
    return gen_result


def ticketgen(amount: int):
    """
    Call point.
    Function generates random tickets for all transport types.
    """
    # Load sources from what to generate random tickets
    sources = _load_sources()
    transports = list(sources.keys())

    # Generate all types of tickets
    tickets = {}
    for transport in transports:
        try:
            _source = sources[transport]
        except KeyError:
            raise Exception('Invalid "%s" key of sources.json' % transport)

        if _source:
            _tickets = _ticketgen(transport, _source, amount)

            tickets[transport] = _tickets

    # Make dump for application
    # if tickets:
    #     _make_dump(tickets)


def _make_dump(tickets: dict):
    """ Dump each of tickets groups to eponymous .json """
    for group in tickets:
        try:
            FILE = open('./%s.json' % group, 'w')
        except FileNotFoundError:
            raise Exception('Invalid tickets group name: %s' % group)

        if FILE:
            json.dump(tickets[group], FILE, ensure_ascii=False)
            FILE.close()


def _force_quit():
    exit('Please, specify the amount of tickets to generate (must be int)')


if __name__ == '__main__':
    if len(argv) == 2:
        try:
            int(argv[1])
        except ValueError:
            _force_quit()
    else:
        _force_quit()

    ticketgen(int(argv[1]))
