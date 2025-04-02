
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import numpy as np

# Data input
data = {
    "2KB": [0.664576530456543, 0.6637747287750244, 0.6646454334259033, 0.6652812957763672, 0.6619727611541748],
    #"3KB": [0.6598618030548096, 0.6684329509735107, 0.6672298908233643, 0.6665196418762207, 0.6680891513824463],
    "4KB": [0.6678228378295898, 0.6697590351104736, 0.6697163581848145, 0.6705381870269775, 0.6706717014312744],
    "6KB": [0.6740798950195312, 0.6743040084838867, 0.6747331619262695, 0.6753687858581543, 0.6760921478271484],
    "8KB": [0.6784524917602539, 0.6799595355987549, 0.6787745952606201, 0.680999755859375, 0.6784207820892334],
    "10KB": [0.6846325397491455, 0.6841464042663574, 0.6832573413848877, 0.6846363544464111, 0.6859853267669678]
}

# Process data
memory_sizes = []
average_runtimes = []
for memory, runtimes in data.items():
    memory_sizes.append(int(memory.replace("KB", "")))  # Extract numeric memory size
    average_runtimes.append(np.mean(runtimes))  # Compute average runtime

# Plot the data
plt.figure(figsize=(8, 4))
plt.plot(memory_sizes, average_runtimes, color='black', marker='o', linestyle='--', linewidth=2)
#plt.title('State Size vs Average Runtime')
plt.xlabel('Size of Local Training Dataset (KB)')
plt.ylabel('Runtime (s)')
plt.grid(True)
plt.xticks(memory_sizes)
#plt.tight_layout()

# Show the graph
#plt.show()
plt.savefig('./runtime_vs_statesize.png', bbox_inches='tight')
