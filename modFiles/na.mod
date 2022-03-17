:Fast spiking sodium current from Turrigiano et al. 1995


NEURON {
	SUFFIX na
	USEION na READ ena WRITE ina
	RANGE g, G
	RANGE minf, taum, hinf, tauh, i
	
}

UNITS {
	(mA) = (milliamp)
	(mV) = (millivolt)
}

PARAMETER {
	g = 0.012 (siemens/cm2) <0,1e9>
}

ASSIGNED {
	v (mV)
	ek (mV)
	ik (mA/cm2)
	G (siemens/cm2)
	minf
	taum (ms)
	hinf
	tauh (ms)
	i
	ina
	ena (mV)
}

STATE {
	m
	h
}

BREAKPOINT {
	SOLVE states METHOD cnexp
	G = g*m*m*m*h
	i = G*(v-ena)
	ina=i
}

INITIAL {
	rate(v)
	m = minf
	h = hinf
}

DERIVATIVE states {
	rate(v)
	m' = (minf-m)/taum
	h' = (hinf-h)/tauh
}

PROCEDURE rate(v (mV)) {
	UNITSOFF
	
	minf = minffun(v)
	taum = taumfun(v)
	
	hinf = hinffun(v)
	tauh = tauhfun(v)
	
	UNITSON
}

FUNCTION minffun(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	minffun = 1/(1+exp((v+18.5)/-5.29))
}

FUNCTION taumfun(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	taumfun = 1.34 - 1.26/(1+exp((v+120)/-25))
}

FUNCTION hinffun(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	hinffun = 1/(1+exp((v+48.9)/5.18))
}

FUNCTION tauhfun(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	tauhfun = (0.67/(1+exp((v+62.9)/-10)))*(1.5 + 1/(1+exp((v+34.9)/3.6)))
}