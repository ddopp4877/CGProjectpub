:NaP current fit from Ransdell & Schulz 2011 data in Jan 2012


NEURON {
	SUFFIX nap2
	USEION na READ ena WRITE ina
	RANGE G, g
	RANGE minf, taum, hinf, tauh, i
}

UNITS {
	(mA) = (milliamp)
	(mV) = (millivolt)
}

PARAMETER {
	g = 0.012 (siemens/cm2) <0,1e9>
	q = 0
}

ASSIGNED {
	v (mV)
	ena
	i
	ina (mA/cm2)
	G (siemens/cm2)
}

STATE {
	m h
}

BREAKPOINT {
	SOLVE states METHOD cnexp
	G = g*m*m*m*h^q
	i = G*(v-ena)
	ina = i
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
	minf = 1/(1+exp((v+32.7)/-18.81))* 1/(1+exp((v+35)/-0.5))
	:minf = 1/(1+exp((v+32.7)/-18.81))

}

FUNCTION taum(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	taum = 3.15 + 0.8464/(exp((v+0.8703)/-6.108))
}


FUNCTION hinf(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	hinf = 1/(1+exp((v+48.5)/4.8))
}

FUNCTION tauh(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	tauh = 666 - 379/(1 + exp((v+33.6)/-11.7))
}
