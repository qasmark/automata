import csv
import networkx as nx
import matplotlib.pyplot as plt
import subprocess

# Чтение данных из CSV-файла
with open('output.csv', 'r') as file:
    reader = csv.reader(file)
    data = list(reader)

# Создание графа
G = nx.DiGraph()

# Объединение выходных сигналов и состояний в одно
merged_data = []
for row in data[1:]:
    signals = [signal for signal in row[1:] if signal != '']
    state = row[0]
    merged_data.append((state, '|'.join(signals)))

# Добавление узлов и ребер
for i, row in enumerate(merged_data):
    state, signals = row
    G.add_node(state)
    G.add_edge('x1', state, label=f'x1/{signals}')

# Подписывание названий ребер x1
edge_labels = nx.get_edge_attributes(G, 'label')

# Определение позиций узлов на графе
pos = nx.spring_layout(G)

# Рисование графа с помощью matplotlib
nx.draw_networkx(G, pos, with_labels=True, node_size=1000, node_color='lightblue', font_size=10)
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)

# Отображение графика
plt.axis('off')
plt.show()