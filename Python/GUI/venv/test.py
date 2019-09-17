def read_records_from_file(filename):
    '''read the file'''
    data = []
    started = False
    for line in open(filename):
        line = line.strip()
        if line == '<end step data>':
            break
        if started:
            data.append(tuple(line.split(',')))
        if line == '<begin step data>':
            started = True
    data = [(x[0], int(x[1])) for x in data]
    return data