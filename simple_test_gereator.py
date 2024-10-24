import random

def random_data(size, value_range, cache_size, filename):
    data = [random.randint(0, value_range) for _ in range(size)]
    
    with open(filename, 'w') as file:
        file.write(f"{cache_size} {size} " + " ".join(map(str, data)) + "\n")

random_data(1000000, 50000000, 1000000, 'random_data.txt')
