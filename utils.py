import struct

def hex_to_rgb(value):
    value = value.lstrip('#')
    lv = len(value)
    return tuple(int(value[i:i + lv // 3], 16) for i in range(0, lv, lv // 3))


def hex_to_brg(value):
    r, g, b = hex_to_rgb(value)
    return b, r, g


def pack_objects(values):
    value_str = f'{len(values)}i'
    return struct.pack(value_str, *values)