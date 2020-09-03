
# Monte Carlo Simulation using Merlin

This tutorial will walk through how to setup a HELICS Monte Carlo simulation
using Merlin. We assume that you have already completed the
orchestration tutorial [here](orchestration.md) and have some
familiarity on how Merlin works.

We will walk through how to user Merlin to setup and run a HELICS
Monte Carlo simulation. Code for the Monte Carlo simulation and the
full Merlin spec and be found [here](<link to the code on github>)

# EV Charging Monte Carlo Co-simulation
<Blurb about Monte Carlo sims>

# Merlin spec for Co-Simulation
In this specification we will be using the
[helics_cli](https://github.com/GMLC-TDC/helics-cli) to execute each
Co-Simulation run since this is a Monte Carlo simulation. This means
that helics_cli will be executed multiple times with different
helics_cli runner files. 

## Helics_cli in Merlin

Since we are using the helics_cli to manage and execute all the
federates, we need to create these runner files for helics_cli.
There is a provided python script called `make_samples.py` that will
generate the runner file and a csv file that will be used in the
study step.

An example of how the helics_cli runner file looks like is shown
below.

```
Example of helics_cli runner for UQ EV example
```

As you can see from the example there are 3 federates 1 for the
EVMsgFed.py, 1 for the EVControllerMsgFed.py and 1 for the HELICS
broker. Helics_cli will start each of these federates. In the Merlin
spec, Merlin will be instructed to execute the helics_cli with all the
generated helics_cli runner files.

## Merlin Specification

In the Merlin spec we will instruct Merlin to execute N number of the
Monte Carlo co-simulations. The number of samples is the number
specified as the `N_SAMPLES` env variable in the env section of the
merlin spec.

```
env:
  variables:
    OUTPUT_PATH: ./UQ_EV_Study
    N_SAMPLES: 10	
```

We set the ouput directory to UQ_EV_Study, this is where all the
output files will be stored.



