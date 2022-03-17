: Calcium Pool dynamics including ca-current dependent influx, buffering


NEURON {
  SUFFIX pool
  USEION ca READ ica 
  USEION cac READ caci WRITE caci, caco VALENCE 2 
  RANGE fcac, tau, cainf,f
}

UNITS {
        (mM) = (milli/liter)
        (mA) = (milliamp)
	(mV) = (millivolt)

}

PARAMETER {
	f = 0.256e6   : uM/nA * (1e9 nA / 1e3 mA)
	tau = 690   : 640   : ms
	cainf = 5e-4 (mM)

}

INITIAL {
	caci = cainf  
	caco = 13
}

STATE { caci (mM) 

} 

ASSIGNED {
	v (mV)
	ica (mA/cm2)
 area
 caco
}

BREAKPOINT {
  SOLVE states METHOD cnexp

}


DERIVATIVE states { 

	caci'= 1/tau*(-f*ica*area/1e8 + (cainf - caci))
	caco = 13
}
