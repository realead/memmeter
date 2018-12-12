import os
import sys

import pandas as pd
import matplotlib.pyplot as plt


input_file=sys.argv[1]
output_file=input_file[0:-3]+'png'


data=pd.read_csv(input_file)

print(data.head())

labels=list(data)

plt.semilogx(data[labels[0]], data[labels[1]], label="my laptop")
plt.xlabel(labels[0])
plt.ylabel(labels[1])

plt.savefig(output_file)
plt.close()


