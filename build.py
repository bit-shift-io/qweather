#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import sys
import subprocess
import inspect
import multiprocessing
import shutil
import getpass

config = {
    'display_name': 'weather',
    'logToFile': False,
    'path': {
        'cwd': os.path.abspath(os.getcwd()),
        'bin': os.path.abspath('./build/src/qweather'),
        'build': os.path.abspath('./build/'),
    },
}


def main():
    menu = {}
    menu['1'] = ['Build release', 'build_release']
    menu['2'] = ['Build debug', 'build_debug']
    menu['3'] = ['Run desktop', 'run_desktop']
    menu['4'] = ['Run mobile', 'run_mobile']
    menu['5'] = ['Debug (GDB)', 'debug']
    menu['d'] = ['Generate database', 'database']
    menu['0'] = ['Requirements', 'requirements']

    print('\n********************')
    print ('    {}'.format(config['display_name']))
    print('********************')
    for item in menu:
        print (' ' + item + '. ' + menu[item][0])
        
    selection = input('> ')
    # check if in menu
    if selection in menu:
        eval(menu[selection][1] + '()')

    # exec function
    if '()' in selection:
        eval(selection)

    main()
    return


def database():
    # pip install bs4
    # pip install lxml
    # pip install dbf
    import json
    import requests
    from bs4 import BeautifulSoup
    import re
    import urllib
    import dbf
    import math

    observation_data = [
        'http://www.bom.gov.au/sa/observations/saall.shtml',
        'http://reg.bom.gov.au/act/observations/canberra.shtml',
        'http://www.bom.gov.au/nsw/observations/nswall.shtml',
        'http://www.bom.gov.au/nt/observations/ntall.shtml',
        'http://www.bom.gov.au/qld/observations/qldall.shtml',
        'http://www.bom.gov.au/tas/observations/tasall.shtml',
        'http://www.bom.gov.au/vic/observations/vicall.shtml',
        'http://www.bom.gov.au/wa/observations/waall.shtml',
    ]

    area_data = [
        'ftp://ftp.bom.gov.au/anon/home/adfd/spatial/IDM00013.dbf'
    ]

    # http://www.bom.gov.au/australia/radar/about/radar_site_info.shtml
    radar_data = [
        'http://www.bom.gov.au/australia/radar/info/nsw_info.shtml',
        'http://www.bom.gov.au/australia/radar/info/vic_info.shtml',
        'http://www.bom.gov.au/australia/radar/info/qld_info.shtml',
        'http://www.bom.gov.au/australia/radar/info/wa_info.shtml',
        'http://www.bom.gov.au/australia/radar/info/sa_info.shtml',
        'http://www.bom.gov.au/australia/radar/info/tas_info.shtml',
        'http://www.bom.gov.au/australia/radar/info/nt_info.shtml',
    ]

    # http://www.bom.gov.au/catalogue/data-feeds.shtml#forecasts
    # we only need the precis short form data
    # ftp://ftp.bom.gov.au/anon/gen/fwo/[id].xml
    forecast_data = [
        'IDN11060',
        'IDD10207',
        'IDQ11295',
        'IDS10044',
        'IDT16710',
        'IDV10753',
        'IDW14199',
    ]


    # function to get distance between lat_lon
    def get_distance(ax, ay, bx, by):
        distance = math.sqrt( ((ax-bx)**2)+((ay-by)**2) )
        return distance


    # radar site data
    radar_site_data = []
    lat_lon_pattern = re.compile("([0-9]{1,3}[.][0-9]{1,3})")
    for url in radar_data:
        page = requests.get(url)
        soup = BeautifulSoup(page.text, 'html.parser')
        print(url)
        #print(soup.prettify())
        for site in soup.find_all("div", {"class": "site-info"}):
            site_info = []
            for item in site.find_all("li"):
                text = item.get_text().lower()

                if (text.startswith('location')):
                    match = lat_lon_pattern.findall(text)
                    site_info.append(float('-' + match[0])) # south is negative
                    site_info.append(float(match[1]))

                if (text.startswith('link')):
                    link = str(item)
                    id_start = link.find('/products/') + 10
                    id_end = link.find('.loop.')
                    radar_id = link[id_start: id_end]
                    site_info.append(radar_id)

            print(site_info)
            radar_site_data.append(site_info)


    # area code data
    area_code_data = []
    db = urllib.request.urlretrieve(area_data[0], 'database.dbf')
    #db = requests.get(area_data[0])
    table = dbf.Table(db[0])  # table is closed
    table.open()
    for item in table:
        array = [item['aac'].strip(), item['pt_name'].strip(), item['state_code'].strip(), item['lat'], item['lon']]
        area_code_data.append(array)
        #print(array)


    # forecast area data
    # add forecast url to the area code data
    forecast_area_data = []
    for url in forecast_data:
        new_url = 'ftp://ftp.bom.gov.au/anon/gen/fwo/{}.xml'.format(url)
        source = urllib.request.urlretrieve(new_url, 'xml.xml')
        page = open(source[0],'r')
        soup = BeautifulSoup(page.read(), 'xml')
        print(url)
        #print(soup.prettify())
        for area in soup.find_all('area'):
            if ('aac' in area.attrs and 'type' in area.attrs):
                if (area.attrs['type'] == 'metropolitan' or area.attrs['type'] == 'location'):
                    # convert aac -> lat_lon
                    aac = area.attrs['aac']
                    for item in area_code_data:
                        item_aac = item[0]
                        if (item_aac == aac):
                            item.append(url)
                            forecast_area_data.append(item)
                            print(item)

    print('areas found: {}'.format(len(area_code_data)))
    print('forecast found: {}'.format(len(forecast_area_data)))

    # observation station data
    #detailed_pattern = re.compile('(?<=places\/)(\w*)(?=\/)(?:\/KHEF\/\?location=|\/)(\w*)')
    detailed_pattern = re.compile('(?<=places\/)(\w*)(?=\/)(?:\/)(\w*)')
    station_data = []
    for url in observation_data:
        page = requests.get(url)
        soup = BeautifulSoup(page.text, 'html.parser')
        print(url)
        #print(soup.prettify())

        # get all links on page
        for link in soup.find_all('a'):
            a = link.get('href')[1:][:-6].split('/') # trim .shtml and first /
            if (a[0] == 'products' and len(a) == 3):
                b = str(a[2]).split('.')
                id_state = b[0]
                id_wmo = b[1]
                # for each link, get state and name of station
                json = requests.get('http://www.bom.gov.au/fwo/{}/{}.{}.json'.format(id_state, id_state, id_wmo)).json()
                header = json['observations']['header']
                if (len(header) > 0):
                    header = header[0]
                else:
                    log('no head {}'.format(id_wmo))
                    continue

                state = header['state_time_zone']
                name = header['name']
                data = json['observations']['data']

                if (len(data) > 0): # may have no data
                    data = data[0]
                else:
                    log('no data {}'.format(name))
                    continue
                
                lat = data['lat']
                lon = data['lon']

                # find the closest forecast station
                smallest_distance = 999999
                closest_forecast_station = []
                for item in forecast_area_data:
                    d = get_distance(lat, lon, item[3], item[4])
                    if (d < smallest_distance):
                        smallest_distance = d
                        closest_forecast_station = item

                forecast_url = closest_forecast_station[5]
                aac = closest_forecast_station[0]

                # find the closest radar station
                smallest_distance = 999999
                closest_radar_station = []
                for item in radar_site_data:
                    d = get_distance(lat, lon, item[0], item[1])
                    if (d < smallest_distance):
                        smallest_distance = d
                        closest_radar_station = item

                radar_station = closest_radar_station[2]

                # detailed forecast info
                # http://www.bom.gov.au/places/[state]/[location]/forecast/detailed/
                # http://www.bom.gov.au/places/sa/KHEF/?location=Yatala
                search_url = 'http://www.bom.gov.au/places/search/?q={}%2C{}'.format(lat, lon)
                r = str(requests.get(search_url).url)
                match = re.findall(detailed_pattern, r)
                '''
                if ('location=' in r):
                    
                else:
                    match = re.findall(detailed_pattern_2, r)
                '''
                detailed = '{}/{}'.format(match[0][0], match[0][1])

                array = [id_wmo, id_state, forecast_url, aac, radar_station, lat, lon, state, name, detailed]
                station_data.append(array)
                print(array)


    # write json
    with open('database.json', 'w', encoding='utf-8') as f:
        f.writelines('{\n')
        f.writelines('  "stations" : [\n')
        for item in station_data:
            f.writelines('      ["{}", "{}", "{}", "{}", "{}", {}, {}, "{}", "{}", "{}"],\n'.format(item[0], item[1], item[2], item[3], item[4], item[5], item[6], item[7], item[8], item[9]))
        f.writelines('  ]\n')
        f.writelines('}\n')
        # to big to use json dump
        #json.dump(station_data, f, ensure_ascii=False, indent=4)

    return


