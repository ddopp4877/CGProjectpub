

NEURON {
	SUFFIX kd
	USEION k READ ek WRITE ik
	RANGE g, G
	RANGE minf, taum, i

	RANGE tbase, tamp, vhalf, k
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
	i
}

STATE {
	m
}

BREAKPOINT {
	SOLVE states METHOD cnexp
	G = g*m*m*m*m
	i = G*(v-ek)
	ik=i
}

INITIAL {
	rate(v)
	m = minf
}

DERIVATIVE states {
	rate(v)
	m' = (minf-m)/taum
}

PROCEDURE rate(v (mV)) {
	UNITSOFF
	
	minf = minffun(v)
	taum = taumfun(v)

	UNITSON
}

FUNCTION minffun(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	minffun = 1/(1+exp((v+13.3)/-9.8))
}

FUNCTION taumfun(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	taumfun = 104.4 - 12.8/(1+exp((v+28.3)/-19.2))  :taumfun = 14.4 - 12.8/(1+exp((v+28.3)/-19.2))
}