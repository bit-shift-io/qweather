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

    # http://www.bom.gov.au/catalogue/data-feeds.shtml#forecasts
    forecast_data = [
        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDN11060.xml',
        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDN11100.xml',
        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDN11050.xml',

        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDD10207.xml',
        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDD10208.xml',
        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDD10198.xml',
        
        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDQ11295.xml',
        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDQ11296.xml',
        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDQ10605.xml',

        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDS10044.xml',
        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDS11039.xml',
        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDS10037.xml',

        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDT16710.xml',
        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDT13515.xml',
        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDT13630.xml',

        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDV10753.xml',
        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDV10752.xml',
        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDV10751.xml',

        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDW14199.xml',
        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDW14100.xml',
        'ftp://ftp.bom.gov.au/anon/gen/fwo/IDW12400.xml',
    ]

    result = {
        'areas': [],
        'forecasts' : [],
    }
    

    # print forecast area data
    for url in forecast_data:
        source = urllib.request.urlretrieve(url, 'xml.xml')
        page = open(source[0],'r')
        soup = BeautifulSoup(page.read(), 'xml')
        print(url)
        #print(soup.prettify())
        for area in soup.find_all('area'):
            if ('aac' in area.attrs and 'type' in area.attrs):
                if (area.attrs['type'] == 'metropolitan' or area.attrs['type'] == 'location'):
                    array = [area.attrs['aac'], area.attrs['description'], url]
                    print(array)
                    result['forecasts'].append(array)

    # print area code data
    db = urllib.request.urlretrieve(area_data[0], 'database.dbf')
    #db = requests.get(area_data[0])
    table = dbf.Table(db[0])  # table is closed
    table.open()
    for item in table:
        array = [item['aac'].strip(), item['pt_name'].strip(), item['state_code'].strip(), item['lat'], item['lon']]
        result['areas'].append(array)
        print(array)

    # write json
    with open('areas.json', 'w', encoding='utf-8') as f:
        json.dump(result, f, ensure_ascii=False, indent=4)
 

    result = {
        'stations' : []
    }

    # print observation station data
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
                array = [id_wmo, id_state, state, name, lat, lon]
                result['stations'].append(array)
                print(array)

    # write json
    # TODO: some crash here due to size being to large??!
    with open('stations.json', 'w', encoding='utf-8') as f:
        json.dump(result, f, ensure_ascii=False, indent=4)

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
    cmake .. -DCMAKE_BUILD_TYPE=Debug
    make
    cd ..
    ''')
    return
    

def build_release():
    run('''
    mkdir -p build
    cd build
    cmake .. -DKDE_INSTALL_USE_QT_SYS_PATHS=ON -DCMAKE_BUILD_TYPE=Release
    make
    cd ..
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
