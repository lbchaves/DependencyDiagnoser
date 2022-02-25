# DependencyDiagnoser

General Purpose Dependency Analyzer and Optimizer

Why and how? 

Dependency appears in a variety of domains where items depend on each other; it can measure the strength of the relationships between items. Those items can be expressed in software (i.e., interdependence between software modules) or in hardware (i.e., in combinational circuits). 

When dependency is not properly handled it can have a negative effect on system behavior. It can have a negative impact on professional software development or on combinational circuits designed to be feed-forward or acyclic. This project uses a graph approach using C++ and Boost to analyze any collection of items (represented as nodes) and their mutual dependencies (represented as edges).


What are the functionalities provided?

A) Ordering and Processing Optimizer 

Given a vector of items (nodes) and their dependencies (edges) an ordering is provided respecting the existing dependencies. It is especially useful if an element (node) changes. If an item (node) changes it determines which other items (nodes) are affected by this change.

B) Cyclic Dependency Detection 

Detection of circular dependencies (or cyclic dependencies). This study has a degree of generality that allows its application in any domain, help to identify the level of coupling in Software Design and Software Development.
Note: when change in one module spreads into other modules and potentially can cause unwanted effects (program errors, compile errors); can also result in infinite recursion, loops and other unexpected failures; can cause memory leaks and even prevent certain more primitive automatic garbage collectors from deallocating unused resources; in C++ language dependency can appear due to circular "include” dependencies between files in a C++ project. 

C) More Applications 

- Spreadsheets: a formula that visits a cell more than once; 
- Combinational circuits designed to be feed-forward or acyclic (hardware); 
- Many others. 
