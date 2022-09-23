import numpy as np


def test_mlp(data_file):
    # Load the test set

    # START
    from numpy import genfromtxt
    X_test = genfromtxt(data_file, delimiter=',', filling_values=0)
    # END


    # Load your network
    # START
    weight_hidden = np.load('Trained_weight_hidden.npy')
    hidden_bias = np.load('Trained_bias_hidden.npy')
    weight_output = np.load('Trained_weight_output.npy')
    output_bias = np.load('Trained_bias_output.npy')

    import Q4_functions
    y_pred = Q4_functions.forward(X_test, weight_hidden, weight_output,hidden_bias, output_bias)
    # END
    # Predict test set - one-hot encoded
    y_pred = np.where(y_pred < 0.5, 0, y_pred)
    y_pred = np.where(y_pred >= 0.5, 1, y_pred)

    return y_pred


'''
How we will test your code:

from test_mlp import test_mlp, STUDENT_NAME, STUDENT_ID
from acc_calc import accuracy 

y_pred = test_mlp('./test_data.csv')

test_labels = ...

test_accuracy = accuracy(test_labels, y_pred)*100
'''