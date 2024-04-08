import matplotlib.pyplot as plt

params = {'text.usetex': True, 
          'text.latex.preamble': [r'\usepackage{cmbright}', r'\usepackage{amsmath}']}
plt.rcParams.update(params)

# Data
runtime_acron = {
    "HMAC": 1480,
    "ED25519": 1524,
    "ECC": 1616,
    "Sphincs": 3612
}

runtime_baseline = {
    "HMAC": 1280,
    "ED25519": 1484,
    "ECC": 1584,
    "Sphincs": 3580
}

# Extracting labels and values
labels = list(runtime_acron.keys())
acron_values = list(runtime_acron.values())
baseline_values = list(runtime_baseline.values())

# Plotting
x = range(len(labels))
width = 0.3

labels = [r'\ensuremath{\sf{Baseline_{SK}}}', r'\ensuremath{\sf{SLAPP_{SK}}}', r'\ensuremath{\sf{Baseline_{PK}}}', r'\ensuremath{\sf{SLAPP_{PK}}}', r'\ensuremath{\sf{Baseline_{PQ}}}', r'\ensuremath{\sf{SLAPP_{PQ}}}']

fig, ax = plt.subplots(figsize=(8, 3))
#rects1 = ax.bar(x, baseline_values, width, label='Baseline')
x = [0]
rects1 = ax.bar([0], runtime_baseline["HMAC"], width, label=labels[0], edgecolor='lightcoral', hatch='x', color='white')
rects2 = ax.bar([i + width for i in x], runtime_acron["HMAC"], width, label=labels[1], color='lightcoral')

x = [1]
rects3 = ax.bar(x, runtime_baseline["ECC"], width, label=labels[2], color='white', edgecolor='skyblue', hatch='x')
rects4 = ax.bar([i + width for i in x], runtime_acron["ECC"], width, label=labels[3], color='skyblue')

x = [2]
rects5 = ax.bar(x, runtime_baseline["Sphincs"], width, label=labels[4], color='white', edgecolor='springgreen', hatch='x')
rects6 = ax.bar([i + width for i in x], runtime_acron["Sphincs"], width, label=labels[5], color='springgreen')

# Adding labels, title, and legend
ax.set_ylabel('Size (in bytes)')
#ax.set_title('Stack Utilization')
#ax.set_xticks([i + width / 2 for i in x])
x = [0,0.3,1,1.3,2,2.3]
ax.set_xticks([i for i in x], labels)
#ax.set_xticklabels(labels)
#ax.legend()
#ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.1), fancybox=True, shadow=True, ncol=2)



# Adding values on top of the bars
def autolabel(rects):
    for rect in rects:
        height = rect.get_height()
        ax.annotate('{}'.format(height),
                    xy=(rect.get_x() + rect.get_width() / 2, height),
                    xytext=(0, 3),  
                    textcoords="offset points",
                    ha='center', va='bottom')

autolabel(rects1)
autolabel(rects2)
autolabel(rects3)
autolabel(rects4)
autolabel(rects5)
autolabel(rects6)

ax.set_ylim(0, 4000) 

#plt.show()
plt.savefig('stack.png', bbox_inches='tight')

