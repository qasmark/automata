import csv
import subprocess
import matplotlib.pyplot as plt
import networkx as nx

input_file = 'input_mealy_2.csv'
output_file = 'output.csv'
output_graph_file = 'output.csv'
input_graph_file = 'input_moore_2.csv'
status = 'moore'


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


draw_automata(status)