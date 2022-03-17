: spike-generating sodium channel (Pyramid)

NEURON {
	SUFFIX naax
	USEION na READ ena WRITE ina
	RANGE gnabar, gna, inaplot
	RANGE minf, hinf, mtau, htau
}

UNITS {
	(mA) = (milliamp)
	(mV) = (millivolt)
}

PARAMETER {
	gnabar = 0.300 (siemens/cm2) <0,1e9>
}

ASSIGNED {
	v (mV)
	ena (mV)
	ina (mA/cm2)
	minf
	hinf
	mtau (ms)
	htau (ms)
	gna (siemens/cm2)
	inaplot
}

STATE {
	m h
}

BREAKPOINT {
	SOLVE states METHOD cnexp
	gna = gnabar*m*m*m*h
	ina = gna*(v-ena)
	inaplot = ina
}

INITIAL {
	rate(v)
	m = minf
	h = hinf
}

DERIVATIVE states {
	rate(v)
	m' = (minf-m)/mtau
	h' = (hinf-h)/htau
}

PROCEDURE rate(v (mV)) {
	UNITSOFF

    if (v < -40 ) {
	minf = 0
	} else{
	minf = (1.0)/(1.0+(exp ((v+18.7)/(-5.29))))  :was 24.7
	}
    
	       
	mtau = (1.32) - ((1.26)/(1.0+(exp (-(v+120)/(25)))))         
	
	hinf = 1.0/(1.0+(exp ((v+43.9)/(5.18))))   :was 48.9    
	htau = ((0.67/(1+(exp (-(v+62.9)/(10.0)))))*(1.5+(1.0/(1+(exp ((v+34.9)/(3.6)))))))*10 :10 added    
	UNITSON
}
