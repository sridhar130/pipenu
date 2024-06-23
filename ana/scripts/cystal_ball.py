import numpy as np
import matplotlib.pyplot as plt
from scipy.special import erf

plt.close("all")

def crystal_ball(x, alpha, n, xbar, sigma):
    n_over_alpha = n/abs(alpha)
    exp = np.exp(-0.5*alpha ** 2)
    A = (n_over_alpha)**n*exp
    B =  n_over_alpha - abs(alpha)
    C = n_over_alpha/(n-1)*exp
    D = np.sqrt(0.5*np.pi)*(1 + erf(abs(alpha)/np.sqrt(2)))
    N = 1/(sigma*(C + D))

    mask = (x - xbar)/sigma > -alpha

    gaussian = N*np.exp(-0.5*((x[mask]-xbar)/sigma)**2)
    powerlaw = N*A*(B - (x[~mask]-xbar)/sigma)**-n

    result = np.zeros_like(x)
    result[mask] = gaussian
    result[~mask] = powerlaw
    # or
    # result = np.concatenate((powerlaw, gaussian))

    return result, mask

x = np.linspace(-10, 4, 1000)
alpha = 1
n = 10
xbar = 0
sigma = 1

y, mask = crystal_ball(x, alpha, n, xbar, sigma)

plt.plot(x[mask], y[mask], label="gauss")
plt.plot(x[~mask], y[~mask], label="exp")
plt.legend()
