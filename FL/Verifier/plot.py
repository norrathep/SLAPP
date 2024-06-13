import matplotlib.pyplot as plt

# Data
mem_size = [0x800, 0x1000, 0x1800, 0x2000, 0x2800, 0x3000, 0x3800]
hash_data = [595641, 1148824, 1700364, 2252191, 2803799, 3355419, 3906625]
search_data = [531437, 842741, 1155334, 1470835, 1783286, 2077515, 2389749]

# Plot
plt.plot(mem_size, hash_data, label='Hash')
plt.plot(mem_size, search_data, label='Search')

# Labeling
plt.xlabel('Memory Size')
plt.ylabel('Number of Operations')
plt.title('Hash (SHA256) and Search Operations (Aho-corasick with 2 keywords) vs Memory Size')
plt.legend()

# Show plot
plt.show()
