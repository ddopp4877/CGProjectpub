: Kd current fit from Ransdell & Schulz 2011 data


NEURON {
	SUFFIX kd2
	USEION k READ ek WRITE ik
	RANGE g1,g2,G1,G2
	RANGE minf, taum, i

	RANGE tbase, tamp, vhalf, k
}

UNITS {
	(mA) = (milliamp)
	(mV) = (millivolt)
}

PARAMETER {
	g1 = 0.783e-6 (siemens/cm2) <0,1e9>
	g2 = 0.3356e-6 (siemens/cm2) <0,1e9>
}

ASSIGNED {
	v (mV)
	ek (mV)
	ik (mA/cm2)
	G (siemens/cm2)
	i
}

STATE {
	m1 h1 m2
}

BREAKPOINT {
	SOLVE states METHOD cnexp
	G = g1*m1^4*h1 + g2*m2^4
	i = G*(v-ek)
	ik=i
}

INITIAL {
	m1 = minf1(v)
	h1 = hinf1(v)
	m2 = minf2(v)
}

DERIVATIVE states {
	m1' = (minf1(v)-m1)/taum1(v)
	h1' = (hinf1(v)-h1)/tauh1(v)
	m2' = (minf2(v)-m2)/taum2(v)
}


FUNCTION minf1(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	minf1 = 1/(1+exp((v+24.19)/-10.77))
}

FUNCTION minf2(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	minf2 = 1/(1+exp((v+23.32)/-10))   : Fit from Schulz data
}

FUNCTION hinf1(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	hinf1 = 0.3 + ((1-0.3)/(1+exp((v+15.87)/5.916)))  :Fit from Schulz data
}



FUNCTION taum1(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	taum1 = 25.049 + 25/(1+exp((v+25.84)/6.252))
	:taum1 = 5.049 + 25/(1+exp((v+25.84)/6.252))
}

FUNCTION taum2(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
	taum2 = 100 + 550/(1+exp((v+15)/12.46))
}

FUNCTION tauh1(v(mV)) {
	TABLE FROM -150 TO 150 WITH 500
    tauh1 = 550 + 954.9/(1+exp((v+10.8)/-15))  :tauh1 = 600 + 7936/(1+exp((v+13.98)/3.095))   
}
