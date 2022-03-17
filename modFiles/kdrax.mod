: potassium delayed rectifier channel (Pyramid)

NEURON {
	SUFFIX kdrax
	USEION k READ ek WRITE ik
	RANGE gkdrbar, gkdr, ikd
	RANGE inf, tau
}

UNITS {
	(mA) = (milliamp)
	(mV) = (millivolt)
}

PARAMETER {
	gkdrbar = 1.860 (siemens/cm2) <0,1e9>
}

ASSIGNED {
	v (mV)
	ek (mV)
	ik (mA/cm2)
	gkdr (siemens/cm2)
	inf
	tau (ms)
	ikd
}

STATE {
	n
}

BREAKPOINT {
	SOLVE states METHOD cnexp
	gkdr = gkdrbar*n*n*n*n
	ik = gkdr*(v-ek)
	ikd = ik
}

INITIAL {
	rate(v)
	n = inf
}

DERIVATIVE states {
	rate(v)
	n' = (inf-n)/tau
}

PROCEDURE rate(v (mV)) {
	UNITSOFF
    if (v < -40 ) {
	inf = 0
	} else{
	inf = 1.0/(1+ (exp ((v+9.2)/(-11.8)))) :was 14.2
        }
	tau = (7.2-(6.4/(1+(exp (-(v+28.3)/(19.2))))))*2 :Delaying the potassium with x2
	UNITSON
}
