NEURON {
  POINT_PROCESS GAP
  NONSPECIFIC_CURRENT i
  POINTER vgap
  RANGE r,i 
}



PARAMETER { r = 1e10 (megaohm)}

ASSIGNED {
v    (millivolt)
vgap (millivolt)
i    (nanoamp)
  }

BREAKPOINT {i = (v-vgap)/r}

