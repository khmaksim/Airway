# import io
import sys
import tabula
import sqlite3
import pandas as pd
import math
import re
from os import path
 
# from pdfminer.converter import TextConverter
# from pdfminer.pdfinterp import PDFPageInterpreter
# from pdfminer.pdfinterp import PDFResourceManager
# from pdfminer.pdfpage import PDFPage

# def extract_text_from_pdf(pdf_path):
#     resource_manager = PDFResourceManager()
#     fake_file_handle = io.StringIO()
#     converter = TextConverter(resource_manager, fake_file_handle)
#     page_interpreter = PDFPageInterpreter(resource_manager, converter)
 
#     with open(pdf_path, 'rb') as fh:
#         for page in PDFPage.get_pages(fh, caching=True, check_extractable=True):
#             page_interpreter.process_page(page)
#             break
#         text = fake_file_handle.getvalue()
 
#     # close open handles
#     converter.close()
#     fake_file_handle.close()
 
#     if text:
#         return text

def airway_insert(connect, code_airway):
	cursor = connect.cursor()
	cursor.execute("INSERT OR IGNORE INTO airway_name VALUES(:code)", {"code": code_airway})
	id = cursor.lastrowid
	connect.commit()
	return id

def point_insert(connect, code_airway, point, order):
    cursor = connect.cursor()
    cursor.execute("INSERT OR IGNORE INTO point VALUES(:code, :name, :code_state, :lat, :lon)", 
		{"code": point['code_point'], "name": point['name_point'], "code_state": point['code_state'], "lat": point['lat'], "lon": point['lon']})
    id = cursor.lastrowid
    if id == None:
        id = code;

    cursor.execute("INSERT OR IGNORE INTO airway_point VALUES(:code_airway, :code_point, :order)", 
    {"code_airway": code_airway, "code_point": point['code_point'], "order": order})
    connect.commit()
 
def parse_airway(line):
    airway_line = re.match( r'^\uf020(.*)\/(.*)', line[0])

    if airway_line:
        airway_name_ru = airway_line.group(1)
        airway_name_en = airway_line.group(2)
        airway_distance = float(line[1].split(' ')[0])
        print(airway_name_ru, airway_name_en, airway_distance)
        return True

    return False

def parse_airway_point(line):
    point_airway_line = re.match( r'^[\uf072|\uf070]\uf020(.*)\/(.*)', line[0])

    if point_airway_line:
        point_airway_name_ru = point_airway_line.group(1)
        point_airway_name_en = point_airway_line.group(2)
        point_airway_coordinates = line[1]
        print(point_airway_name_ru, point_airway_name_en, point_airway_coordinates)
        return True

    return False

def parse_airway_point_detail(lines):
    # print(lines)
    # print(' '.join(str(x) for x in lines[0].flatten().tolist()).split(' ')[1:8])

    angle1, distance, top_border, height_min_abs, width, direction1, direction2 = ' '.join(str(x) for x in lines[0].flatten().tolist()).split(' ')[1:8]
    angle2, bottom_border = ' '.join(str(x) for x in lines[1].flatten().tolist()).split(' ')[1:3]
     # = lines[0][2]
    # bottom_border = lines[1][2]
    # height_min_abs = lines[0][3]
    # width, direction1, direction2 = lines[0][2].split(' ')
    # direction1 = lines[0][5]
    # direction2 = lines[0][6]

    print('{0}/{1}, {2}, {3}/{4}, {5}, {6}, {7}/{8}'.format(angle1[0:3], angle2[0:3], distance, top_border, bottom_border, height_min_abs, width, direction1, direction2))
    return True

if __name__ == '__main__':
    # print(extract_text_from_pdf('cbcng2008.pdf'))
    conn = sqlite3.connect('airway.db')

    if conn != None and len(sys.argv) > 1:
        file = sys.argv[1]

        if path.exists(file):
            print('Read data...')
            # df = tabula.read_pdf(file, pages="all", stream=True)
            df = tabula.read_pdf(file, pages=12, stream=True)
            order = 0

            # pages
            for p in range(len(df)):
            	# rows in DataFrame
                # print(df[p].values)

                read_columns = False
                lines = []
                i = 0
                while i < len(df[p]):
                    print(p,'-', i)
                    line = df[p].columns
                    
                    if isinstance(line[0], str) == True and not read_columns:
                        parse_airway(line)
                        read_columns = True

                    line = df[p].values[i]
                    i = i + 1

                    if isinstance(line[0], str) != True and math.isnan(float(line[0])):
                        continue

                    if parse_airway(line):
                        continue

                    # print(line) 
                    if parse_airway_point(line):
                        continue

                    point_detail_begin_line = re.match( r'^\uf020$', line[0])

                    if point_detail_begin_line:
                        lines.append(line)

                    if len(lines) == 2:
                        parse_airway_point_detail(lines)
                        lines = []


                    # isNanCodeAirway = False
                    # try:
                    #     isNanCodeAirway = math.isnan(float(df[p].loc[i][0]))
                    # except:
                    #     isNanCodeAirway = False

                    # if not isNanCodeAirway:	
                    #     code_airway = df[p].loc[i][0]
                    #     airway_insert(conn, code_airway)
                    #     order = 0

                    # isNanCodePoint = False
                    # try:
                    #     isNanCodePoint = math.isnan(float(df[p].loc[i][3]))
                    # except:
                    #     isNanCodePoint = False

                    # n = 3
                    # if isNanCodePoint:
                    #     n = 4

                    # code_state = df[p].loc[i][1]
                    # name_point = df[p].loc[i][2]    
                    # code_point = df[p].loc[i][n]
                    # lat = df[p].loc[i][n + 1]
                    # lon = df[p].loc[i][n + 2]

                    # point = {'code_state': code_state, 'name_point': name_point, 'code_point': code_point, 'lat': lat, 'lon': lon}
                    # point_insert(conn, code_airway, point, order)
                    # order = order + 1
                    # print(code_airway, code_state, name_point, code_point, lat, lon)
                # print('Insert data ot DB progress: {} %'.format(round(((p + 1) * 100) / len(df))))
                # break;
    conn.close()