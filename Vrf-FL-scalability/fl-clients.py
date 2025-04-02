import numpy as np
import hmac
import hashlib
import time
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt

# Define LSTM dimensions for clarity
INPUT_SIZE = 256  # Dimension of input features
HIDDEN_SIZE = 8  # Number of neurons in LSTM
LSTM_WEIGHT_SIZE = 4 * HIDDEN_SIZE  # LSTM has 4 gates, so weights scale by 4


def verify_hmac(weights, key, mac):
    """
    Verify HMAC-SHA256 signature for a given model weights dictionary.

    Args:
        weights (dict): Dictionary of model weights.
        key (bytes): Secret key for HMAC verification.
        mac (bytes): Received HMAC value.

    Returns:
        bool: True if verification passes, False otherwise.
    """
    serialized = str(weights).encode('utf-8')
    computed_mac = hmac.new(key, serialized, hashlib.sha256).digest()
    return hmac.compare_digest(computed_mac, mac)


def fedavg_aggregate(client_weights):
    """
    Perform FedAVG aggregation.

    Args:
        client_weights (list of dict): List of dictionaries where each dictionary contains local model weights.
        num_samples (list): List of integers, number of training samples for each client.

    Returns:
        dict: Aggregated global model weights.
    """
    # Initialize an empty dictionary for aggregated weights
    global_weights = {}


    # Iterate through each layer's weights in the model
    for layer in client_weights[0].keys():
        # Initialize the global weight for the layer
        global_weights[layer] = np.zeros_like(client_weights[0][layer])

        # Weighted sum of the layer's weights from all clients
        for i, client in enumerate(client_weights):
            global_weights[layer] += client[layer]

    return global_weights


# Function to measure runtime for different numbers of clients
def measure_runtime(num_clients_list, runs=10):
    secret_key = b"shared_secret"  # Secret key for HMAC verification
    average_runtimes = []

    for num_clients in num_clients_list:
        total_runtime_per_client = 0
        for _ in range(runs):
            # Generate client data dynamically
            client_data = []
            num_samples = []
            for i in range(num_clients):
                weights = {
                    "lstm_weight_ih": np.random.randn(LSTM_WEIGHT_SIZE, INPUT_SIZE),
                    "lstm_weight_hh": np.random.randn(LSTM_WEIGHT_SIZE, HIDDEN_SIZE),
                    "lstm_bias": np.random.randn(LSTM_WEIGHT_SIZE)
                }
                serialized = str(weights).encode('utf-8')
                mac = hmac.new(secret_key, serialized, hashlib.sha256).digest()
                client_data.append({"weights": weights, "mac": mac})
                num_samples.append(np.random.randint(10, 50))  # Random number of samples for each client

            # Verify and aggregate
            start_time = time.time()
            verified_weights = []
            for client in client_data:
                if verify_hmac(client["weights"], secret_key, client["mac"]):
                    verified_weights.append(client["weights"])
                else:
                    print(f"MAC verification failed for a client.")
                    return

            fedavg_aggregate(verified_weights)
            total_runtime_per_client += time.time() - start_time

        average_runtime = total_runtime_per_client / runs
        average_runtimes.append(average_runtime)
        print(f"Number of Clients: {num_clients}, Average Runtime: {average_runtime:.4f} seconds over {runs} runs")

    return average_runtimes

def main():
    # Define varying number of clients
    num_clients_list = [1, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500]
    runs = 10  # Number of runs to average runtime per client setup

    average_runtimes = measure_runtime(num_clients_list, runs)

    # Plotting results
    plt.figure(figsize=(8, 4))
    plt.plot(num_clients_list, average_runtimes, color='black', marker='o', linestyle='--', linewidth=2)
    plt.xlabel("Number of Clients")
    plt.ylabel("Total Runtime on Vrf (s)")
    #plt.title("Number of Clients vs Average Total Runtime")
    plt.grid(True)
    #plt.show()
    plt.savefig('./vrf_runtime.png', bbox_inches='tight')

if __name__ == "__main__":
    main()
