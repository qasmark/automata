import csv

input_file = 'input_moore.csv'
output_file = 'output.csv'
status = 'moore2mealy'
# 'mealy2moore'
# 'moore2mealy'


def get_list_states(s):
    lst = []
    ch = ''

    for i in s:
        if i != ' ':
            ch += i
        else:
            lst.append(ch)
            ch = ''

    lst.append(ch)
    return lst


def find_unique_states(lst):
    st = set()
    for i in range(1, len(lst)):
        for j in range(1, len(lst[i])):
            if lst[i][j] not in st:
                st.add(lst[i][j])
    return list(st)


def get_output_signals(lst):
    signals_lst = []
    for i in range(len(lst)):
        signals_lst.append(lst[i][-2:])
    return signals_lst


def get_output_states(signals, dct):
    lst = []
    row_lst = []

    for i in dct.keys():
        temp_state = dct[i][:-3]
        find_pos = lines[0].index(temp_state) + 1
        for j in range(len(signals)):
            row_lst.append(lines[1 + j][find_pos])
        lst.append(row_lst)
        row_lst = []

    return lst


def get_truly_transfers(lst, dct):
    for i in range(len(lst)):
        for j in range(len(lst[i])):
            lst[i][j] = dct[lst[i][j]]


def reverse_transfers(lst, signals):
    new_lst = []
    for i in range(len(signals)):
        new_lst.append([f'x{i + 1}'])

    for i in range(len(lst)):
        for j in range(len(lst[i])):
            new_lst[j].append(lst[i][j])
    return new_lst


def get_output_signals_with_states(rows):
    dct = dict(zip(rows[1], rows[0]))

    rows[0].insert(0, ' ')
    rows[1].insert(0, ' ')

    for i in range(1, len(rows[0])):
        output_signal = rows[0][i]
        for j in range(2, len(rows)):
            if not rows[j][i].startswith('x'):
                rows[j][i] += '/' + output_signal
                rows[j][i].replace('q', 's')


    print(rows)
    return rows


if status == 'mealy2moore':
    with open(input_file, 'r') as csvfile:
        reader = csv.reader(csvfile)

        lines = []

        for row in reader:
            temp = str(''.join(row)).replace(';', ' ').strip()
            lines.append(get_list_states(temp))

        uniq_states = find_unique_states(lines)
        uniq_states = sorted(uniq_states)

        new_uniq_states = [f'q{i}' for i in range(len(uniq_states))]
        output_signals = get_output_signals(uniq_states)

        dict_of_states = dict(zip(new_uniq_states, uniq_states))
        dict_of_transfer = dict(zip(uniq_states, new_uniq_states))

        input_signals = [f'x{i}' for i in range(1, len(lines))]
        result = []

        transition_table = get_output_states(input_signals, dict_of_states)
        get_truly_transfers(transition_table, dict_of_transfer)

        new_output_signals = [' '] + output_signals
        result_new_uniq_states = [' '] + new_uniq_states

        result.append(new_output_signals)
        result.append(result_new_uniq_states)

        reverse_transition_table = reverse_transfers(transition_table, input_signals)
        result += reverse_transition_table

        with open(output_file, 'w', newline='') as f:
            writer = csv.writer(f, delimiter=';')
            for row in result:
                writer.writerow(row)

elif status == 'moore2mealy':
    with open(input_file, 'r') as csvfile:
        reader = csv.reader(csvfile)

        lines = []

        for row in reader:
            temp = str(''.join(row)).replace(';', ' ').strip()
            lines.append(get_list_states(temp))
    print(lines)

    result = get_output_signals_with_states(lines)

    with open(output_file, 'w', newline='') as f:
        writer = csv.writer(f, delimiter=';')
        for row in result:
            writer.writerow(row)
else:
    print('Unknown transfer')
    exit()