def run_desktop():
    try:
        run('''
        cd build/src
        ./qweather
        ''')
    except KeyboardInterrupt:
        log('exit')
    
    return


def run_mobile():
    try:
        run('''
        cd build/src
        QT_QUICK_CONTROLS_MOBILE=true QT_QUICK_CONTROLS_STYLE=Plasma ./qweather
        ''')
    except KeyboardInterrupt:
        log('exit')
    
    return


def debug():
    run('''
    cd build/src
    gdb qweather
    ''')
    return


def build_debug():
    run('''
    mkdir -p build
    cd build
    cmake .. -DDEFINE_DEBUG=ON -DCMAKE_BUILD_TYPE=Debug
    make
    cd ..
    ''')
    return
    

def build_release():
    run('''
    mkdir -p build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release
    make
    sudo make install
    ''')
    return


def requirements():
    run('''
    yay -S --noconfirm --needed cmake extra-cmake-modules gdb
    ''')
    return


def log(str=''):
    print(str)
    if not config['logToFile']:
        return

    with open("log.txt", "a") as f:
        f.write(str + '\n')
    return
    

# run commands
# params:
# cwd
# show cmd
def run(command, params = {}):
    # clean command
    cmd = inspect.cleandoc(command)
    
    # show output
    show_cmd = False
    if 'show_cmd' in params:
        show_cmd = params['show_cmd']

    if show_cmd:
        print(cmd + '\n')
        
    working_dir = os.getcwd()
    if 'cwd' in params:
        working_dir = params['cwd']
        
    # exec
    subprocess.run(cmd, shell=True, cwd=working_dir)
    return


def run_sudo(command, params = {}):
    cmd = inspect.cleandoc(command)
    password = getpass.getpass('[sudo] password: ')
    cmd = 'echo {}|sudo -S {}'.format(password, cmd)
    run(cmd, params)
    return


if __name__ == '__main__':
    os.system('cls||clear')
    main()
