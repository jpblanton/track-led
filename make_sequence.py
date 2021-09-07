def make_sequence(list_of_leds, duration):
    time_per_led = duration / len(list_of_leds)
    return list(zip(list_of_leds, itertools.cycle(time_per_led)))
