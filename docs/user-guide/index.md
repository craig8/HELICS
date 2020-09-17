# User Guide

```eval_rst
.. toctree::
    :hidden:
    :maxdepth: 2
    
    

```

Co-simulation is a powerful analysis technique that allows simulators of different domains to interact through the course of the simulation, typically by dynamically exchanging values that define boundary conditions for other simulators. HELICS is a co-simulation platform that has been designed to allow integration of these simulators across a variety of computation platforms and languages. HELICS has been designed with power system simulation in mind ([GridLAB-D](https://github.com/gridlab-d/gridlab-d), [GridDyn](https://github.com/LLNL/GridDyn), [MATPOWER](https://github.com/GMLC-TDC/MATPOWER-wrapper), [OpenDSS](https://sourceforge.net/projects/electricdss/), [PSLF](https://github.com/GMLC-TDC/PSLF-wrapper), [InterPSS](https://github.com/InterPSS-Project/ipss-common), [FESTIV](https://www.nrel.gov/grid/festiv-model.html)) but is general enough to support a wide variety of simulators and co-simulation tasks. Support for other domains is anticipated to increase over time.

## Orientation

There are a number of classes of HELICS users:

- **New users** that have little to no experience with HELICS and co-simulation in general
	- Start with [**Installation**](../installation/index.md)
	- Read the [**Fundamental Topics**](./index.html#fundamental-topics)
	- Try the [**Examples**](./index.html#examples)	
- **Intermediate users** that have run co-simulations with HELICS using simulators in which somebody else implemented the HELICS support
- **Experienced users** that are incorporating a new simulator and need to know how to use specific features in the HELICS API
	- Look in the [**Configurations Options Reference**](./index.html#configurations-options-reference) or jump straight to the API references
- **Developers** of HELICS who are improving HELICS functionality and contributing to the code base
	- See the [**Developer Guide**](../developer-guide.md)

## Installation



## Fundamental Topics
```eval_rst
.. toctree::
    :hidden:
    :maxdepth: 2

    co-simulation_overview
    helics_key_concepts
    helics_co-sim_sequence
	

```


- [**Co-Simulation Overview**](./co-simulation_overview.md) - A more detailed discussion of what co-simulation is and how it is used
<!--- suggest renaming 'key concepts' md to 'terminology' -->
- [**HELICS Terminology**](./helics_key_concepts) - Key terms and concepts to understand before running co-simulations with HELICS
- [**HELICS Sequence of Components**](./helics_co-sim_sequence.md) - A notional walk-through of a simple transmission and distribution HELICS co-simulation to show the basic steps the software runs through
	- [**(Design) Integrating a Simulator with HELICS**](./simulator_integration.md) - How to connect an existing simulator with HELICS
	- [**Federate Configuration**](./federates.md) - Discussion of the different types of federates in HELICS and how to configure them.
		- [**Value Federates**](./value_federates.md)
		- [**Message Federates**](./message_federates.md)
		- [**Message Filters**](./filters) - How HELICS message filters can be implemented natively in HELICS or as stand-alone federates
	- [**Timing**](./timing.md) - How HELICS coordinates the simulation time of all the federates in the federation

	- [**(Launch) Execution with `helics_cli`**](./helics_cli.md) - The HELICS team has developed `helics_cli` as a standardized means of running HELICS co-simulations.
	- **Simulation**	
		- [**Connecting to a Running Simulation**](./webserver.md) - Getting live information from a running co-simulation through a webserver.
		- [**Using the web interface**](./web_interface.md) - Using the webserver, HELICS also has a built-in web interface for running, monitoring, and diagnosing co-simulations.
	- [**(Termination) Logging**](./logging.md) - Discussion of logging within HELICS and how to control it.




## Advanced Topics

```eval_rst
.. toctree::
    :hidden:
    :maxdepth: 1

    multiSourceInputs
    broker_hierarchies
    CoreTypes
    queries
    multibroker
    orchestration
    simultaneous_cosimulations


```
- [**Multi-Source Inputs**](./multiSourceInputs.md) - Using inputs (rather and subscriptions) can accept value signals from multiple sources. This section discusses the various tools HELICS provides for managing how to handle/resolve what can be conflicting or inconsistent signal data.
- [**Queries**](./queries.md) - How queries can be used to get information on HELICS brokers, federates, and cores
- [**Cores**](./CoreTypes.md) - Discussion of the different types of message-passing buses and their implementation as HELICS cores
- **Multiple Brokers**
	- [**Connecting Multiple Core Types**](./multibroker.md) What to do when one type of communication isn't sufficient.
	- [**Broker Hierarchies**](./broker_hierarchies.md) - Purpose of broker hierarchies and how to
- **Reiteration (forthcoming)** - Discussion of why reiteration is used and how to successfully execute it in HELICS
- [**Simultaneous co-simulations**](./simultaneous_cosimulations.md) - Options for running multiple independent co-simulations on a single system
- [**Orchestration Tool (Merlin)**](./orchestration.md) Brief guide on using [Merlin](https://github.com/LLNL/merlin) to handle situations where a HELICS co-simulation is just one step in an automated analysis process (_e.g._ uncertainty quantification) or where assistance is needed deploying a large co-simulation in an HPC environment.
- **Large Co-Simulations in HELICS (forthcoming)** - How to run HELICS co-simulations with a large (100+) number of federates
- **HELICS Timing Algorithm (forthcoming)** - Detailed description of the HELICS timing algorithm and the finer points of manipulating it for maximum co-simulation performance.
- **Value Message Types (forthcoming)** - Detailed description of the four types of value messages in HELICS and how they can be used to minimize federation configuration effort.

## Configuration Options Reference
[Configuration Options Reference](../configuration_options_reference.md) - Comprehensive reference to all the HELICS configuration options.

## Examples

## Trouble Shooting

[**Trouble Shooting Support**](./support.md) 
What to do when the co-simulations don't seem to be working correctly.