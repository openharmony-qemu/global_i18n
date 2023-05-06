import re
import pickle
import png
import numpy as np
import json

def read_data(filename):
    data_res = {}
    loc_pattern = re.compile("\(.*\)")
    tz_pattern = re.compile("\[.*\]")
    raw_lines = []
    with open(filename, "r") as f:
        lines = f.readlines()
        for line in lines:
            raw_lines.append(line)
    for line in raw_lines:
        line = line.strip()
        loc = re.search(loc_pattern, line).group()
        tz = re.search(tz_pattern, line).group()
        location = eval(loc)
        timezones = tz[1:-1].split(",")
        data_res[location] = timezones
    
    with open("loc2tz.pickle", "wb") as f:
        pickle.dump(data_res, f)
    
    return data_res


def load_data(pickle_name):
    with open(pickle_name, "rb") as f:
        data = pickle.load(f)
    return data



def classify_timezones(data):
    group_wn = set()
    group_en = set()
    group_ws = set()
    group_es = set()
    for loc, tzs in data.items():
        if loc[0] < 0 and loc[1] >= 0:
            for tz in tzs:
                group_wn.add(tz)
        elif loc[0] >= 0 and loc[1] >= 0:
            for tz in tzs:
                group_en.add(tz)
        elif loc[0] < 0 and loc[1] < 0:
            for tz in tzs:
                group_ws.add(tz)
        else:
            for tz in tzs:
                group_es.add(tz)
    print(len(group_wn))
    print(len(group_en))
    print(len(group_ws))
    print(len(group_es))
    return [group_wn, group_en, group_ws, group_es]


def numerized_dictionary(classes):
    numerized_classes = []
    for group in classes:
        temp_group = {}
        sgroup = list(group)
        sgroup.sort()
        for num, name in enumerate(sgroup):
            temp_group[name] = num
        numerized_classes.append(temp_group)
    for i, c in enumerate(numerized_classes):
        print(i)
        print(c)
    return numerized_classes


def export_numerized_dicts(numerized_classes):
    with open("name2num.txt", "w", encoding="utf-8") as f:
        f.write(str(numerized_classes))
    num2name_dict_list = []
    for d in numerized_classes:
        num2name = dict(zip(d.values(), d.keys()))
        num2name_dict_list.append(num2name)
    with open("num2name.txt", "w", encoding="utf-8") as f:
        f.write(str(num2name_dict_list))
    for d in num2name_dict_list:
        print("#################################################")
        for num, name in d.items():
            print(num, name)


def generate_array_from_data(data, numerized_classes):
    total_nums_list = []
    # array_one_point = [255, 255, 255]
    # array_one_row = array_one_point * 3600
    # array_map = [array_one_row] * 1800
    for key, val in data.items():
        original_keys = key
        fixed_keys = (int(original_keys[0]*10+1800), int(original_keys[1]*10+900))
        if original_keys[0] < 0 and original_keys[1] >= 0:
            quater = 0
        elif original_keys[0] >= 0 and original_keys[1] >= 0:
            quater = 1
        elif original_keys[0] < 0 and original_keys[1] < 0:
            quater = 2
        else:
            quater = 3
        names_l = []
        for v in val:
            names_l.append(numerized_classes[quater][v])
        nums_l = [255, 255, 255]
        for i in range(len(names_l)):
            nums_l[i] = names_l[i]
        total_nums_list += nums_l
    # print(total_nums_list)
    print(len(total_nums_list))
    return total_nums_list


def group_in_total(total_list):
    i = 0
    map_array = []
    col_array = []
    for item in total_list:
        col_array.append(item)
        i += 1
        if i == 1800*3:
            map_array.append(col_array)
            col_array = []
            i = 0
    print(len(map_array))
    return map_array



def pack2png(map_array):
    png.from_array(map_array, "RGB").save("loc2tz.png")


def save_png_test():
    array_one_point = [255, 0, 0]
    array_one_row = [array_one_point] * 3600
    array_map = [array_one_row] * 1800
    # print(len(array_map[0]))
    # print(array_map)
    png.from_array(array_map, "RGB").save("03.png")

    # w = png.Writer(3600, 1800, greyscale=True)
    # f = open("png.png", "wb")
    # w.write(f, [range(256), range(256)])
    # f.close()




if __name__ == '__main__':
    # data = read_data("location2tz.txt")


    data = load_data("loc2tz.pickle")
    name_classes = classify_timezones(data)
    numerized_classes = numerized_dictionary(name_classes)
    export_numerized_dicts(numerized_classes)

    # save_png_test()
    total_list = generate_array_from_data(data, numerized_classes)
    map_array = group_in_total(total_list)
    pack2png(map_array)