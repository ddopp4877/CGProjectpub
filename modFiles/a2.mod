: IA fit from Ransdell & Schulz Jan 2012, modified by Jing Wang due to erroneous h curve 

: 

NEURON {
	SUFFIX a2
	USEION k READ ek WRITE ik
	RANGE g, G,i
	
}

UNITS {
	(mA) = (milliamp)
	(mV) = (millivolt)
}

PARAMETER {
	g = 0.00892 (siemens/cm2) <0,1e9>
}

ASSIGNED {
	v (mV)
	ek (mV)
	ik (mA/cm2)
	G (siemens/cm2)
	i
}

STATE {
	m h
}

BREAKPOINT {
	SOLVE states METHOD cnexp
	G = g*m*m*m*h
	i = G*(v-ek)
	ik=i
}

INITIAL {
	m = minf(v)
	h = hinf(v)
}

DERIVATIVE states {
	m' = (minf(v)-m)/taum(v)
	h' = (hinf(v)-h)/tauh(v)
}

FUNCTION minf(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	minf = 0.3463/(1+0.008685*exp(-v/5.033248))+0.75187/(1+1.11022*exp(-v/9.610637))+0.162947
}

FUNCTION taum(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	taum = 3.002 + 4.073/(1+exp((v+24.18)/2.592))
}



FUNCTION hinf(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	hinf = 0.93854475/(1+144209.656*exp(v/5.08660603))+0.02804584
}

FUNCTION tauh(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	tauh = 9.434 + 11.7/(1+exp((v+1)/5.317))
}