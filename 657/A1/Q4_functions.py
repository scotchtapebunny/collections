import numpy as np

def sigmoid(A):
    #suppress warnings needed because numpy uses float(64) by default and this function can return results larger than that
    return 1 / (1 + np.exp(-A))

# softmax activation function - used for output layer
def softmax(A):
    expA = np.exp(A - np.max (A))
    return expA / expA.sum(axis=1, keepdims=True)

def forward(X_training_data, hidden_layer_weights, output_layer_weights, hidden_layer_bias, output_layer_bias):
    hidden_layer_Sum = np.dot(X_training_data, hidden_layer_weights) + hidden_layer_bias
    hidden_layer_Sig = sigmoid(hidden_layer_Sum)
    output_layer_Sum = np.dot(hidden_layer_Sig, output_layer_weights) + output_layer_bias
    final_output = softmax(output_layer_Sum)

    #we are interested in getting the final output and output of hidden layer from forward propagation to use it for backpropagation
    return final_output