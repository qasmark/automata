import csv
import subprocess
import matplotlib.pyplot as plt
import networkx as nx

input_file = 'input_mealy_9.csv'
output_file = 'input_moore_9.csv'
output_graph_file = 'output_graph.csv'
input_graph_file = 'input_graph.csv'
status = 'mealy'
# 'mealy'
# 'moore'


def vis_graph(filename):
    with open(filename, 'r') as hf:
        s = hf.read()

    G = nx.DiGraph()
    lines = s.strip().split('\n')
    nodes_l = []
    for line in lines[1:-1]:
        idx, lbl = line.split(" ")
        lbl = lbl[lbl.find('"') + 1:lbl.rfind('"')]
        if '->' in idx:
            delim = idx.find("-")
            src = int(idx[:delim])
            dst = int(idx[delim + 2:])

            G.add_edge(nodes_l[src], nodes_l[dst], label=lbl)
        else:
            node_id = int(idx)
            G.add_node(lbl)
            nodes_l.append(lbl)

    # Определение позиций узлов на графе
    pos = nx.spring_layout(G)

    # Рисование графа с помощью matplotlib
    nx.draw_networkx(G, pos, with_labels=True, node_size=400, node_color='lightblue', font_size=10)

    grafo_labels = nx.get_edge_attributes(G, 'label')
    nodes = nx.get_node_attributes(G, 'label')
    nx.draw_networkx_edge_labels(G, pos, edge_labels=grafo_labels)
    # Отображение графика
    plt.axis('off')
    plt.show()
    # nx.write_graphml(G, "keker.graphml")


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

def draw_automata(status):
    if status == 'mealy':
        subprocess.call(['convert.exe', 'moore', output_file, output_graph_file])
        subprocess.call(['convert.exe', status, input_file, input_graph_file])
        vis_graph(output_graph_file)
        vis_graph(input_graph_file)
    elif status == 'moore':
        subprocess.call(['convert.exe', 'mealy', output_file, output_graph_file])
        subprocess.call(['convert.exe', status, input_file, input_graph_file])
        vis_graph(output_graph_file)
        vis_graph(input_graph_file)


if status == 'mealy':
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

        print(dict_of_states)
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
        draw_automata(status)

elif status == 'moore':
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
    draw_automata(status)


