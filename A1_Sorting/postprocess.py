#!/usr/bin/env python3
import json
import os
import shutil
import matplotlib.pyplot as plt
from tabulate import tabulate

filename = 'out.txt'
with open(filename, 'r') as f:
    content = f.read()

parts = content.split('\n\n', 1)
if len(parts) < 2:
    raise ValueError("File does not contain both JSON and CSV data separated by a blank line.")

json_text, csv_text = parts[0], parts[1]

try:
    correctness_data = json.loads(json_text)
except json.JSONDecodeError as e:
    raise ValueError("Error parsing JSON: " + str(e))

perf_data = []
csv_lines = csv_text.strip().splitlines()
for line in csv_lines:
    if not line.strip():
        continue
    parts_line = line.split(',')
    if len(parts_line) != 4:
        continue 
    alg, typ, n_str, time_str = parts_line
    try:
        n_val = int(n_str)
        time_val = float(time_str)
    except ValueError:
        continue
    perf_data.append({
        'algorithm': alg,
        'type': typ,
        'n': n_val,
        'time': time_val
    })

total_pass = 0
total_tests = 0
table_rows = []

for alg, tests in correctness_data.items():
    for data_type, result in tests.items():
        passed = result.get("pass", 0)
        failed = result.get("fail", 0)
        total_pass += passed
        total_tests += (passed + failed)
        table_rows.append([alg, data_type, passed, failed])

user_score = total_pass / total_tests if total_tests > 0 else 0

print("\n=== USER SCORE ===")
print("User Score: {:.2%}".format(user_score))

print("\n=== CORRECTNESS TEST DETAILS ===")
print(tabulate(table_rows, headers=["Algorithm", "Data Type", "Pass", "Fail"], tablefmt="grid"))

plots_dir = 'plots'
if os.path.exists(plots_dir):
    shutil.rmtree(plots_dir)
os.makedirs(plots_dir)

user_algs = list(correctness_data.keys())
data_types = ['int', 'string']

for alg in user_algs:
    for typ in data_types:
        user_dict = {d['n']: d['time'] for d in perf_data if d['algorithm'] == alg and d['type'] == typ}
        std_dict  = {d['n']: d['time'] for d in perf_data if d['algorithm'] == 'std_sort' and d['type'] == typ}

        common_ns = sorted(set(user_dict.keys()).intersection(set(std_dict.keys())))
        
        x_common = []
        y_user   = []
        y_std    = []
        for n in common_ns:
            t_user = user_dict[n]
            t_std  = std_dict[n]
            if t_user < 0 or t_std < 0:
                break
            x_common.append(n)
            y_user.append(t_user)
            y_std.append(t_std)
        
        if not x_common:
            continue

        plt.figure(figsize=(8, 5))
        plt.plot(x_common, y_user, marker='o', label=alg)
        plt.plot(x_common, y_std, marker='o', label='std_sort')
        plt.xlabel("N (Input Size)")
        plt.ylabel("Time (s)")
        plt.title(f"Performance Comparison: {alg} vs std_sort ({typ})")
        plt.legend()
        plt.grid(True)
        
        plot_filename = os.path.join(plots_dir, f"performance_{alg}_{typ}.png")
        plt.savefig(plot_filename)
        plt.close()

compare_algs = ['quick_sort', 'merge_sort', 'std_sort']
for typ in ['int', 'string']:
    perf_dicts = {}
    for alg in compare_algs:
        perf_dicts[alg] = {d['n']: d['time'] for d in perf_data if d['algorithm'] == alg and d['type'] == typ}
    
    common_ns = sorted(set.intersection(*(set(perf_dicts[alg].keys()) for alg in compare_algs)))
    
    x_common = []
    y_quick = []
    y_merge = []
    y_std   = []
    for n in common_ns:
        t_quick = perf_dicts['quick_sort'][n]
        t_merge = perf_dicts['merge_sort'][n]
        t_std   = perf_dicts['std_sort'][n]
        if t_quick < 0 or t_merge < 0 or t_std < 0:
            break
        x_common.append(n)
        y_quick.append(t_quick)
        y_merge.append(t_merge)
        y_std.append(t_std)
    
    if not x_common:
        continue

    plt.figure(figsize=(8, 5))
    plt.plot(x_common, y_quick, marker='o', label='quick_sort')
    plt.plot(x_common, y_merge, marker='o', label='merge_sort')
    plt.plot(x_common, y_std, marker='o', label='std_sort')
    plt.xlabel("N (Input Size)")
    plt.ylabel("Time (s)")
    plt.title(f"Performance Comparison: quick_sort, merge_sort and std_sort ({typ})")
    plt.legend()
    plt.grid(True)
    
    plot_filename = os.path.join(plots_dir, f"performance_compare_{typ}.png")
    plt.savefig(plot_filename)
    plt.close()

print(f"\nAll plots have been saved in the '{plots_dir}' directory.")
