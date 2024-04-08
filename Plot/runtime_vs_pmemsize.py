import matplotlib.pyplot as plt

params = {'text.usetex': True, 
          'text.latex.preamble': [r'\usepackage{cmbright}', r'\usepackage{amsmath}']}
plt.rcParams.update(params)

# Data
x = [4, 8, 12, 16, 20, 24, 28, 32]  # x values in KB
#x = [i*1024 for i in x]
BaselineHMAC = [0.0790095329284668, 0.12446427345275879, 0.17047786712646484, 0.21570920944213867, 0.2619667053222656, 0.30765604972839355, 0.3547205924987793, 0.3993499279022217]
aronHMAC = [0.08375906944274902, 0.129044771194458, 0.1743919849395752, 0.22248125076293945, 0.26660728454589844, 0.3120884895324707, 0.35889720916748047, 0.4047119617462158]
BaselineED2519 = [14.5045485496521, 14.547479629516602, 14.592308759689331, 14.636958837509155, 14.68211317062378, 14.725340604782104, 14.77086091041565, 14.814823389053345]
acronEd25519 = [14.49381399154663, 14.541197299957275, 14.586003541946411, 14.63441801071167, 14.681947469711304, 14.724242925643921, 14.77163815498352, 14.816883087158203]
acronECC = [2.2518115043640137, 2.2988271713256836, 2.343857526779175, 2.389514923095703, 2.435807228088379, 2.484464645385742, 2.527547597885132, 2.5684518814086914]
BaselineECC = [2.2474045753479004, 2.28305029869079, 2.33861470222473, 2.3731114864349365, 2.4212682247161865, 2.4771275520324707, 2.52275013923645, 2.5620603561401367]
BaselineSphincs = [103.38624906539917, 103.37984991073608, 103.84800958633423, 103.65508389472961, 103.63118529319763, 103.55302786827087, 103.58099865913391, 103.62103605270386]
acronSphincs = [103.60318803787231, 103.6840922832489, 103.44010138511658, 103.64500617980957, 103.55301690101624, 103.56300139427185, 103.77327919006348, 103.92214632034302]

# Plotting
plt.figure(figsize=(8, 4))

labels = [r'\ensuremath{\sf{Baseline_{SK}}}', r'\ensuremath{\sf{SLAPP_{SK}}}', r'\ensuremath{\sf{Baseline_{PK}}}', r'\ensuremath{\sf{SLAPP_{PK}}}', r'\ensuremath{\sf{Baseline_{PQ}}}', r'\ensuremath{\sf{SLAPP_{PQ}}}']

lsBaseline = '-.'
lsAcron = ':'
plt.plot(x, acronSphincs, linestyle=lsAcron, marker='o', label=labels[5])
plt.plot(x, BaselineSphincs, linestyle=lsBaseline, marker='x', label=labels[4])
plt.plot(x, acronECC, linestyle=lsAcron, marker='o', label=labels[3])
plt.plot(x, BaselineECC, linestyle=lsBaseline, marker='x', label=labels[2])
plt.plot(x, aronHMAC, linestyle=lsAcron, marker='o', label=labels[1])
plt.plot(x, BaselineHMAC, linestyle=lsBaseline, marker='x', label=labels[0])

# Setting log scale for y-axis
plt.yscale('log')

plt.xlabel('PMEM size (in KB)')
plt.ylabel('Time (seconds)')
#plt.title('Time vs. % x')
plt.grid(True)
plt.legend()
#plt.show()
plt.savefig('runtime_vs_pmemsize.png', bbox_inches='tight')
