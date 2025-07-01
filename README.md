# CCF_PROJ

1.	01BuildNetwork: Create the system with initial coordinates 

    a.	Assign an initial position vector to each atom.

    b.	Assign the appropriate charge to each atom. 

2.	02CompressNetwork: Compress the system with LJ potential and compression at T=1.0 and rho=0.85

    a.	Fix langevin

    b.	Fix NVE

    c.	Fix deform

3.	03EquilNPT: Equilibrate the system at T=[0.45 0.65 0.85 1.05 1.20] and p = 0.0 (no external pressure only pressure from interactions)

    a.	Run at different temperatures 

    b.	Fix NPT – Nose hoover thermostat and barostat 

    c.	Find the correct average volume at that state point

    d.	Turn on charges with no electric field

4.	Deform to avg vol

    a.	Fix langevin 

    b.	Fix deform and deform box size to avg vol from 3

    c.	Fix nve  

    d.	Charges remain on

5.	Production run for T = […] q = […] & E = 0 equilibrium simulation in NVT ensemble

    a.	Charges remain on

    b.	Nose hoover (NVT), langevin + NVE, & DPD + NVE

6.	Production run for T = […] q = […] & E = […] non-equilibrium simulation

    a.	Apply electric field and check if you reached steady state

    b.	When the sum of the dipole projection in z doesn’t change => steady state
  
    c.	Nose Hoover (NVT), langevin + NVE (maybe), DPD + NVE

    d.	Maybe apply barostat in x,y plane

7.	Equilibrate after applying electric field

    a.	Electric field is turned off
  
    b.	Fix DPD and NVE  

