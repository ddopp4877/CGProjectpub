: L-type calcium current fit from Joey's data 11-7-2011


NEURON {
	SUFFIX cal
	USEION ca READ eca WRITE ica
	USEION cac READ caci VALENCE 2 
	RANGE G, g, i
}

UNITS {
	(mA) = (milliamp)
	(mV) = (millivolt)
	FARADAY = (faraday) (coulomb)
	R = (k-mole) (joule/degC)
}

PARAMETER {
	g = 0.9e-6 (siemens/cm2) <0,1e9>
	koh = 0.02 (1/msec)
	
	c1 = 40  : 20 0.7  :(uM)
	c2 = 45  : 45 0.6  :(uM)
	

}

ASSIGNED {
	v (mV)
	caci
	ek (mV)
	eca
	i
	ica (mA/cm2)
	G (siemens/cm2)
}

STATE {
	m
	h
}

BREAKPOINT {
	SOLVE states METHOD cnexp
	G = g*m*m*h

	i = G*(v-eca)
	ica = i
}

INITIAL {
	m = minf(v)
	h = hinf(caci)
}

DERIVATIVE states {
	m' = (minf(v)-m)/taum(v)
	h' = (hinf(caci)-h)*koh
}


FUNCTION minf(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	minf = 1/(1+exp((v+24.75)/-5))
	:minf = 1/(1+exp((v+22.5)/-4))*
	:minf = 1/(1+exp((v+24)/-7.9))
}

FUNCTION taum(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	:taum =  60 
	taum = 20 + 50.2/(1+exp((v+20.25)/1))
	:taum = 20.98 + 12.09/(exp((v+0.9278)/-1)+exp((v-5.929)/1))*
}


FUNCTION hinf(ca) {
	hinf = c1/(c2+ca)
	:hinf = 1/(1+exp((ca-c1)/15))
}