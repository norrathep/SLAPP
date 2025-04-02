
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import numpy as np

# Train time
slapp = {
    "2KB": [23.77454924583435, 23.776171684265137],
    "4KB": [23.801414012908936, 23.802979230880737],
    "6KB": [23.801609754562378, 23.80368685722351],
    "8KB": [23.82357096672058, 23.850913047790527],
    "10KB": [23.837395191192627, 23.87251043319702]
}

# Basically SLAPP - Hash(PMEM) - CheckState - SetState
existing_techniques = {
    "2KB": [23.1132173538208, 23.11469531059265],
    "4KB": [23.11936593055725, 23.109076976776123],
    "6KB": [23.11307978630066, 23.108007192611694],
    "8KB": [23.12988042831421, 23.13869857788086],
    "10KB": [23.1281840801239, 23.133814096450806]
}

# Function to calculate average and overhead percentage
def compute_averages_and_overheads(slapp, existing_techniques):
    memory_sizes = []
    overheads = []
    for key in slapp.keys():
        slapp_avg = sum(slapp[key]) / len(slapp[key])
        existing_avg = sum(existing_techniques[key]) / len(existing_techniques[key])
        overhead_percentage = ((slapp_avg - existing_avg) / existing_avg) * 100
        memory_sizes.append(key)
        overheads.append(overhead_percentage)
    return memory_sizes, overheads

# Compute averages and overheads
memory_sizes, overheads = compute_averages_and_overheads(slapp, existing_techniques)

# Plot the overhead percentages
plt.figure(figsize=(8, 3))
plt.bar(memory_sizes, overheads, width=0.5, color='black', alpha=0.7)
#plt.title("Overhead Percentage for SLAPP vs Existing Techniques", fontsize=14)
plt.xlabel("Size of local training dataset (in KB)", fontsize=12)
plt.ylabel("Runtime Overhead (%)", fontsize=12)
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.tight_layout()
#plt.show()
plt.savefig('./runtime_vs_statesize_vs_existing.png', bbox_inches='tight')
