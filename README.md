This is the implementation of my Bachelor's thesis project. 

# Neuro-Evolution

The folder "NeuroEvolution" contains the source code for two neuro evolutionary algorithms.

The first one is called "NEAT" ("NeuroEvolution of Augmented Topologies") and is an Algorithm discovered by Ken Stanley.

The second one is called "HNE" ("Historical Neuro Evolution") and was developed by me. 

## Unreal Engine Project

This is an application of the NEAT algorithm.

50 agents are trying to reach the goal and must not collide with anything (except the ground and other agents)

The agents are equipped with sensors:

![Sensoric](/readme_files/Sensoric.gif)

The agents are set to inactive if they collide or fall of the map.

Furthermore, they must progress or they get set to inactive.

If every agent is set to inactive a new iteration starts:

![Sensoric](/readme_files/Iteration.gif)

## Main sources for the devlopment of NEAT

[Downloadlink to in depth description of NEAT!](https://repositories.lib.utexas.edu/bitstream/handle/2152/1266/stanleyk74304.pdf)

AI Techniques For Game Programming Mat Buckland 2002
